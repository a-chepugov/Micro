// Разработать переносимую модель данных (набор функции для доступа и обработки данных)
#define PRINT_TECHNICAL_MESSAGES

#ifdef PRINT_TECHNICAL_MESSAGES
#endif

void abort(int ErrorNum)
{
  Serial.print ('\n');
  switch ( ErrorNum ) {
  case 0:
    Serial.print ("Array indexing error");
    break;
  default:
    Serial.print ("Unknown Error");
    break;
  };
  Serial.print ('\n');
  abort();
};

#include <Stepper.h>
#include "SharedData.h"
#include "Coordinates.h"
#include "Sensors.h"
#include "Command.h"
#include "System.h"
#include "Carriage.h"
#include "Piezoer.h"
#include "Carriage_Piezo.h"
#include "Carriage_StepMotor.h"

void print(char * str, char stat = '@'); 
unsigned long checktime(unsigned long starttime = 0); 

//===================================================================================================

void StepMotor3::AxisMove(char AxisNum, Coordinate & CurrientCoordinate, Coordinate NewCoordinate) { 

#ifdef PRINT_TECHNICAL_MESSAGES
  print ("HeadMove start");
#endif

  Serial.print ("\nAxis ");
  Serial.print (int(AxisNum));
  Serial.print (" : ");
  Serial.print (CurrientCoordinate);
  Serial.print (" -> ");
  Serial.print (NewCoordinate);
  Serial.print (" = ");
  Serial.print ( (int) (NewCoordinate - CurrientCoordinate) );
  (*(Stepper *)Movers[AxisNum]).step( (int) (NewCoordinate - CurrientCoordinate) );
  CurrientCoordinate = NewCoordinate;

#ifdef PRINT_TECHNICAL_MESSAGES
  print ("HeadMove end");
#endif
};

void Piezo3::AxisMove(char AxisNum, Coordinate & CurrientCoordinate, Coordinate NewCoordinate)
{
#ifdef PRINT_TECHNICAL_MESSAGES
  print ("TipMove start");
#endif

  Serial.print ("\nAxis ");
  Serial.print (int(AxisNum));
  Serial.print (" : ");
  Serial.print (CurrientCoordinate);
  Serial.print (" -> ");
  Serial.print (NewCoordinate);
  Serial.print (" = ");
  Serial.print ( (int) (NewCoordinate - CurrientCoordinate) );
  (*(Piezo *)Movers[AxisNum]).TakePosition( NewCoordinate );
  CurrientCoordinate = NewCoordinate;

#ifdef PRINT_TECHNICAL_MESSAGES
  print ("TipMove end");
#endif
};

//===================================================================================================

SensorsSystemData::SensorsSystemData()
{
  pinMode(PIN_SENSOR_TIP, INPUT); // Установка режима чтения контактов сенсора острия
  pinMode(PIN_SENSOR_Ptip, INPUT); // Установка режима чтения контактов сенсора давления
  pinMode(PIN_SENSOR_T, INPUT); // Установка режима чтения контактов сенсора температуры
  pinMode(PIN_SENSOR_Patm, INPUT); // Установка режима чтения контактов сенсора давления окружающей среды
  pinMode(PIN_SENSOR_HUMIDITY, INPUT); // Установка режима чтения контактов сенсора влажности
  pinMode(PIN_SENSOR_VIBRATION, INPUT); // Установка режима чтения контактов сенсора вибрации
};

void SensorsSystemData::GetSensorsValue() {
  SensorsDataReset();
  SensorsValue[Itip].Fx = analogRead(PIN_SENSOR_TIP);
  SensorsValue[Ptip].Fx = analogRead(PIN_SENSOR_Ptip);
  SensorsValue[T].Fx = analogRead(PIN_SENSOR_T);
  SensorsValue[Patm].Fx = analogRead(PIN_SENSOR_Patm);
  SensorsValue[Humidity].Fx = analogRead(PIN_SENSOR_HUMIDITY);
  SensorsValue[Vibration].Fx = analogRead(PIN_SENSOR_VIBRATION);
};

//===================================================================================================

class MasterRecord {
private:
  SystemData System; // Системные данные
  CommandData Command; // Данные команды
  SensorsSystemData Sensors; // Данные сенсоров
  DataArray < Carriage *, 2 > Carriages; // Массив суппортов
public:
  MasterRecord();
  inline void Processing(); // Ожидание команды
  inline void ReadCommand(); // Получение команды
  inline void CheckCommand(); // Проверка корректности комманды
  inline void ExecuteCommand(); // Выполнение полученной команды

  inline void MoveToPoint(Coordinate TargetPosition[]); // Безусловное перемещение в новую точку
  inline void MoveToPointSafe(Coordinate TargetPosition[]); // Безопасное перемещение в новую точку
  inline void SafeAxisMove(char AxisNum, bool ReverseMoveDirection); // Подведение к образцу
  inline void MovePrepareActions(); // Перемещение в заданные координаты и снятие данных датчиков
  inline bool SafetyCheck(bool Position); // Проверка безопасности для острия  
  inline bool CanContinueScan(); // Проверка возможности дальнейшего сканирования

  
  inline void SystemDataScan(); // Сканирование
  inline void PointPricking(); // Операция индентирование

