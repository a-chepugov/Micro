template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];
#define arraylength(array) (sizeof(ArraySizeHelper(array)))

template <typename TTypeName, char TArrayLength> struct DataArray
{
  TTypeName Array[TArrayLength];

  inline TTypeName & operator[](char Item)
  {
    if (Item >= 0 && Item < TArrayLength )
    {
      return Array[Item];
    }
    else
    {
      ::abort();
    };
  };

  inline int Length()
  {
    return arraylength(Array);
  };
};

#include <Stepper.h>
#include "Coordinates.h"
#include "Sensors.h"
#include "Command.h"
#include "System.h"
#include "Carriage.h"
#include "Carriage_StepMotor.h"
#include "Carriage_Piezo.h"

void abort(int ErrorNum)
{
  Serial.print ('\n');
  switch ( ErrorNum ) {
  case 0:
    Serial.print ("Array indexing error");
    break;
  default:
    Serial.print ("Unknown Error");
  };
  Serial.print ('\n');
  abort();
};

void print(char * str, char stat = '@'); 
unsigned long checktime(unsigned long starttime = 0); 


//===================================================================================================

void Carriage::AxisMove(char AxisNum, Coordinate & currient_coordinate, Coordinate new_coordinate) { 
  //  print ("AxisMove start");
  Serial.print ("\nAxis ");
  Serial.print (int(AxisNum));
  Serial.print (" : ");
  Serial.print (currient_coordinate);
  Serial.print (" -> ");
  Serial.print (new_coordinate);
  Serial.print (" = ");
  Serial.print ( (int) (new_coordinate - currient_coordinate) );
  (*(Stepper *)Movers[AxisNum]).step( (int) (new_coordinate - currient_coordinate) );
  currient_coordinate = new_coordinate;
  //  print ("AxisMove end");
};

//===================================================================================================

SensorsItem::SensorsItem()
{
  pinMode(PIN_SENSOR_TIP, INPUT); // Установка режима чтения контактов сенсора острия
  pinMode(PIN_SENSOR_Ptip, INPUT); // Установка режима чтения контактов сенсора давления
  pinMode(PIN_SENSOR_T, INPUT); // Установка режима чтения контактов сенсора температуры
  pinMode(PIN_SENSOR_HUMIDITY, INPUT); // Установка режима чтения контактов сенсора влажности
  pinMode(PIN_SENSOR_VIBRATION, INPUT); // Установка режима чтения контактов сенсора вибрации
};

void SensorsItem::GetSensorsValue() {
  SensorsDataReset();
  SensorsValue[Itip].Fx = analogRead(PIN_SENSOR_TIP);
  SensorsValue[Ptip].Fx = analogRead(PIN_SENSOR_Ptip);
  SensorsValue[T].Fx = analogRead(PIN_SENSOR_T);
  SensorsValue[Humidity].Fx = analogRead(PIN_SENSOR_HUMIDITY);
  SensorsValue[Vibration].Fx = analogRead(PIN_SENSOR_VIBRATION);
};

//===================================================================================================

class MasterRecord {
public:
  SystemItem System; // Системные данные
  CommandData Command; // Данные команды
  SensorsItem Sensors; // Данные сенсоров
  DataArray < Carriage, 2 > Carriages; // Массив суппортов

public:
  MasterRecord();
  inline void Processing(); // Ожидание команды
  inline void ReadCommand(); // Получение команды
  inline void CheckCommand(); // Проверка корректности комманды
  inline void ExecuteCommand(); // Выполнение полученной команды

  inline void MovePrepareActions(); // Перемещение в заданные координаты и снятие данных датчиков
  inline bool SafetyCheck(bool Position); // Проверка безопасности для острия
  inline bool CanContinueScan(); // Проверка возможности дальнейшего сканирования
  inline void SafeMove(Coordinate TargetPosition[]); // Безопсное Перемещение в новыю точку
  inline void Approach(char AxisNum, bool ReverseMoveDirection); // Подведение к образцу;
  inline void SystemDataScan(); // Сканирование

  inline void PointPricking(); // Операция индентирование
  inline void PrintResult(); // Вывод данных операции
  inline void PrintFullResult(); // Вывод данных операции
  inline void PrintSystemIdle(); // Вывод состояния системы во время простоя
}; 

MasterRecord::MasterRecord() :
Sensors()  {
  Carriages[0] = Carriage('S');
  Carriages[1] = Carriage('P');
};