  inline void PrintResult(); // Вывод данных операции
  inline void PrintFullResult(); // Вывод данных операции
  inline void PrintSystemIdle(); // Вывод состояния системы во время простоя
}; 

MasterRecord::MasterRecord() :
Sensors()  {
  *Carriages[0] = StepMotor3();
  *Carriages[1] = Piezo3();
};

void MasterRecord::Processing() {
  unsigned long time = checktime(0); // Функция рассчета затраченого времени  
  Sensors.GetSensorsValue();
  if (Serial.find(COMMAND_PREFIX)) {
    System.State() = STATE_PREFIX_WORKING;
    ReadCommand();
    CheckCommand();
    if ( !System.Errors(BadCommand) or true) {
      ExecuteCommand();
      SystemData();
    };
  }
  else {
    System.State() = STATE_PREFIX_IDLE_STANDING ;
    PrintSystemIdle();
  };
  checktime(time); // Функция рассчета затраченого времени  
};

void MasterRecord::ReadCommand() {
  Serial.readBytes(Command.fromChar(), Command.Size());
};

void MasterRecord::CheckCommand() {
  if(!Command.CheckCSum()) 
  {
    System.Errors(BadCommand) = true;
  };
};

inline void MasterRecord::MoveToPointSafe(Coordinate TargetPosition[]) {
};

inline bool MasterRecord::SafetyCheck(bool Position) {
  Sensors.GetSensorsValue();
  if ( Sensors.SensorsValue[Itip].Fx < Command.Parameters[Istart]
    or Sensors.SensorsValue[Ptip].Fx < Command.Parameters[Pstart] )
  {
    return true;
  }
  else
  {
    return false;
  };
};

inline bool MasterRecord::CanContinueScan()
{
  Sensors.GetSensorsValue();
  if ( Sensors.SensorsValue[Itip].Fx < Command.Parameters[Icrit]
    or Sensors.SensorsValue[Ptip].Fx < Command.Parameters[Pcrit] )
  {
    return true;
  }
  else
  {
    return false;
  };
};

inline void MasterRecord::SafeAxisMove(char AxisNum, bool ReverseMoveDirection) {
  for(Coordinate i = 0; i < 256 ^ sizeof(Coordinate); i++)
  {
    if( !(SafetyCheck(Max)) )
    {
      break;
    };
    Carriages[0]->AxisMove (AxisNum, Carriages[0]->Location.Position(AxisNum), Carriages[0]->Location.Position(AxisNum) + 1 - 2 * ReverseMoveDirection);
  }; 
};

void MasterRecord::ExecuteCommand() {
#ifdef PRINT_TECHNICAL_MESSAGES
  print ("ExecuteCommand start");
#endif

  switch (Command.Parameters[CommandName]) {
  case COMMAND_PREFIX_MOVE:
    MovePrepareActions();
    PrintResult();
    PrintFullResult();
    break;
  case COMMAND_PREFIX_SCANNING:
    MovePrepareActions();
    SystemDataScan();
    PrintResult();
    PrintFullResult();
    break;
  case COMMAND_PREFIX_PROFILING:
    MovePrepareActions();
    SystemDataScan();
    PrintResult();
    PrintFullResult();
    break;
  case COMMAND_PREFIX_INDENTATION:
    MovePrepareActions();
    PrintResult();
    PrintFullResult();
    break;
  case COMMAND_PREFIX_CALIBRATION:
    MovePrepareActions();
    PrintResult();
    PrintFullResult();
    break;
  default:
    System.State() = STATE_PREFIX_IDLE_STANDING;
  };

#ifdef PRINT_TECHNICAL_MESSAGES
  print ("ExecuteCommand end");
#endif

};

void MasterRecord::MovePrepareActions () {

#ifdef PRINT_TECHNICAL_MESSAGES
  print("MovePrepareActions start");
  print ("CarriagesMove start");
#endif


  for (char i = 0; i < 1; i++)
  {
    print ("Carriage[", '[');
    Serial.print (int(i));
    Serial.print ("]\n");
    Carriages[i]->Move(&(Command.Parameters[HeadX + 3 * i]));
  };

#ifdef PRINT_TECHNICAL_MESSAGES
  print ("CarriagesMove end");
  print ("MovePrepareActions end");
#endif

};

void MasterRecord::SystemDataScan() {
#ifdef PRINT_TECHNICAL_MESSAGES
  print("SystemDataScan start");
#endif

  for (int i = 0; i < Sensors.ScanResult.Length(); i++)
  {
    //    print ("HeadScanMove start");
    Carriages[0]->ScanMove(Z);
    //    print ("HeadScanMove end");

    //    Sensors.SetScanResult(i, SensorData(*Carriages[0].GetPosition(Z), analogRead(PIN_SENSOR_TIP)));
    Sensors.SetScanResult(SensorData(Carriages[0]->GetPosition(Z), analogRead(PIN_SENSOR_TIP)));
    if ( !CanContinueScan() ) {
      break;
    };
  };

#ifdef PRINT_TECHNICAL_MESSAGES
  print ("SystemDataScan end");
#endif

};

void MasterRecord::PointPricking(){
  while (analogRead(PIN_SENSOR_Ptip) < Command.Parameters[Pcrit])
  {
    Carriages[0]->ScanMove(Z);
  };
};

void MasterRecord::PrintResult() { // Вывод данных операции
  print ("-----------------------------------------------------------------------------------------------------",'[');
  System.State() = STATE_PREFIX_OUTPUT_RESULT;

  Serial.print("\nScanState  : ");
  Serial.print(System.State());
  Serial.print("\nError      : ");
  Serial.print(System.GetErrorSymbol());
  Serial.print("\nCommand    : ");
  Serial.print(Command.Parameters[CommandName]);
  Serial.print("\nParameters : ");
  for (int i = 0; i < sizeof(Command.Parameters) / sizeof(Command.Parameters[0]); i++)
  {
    Serial.print(Command.Parameters[i]);
    Serial.print(" / ");
  };
  Serial.print("\nSensors    : ");
  for (int i = 0; i < COUNT_OF_SENSORS; i++)
  {
    Serial.print(Sensors.SensorsValue[i].Fx);
    Serial.print(" / ");
  };
  Serial.print("\nScanResult : ");
  for (int i = 0; (i < Sensors.ScanResult.Length()) && (i < Command.Parameters[ScanDeep]); i++)
  {
    Serial.print(Sensors.ScanResult[i].Fx);
    Serial.print(" / ");
  };
};

void MasterRecord::PrintFullResult() {
  print ("-----------------------------------------------------------------------------------------------------");
  CommandData *TempCommandParametersData;
  TempCommandParametersData = (CommandData *)(& Command);
  for (unsigned int i = 0; i < sizeof(CommandData); i++ )
  {
    Serial.write(TempCommandParametersData->DataToChar(i));
  };
  TempCommandParametersData = 0;
  SensorData *TempSensorData;
  for (unsigned int i = 0; i < Sensors.SensorsValue.Length(); i++ )
  {
    TempSensorData = & Sensors.SensorsValue[i];
    for (unsigned int j = 0; j < sizeof(SensorData); j++ )
    {
      Serial.write(TempSensorData->Chars[j]);
    };
  };
  for (unsigned int i = 0; (i < Sensors.ScanResult.Length()) && (i < Command.Parameters[ScanDeep]); i++)  
  {
    TempSensorData = & Sensors.ScanResult[i];
    for (unsigned int j = 0; j < sizeof(SensorData); j++ )
    {
      Serial.write(TempSensorData->Chars[j]);
    };
  };
  TempSensorData = 0;
  System.State() = STATE_PREFIX_OUTPUT_RESULT;
};

void MasterRecord::PrintSystemIdle() {
  Serial.write('\n');
  SystemData *TempSystemData;
  TempSystemData = (SystemData *)(& System);
  for (unsigned int i = 0; i < TempSystemData->Size(); i++ )
  {
    Serial.write(TempSystemData->DataChar(i));
  };
  TempSystemData = 0;
  SensorData *TempSensorData;
  for (unsigned int i = 0; i < Sensors.SensorsValue.Length(); i++ )
  {
    TempSensorData = & Sensors.SensorsValue[i];
    for (unsigned int j = 0; j < sizeof(SensorData); j++ )
    {
      Serial.write(TempSensorData->Chars[j]);
    };
  };
  TempSensorData = 0;
  System.State() = STATE_PREFIX_OUTPUT_RESULT;
};

MasterRecord System = MasterRecord(); // Объявление Объекта для работы с данными

void setup() {
  Serial.begin(SERIAL_SPEED); // Устанавливаем последовательное соединение
}

void loop() {
  delay(1000); 
  System.Processing();
}

unsigned long checktime(unsigned long starttime)
{
  unsigned long nowtime = micros(); 
  if (starttime)
  {
    Serial.print("\n============================================================== Used time: "); 
    Serial.print( ( nowtime - starttime ) / 1000 ); 
    Serial.print(" msec = "); 
  }; 
  return nowtime; 
}; 

void print(char * str, char stat)
{
  switch (stat) {
  case '[' :
    Serial.print("\n"); 
    Serial.print(str); 
    break; 
  case ']' :
    Serial.print(str); 
    Serial.print("\n"); 
    break; 
  case '-' :
    Serial.print(str); 
    break; 
  case '@' :
    Serial.print("\n"); 
    Serial.print(str); 
    Serial.print("\n"); 
    break; 
  };
};