void MasterRecord::Processing() {
  unsigned long time = checktime(0); // Функция рассчета затраченого времени  
  Sensors.GetSensorsValue();
  if (Serial.find(COMMAND_PREFIX)) {
    System.SetState(STATE_PREFIX_WORKING);
    ReadCommand();
    CheckCommand();
    if ( !System.GetError(BadCommand) or true) {
      ExecuteCommand();
      SystemData();
    };
  }
  else {
    System.SetState(STATE_PREFIX_IDLE_STANDING);
    PrintSystemIdle();
  };
  checktime(time); // Функция рассчета затраченого времени  
};

void MasterRecord::ReadCommand() {
  Serial.readBytes(Command.Chars, sizeof(Command));
};

void MasterRecord::CheckCommand() {
  if(!Command.CheckCSum()) 
  {
    System.SetError(BadCommand);
  };
};

inline void MasterRecord::SafeMove(Coordinate TargetPosition[]) {
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

inline void MasterRecord::Approach(char AxisNum, bool ReverseMoveDirection) {
  for(Coordinate i = 0; i < 256 ^ sizeof(Coordinate); i++)
  {
    if( !(SafetyCheck(ScanStart)) )
    {
      break;
    };
    Carriages[0].AxisMove (AxisNum, Carriages[0].Location.Position[AxisNum], Carriages[0].Location.Position[AxisNum] + 1 - 2 * ReverseMoveDirection);
  }; 
};

void MasterRecord::ExecuteCommand() {
  //  print ("ExecuteCommand start");
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
    System.SetState(STATE_PREFIX_IDLE_STANDING); 
  };
  //  print ("ExecuteCommand end");
};

void MasterRecord::MovePrepareActions () {
  //  print("MovePrepareActions start");
  //  print ("CarriagesMove start");
  for (char i = 0; i < 1; i++)
  {
    print ("Carriage[", '[');
    Serial.print (int(i));
    Serial.print ("]\n");
    Carriages[i].Move(&(Command.Parameters[HeadX + 3 * i]));
  };
  //  print ("CarriagesMove end");
  //  print ("MovePrepareActions end");
};

void MasterRecord::SystemDataScan() {
  //  print("SystemDataScan start");
  for (int i = 0; i < Sensors.ScanResult.Length(); i++)
  {
    //    print ("HeadScanMove start");
    Carriages[0].ScanMove(Z);
    //    print ("HeadScanMove end");

    //    Sensors.SetScanResult(i, SensorData(Carriages[0].GetPosition(Z), analogRead(PIN_SENSOR_TIP)));
    Sensors.SetScanResult(SensorData(Carriages[0].GetPosition(Z), analogRead(PIN_SENSOR_TIP)));
    if ( !CanContinueScan() ) {
      break;
    };
  };
  //  print ("SystemDataScan end");
};

void MasterRecord::PointPricking(){
  //  print("PointPricking start");
  while (analogRead(PIN_SENSOR_Ptip) < Command.Parameters[Pcrit])
  {
    Carriages[0].ScanMove(Z);
  };
  //  print ("PointPricking end");
};

void MasterRecord::PrintResult() { // Вывод данных операции
  //  print ("PrintResult start");
  print ("-----------------------------------------------------------------------------------------------------",'[');
  System.SetState (STATE_PREFIX_OUTPUT_RESULT);

  Serial.print("\nScanState  : ");
  Serial.print(System.GetState());
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
  //  print("PrintResult end");
};

void MasterRecord::PrintFullResult() {
  //  print ("PrintFullResult start");
  print ("-----------------------------------------------------------------------------------------------------");
  CommandData *TempCommandParametersData;
  TempCommandParametersData = (CommandData *)(& Command);
  for (unsigned int i = 0; i < sizeof(CommandData); i++ )
  {
    Serial.write(TempCommandParametersData->Chars[i]);
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
  System.SetState (STATE_PREFIX_OUTPUT_RESULT);
  //  print ("PrintFullResult end");
};

void MasterRecord::PrintSystemIdle() {
  Serial.write('\n');
  SystemData *TempSystemData;
  TempSystemData = (SystemData *)(& System);
  for (unsigned int i = 0; i < sizeof(SystemData); i++ )
  {
    Serial.write(TempSystemData->Chars[i]);
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
  System.SetState (STATE_PREFIX_OUTPUT_RESULT);
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
