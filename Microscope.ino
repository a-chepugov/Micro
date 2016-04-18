// Разработать переносимую модель данных (набор функции для доступа и обработки данных)
#define PRINT_TECHNICAL_MESSAGES

#ifdef PRINT_TECHNICAL_MESSAGES
#endif

//==============================================================================

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
#include "System.h"
#include "Command.h"
#include "Sensors.h"
#include "Coordinates.h"
#include "Carriage.h"
#include "Carriage_Stepper3.h"
#include "Piezoer.h"
#include "Carriage_Piezo3.h"

void print(char * str, char stat = '@'); 
unsigned long checktime(unsigned long starttime = 0); 

//==============================================================================

void Stepper3::AxisMove(char AxisNum, Coordinate & CurrientCoordinate, Coordinate NewCoordinate)
{ 
#ifdef PRINT_TECHNICAL_MESSAGES
  print ("HeadMove start");
  Serial.print ("\nAxis ");
  Serial.print (int(AxisNum));
  Serial.print (" : ");
  Serial.print (CurrientCoordinate);
  Serial.print (" -> ");
  Serial.print (NewCoordinate);
  Serial.print (" = ");
  Serial.print ( (int) (NewCoordinate - CurrientCoordinate) );
#endif

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
  Serial.print ("\nAxis ");
  Serial.print (int(AxisNum));
  Serial.print (" : ");
  Serial.print (CurrientCoordinate);
  Serial.print (" -> ");
  Serial.print (NewCoordinate);
  Serial.print (" = ");
  Serial.print ( (int) (NewCoordinate - CurrientCoordinate) );
#endif  

  (*(Piezoer *)Movers[AxisNum]).GoToPosition( NewCoordinate );
  CurrientCoordinate = NewCoordinate;

#ifdef PRINT_TECHNICAL_MESSAGES
  print ("TipMove end");
#endif
};

//==============================================================================

void EnvironmentData::GetEnvironmentData()
{
  for (unsigned char i = COUNT_OF_SCAN_SENSORS; i < COUNT_OF_SENSORS; i++)
  {
    SensorsValue[i] = analogRead(SensorsPins[i]);
  };
};


SensorsData::SensorsData()
{
  for (unsigned char i = 0; i < COUNT_OF_SENSORS; i++)
  {
    pinMode(SensorsPins[i], INPUT); // Установка режима чтения контактов сенсора острия
  };
};

//==============================================================================

class MasterRecord
{
private:
  SystemData_C System; // Системные данные
  CommandData_C Command; // Данные команды
  SensorsData Sensors; // Данные сенсоров
  DataArray < Carriage *, 2 > Carriages; // Массив суппортов
public:
  MasterRecord();
  inline void StartupConfiguration(); // Начальная настройка системы
  inline void Processing(); // Обработка состояния системы
  inline void ReadCommand(); // Получение команды
  inline void CheckCommand(); // Проверка корректности комманды
  inline void ExecuteCommand(); // Выполнение полученной команды

  inline void MoveToPointSafe(Coordinate TargetPosition[]); // Безопасное перемещение в новую точку
  inline void AxisMoveSafe(char AxisNum, bool ReverseMoveDirection); // Подведение зонда к образцу
  inline void MovePrepareActions(); // Перемещение в заданные координаты и снятие данных датчиков
  inline bool SafetyCheck(bool Position); // Проверка безопасности для острия  
  inline bool CanContinueScan(); // Проверка возможности дальнейшего сканирования

  inline void PointScanning(); // Операция cканирования
  inline void PointPricking(); // Операция индентирование

  inline void PrintResult(); // Вывод данных операции
  inline void PrintFullResult(); // Вывод данных операции
  inline void PrintSystemConditions(); // Вывод состояния системы во время простоя
}; 

MasterRecord::MasterRecord() :
Sensors() {
  Carriages[0] = new Stepper3( DecartCoordinates ( '!!', '!!', '!!' ));
  Carriages[1] = new Piezo3( DecartCoordinates (127, 127, 127 ));
};

void MasterRecord::StartupConfiguration()
{
  // Поиск нулевой позиции
  // Сброс параметров
};

void MasterRecord::Processing()
{
  unsigned long time = checktime(0); // Функция рассчета затраченого времени  
  if (Serial.find(COMMAND_PREFIX))
  {
    System.Data.State() = STATE_PREFIX_WORKING;
    ReadCommand();
    CheckCommand();
    if ( !System.Data.Errors(BadCommand) or true) {
      ExecuteCommand();
      SystemData_C();
    };
  }
  else {
    Sensors.Environment.Data.GetEnvironmentData();
    PrintSystemConditions();
  };
  System.Data.State() = STATE_PREFIX_IDLE_STANDING; 
  checktime(time); // Функция рассчета затраченого времени  
};

void MasterRecord::ReadCommand()
{
  Serial.readBytes(Command.Chars.Pointer(), Command.Chars.Length());
};

void MasterRecord::CheckCommand()
{
  if(!Command.Data.CheckCSum()) 
  {
    System.Data.Errors(BadCommand) = true;
  };
};

void MasterRecord::MoveToPointSafe(Coordinate TargetPosition[])
{
};

bool MasterRecord::SafetyCheck(bool Position)
{
  Sensors.Environment.Data.GetEnvironmentData();
  if ( Sensors.Environment.Data.Item(Itip) < Command.Data.Item(Istart)
    or Sensors.Environment.Data.Item(Ptip) < Command.Data.Item(Ptipstart) )
  {
    return true;
  }
  else
  {
    return false;
  };
};

bool MasterRecord::CanContinueScan()
{
  Sensors.Environment.Data.GetEnvironmentData();
  if ( Sensors.Environment.Data.Item(Itip) < Command.Data.Item(Icrit)
    or Sensors.Environment.Data.Item(Ptip) < Command.Data.Item(Ptipcrit) )
  {
    return true;
  }
  else
  {
    return false;
  };
};

void MasterRecord::AxisMoveSafe(char AxisNum, bool ReverseMoveDirection)
{
  for(Coordinate i = 0; i < 256 ^ sizeof(Coordinate); i++)
  {
    if( !(SafetyCheck(Max)) )
    {
      break;
    };
    Carriages[0]->AxisMove (AxisNum, Carriages[0]->GetPosition()[AxisNum], Carriages[0]->GetPosition()[AxisNum] + 1 - 2 * ReverseMoveDirection);
  }; 
};

void MasterRecord::ExecuteCommand()
{
#ifdef PRINT_TECHNICAL_MESSAGES
  print ("ExecuteCommand start");
#endif

  switch (Command.Data.Item(CommandName)) {
  case COMMAND_PREFIX_MOVE:
    MovePrepareActions();
    PrintResult();
    PrintFullResult();
    break;
  case COMMAND_PREFIX_SCANNING:
    MovePrepareActions();
    PointScanning();
    PrintResult();
    PrintFullResult();
    break;
  case COMMAND_PREFIX_PROFILING:
    MovePrepareActions();
    PointScanning();
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
    System.Data.State() = STATE_PREFIX_IDLE_STANDING;
  };

#ifdef PRINT_TECHNICAL_MESSAGES
  print ("ExecuteCommand end");
#endif
};

void MasterRecord::MovePrepareActions()
{
#ifdef PRINT_TECHNICAL_MESSAGES
  print("MovePrepareActions start");
  print ("CarriagesMove start");
#endif

  for (char i = 0; i < 1; i++)
  {
    print ("Carriage[", '[');
    Serial.print (int(i));
    Serial.print ("]\n");
    Carriages[i]->Move(&(Command.Data.Item(HeadX + 3 * i)));
  };

#ifdef PRINT_TECHNICAL_MESSAGES
  print ("CarriagesMove end");
  print ("MovePrepareActions end");
#endif
};

void MasterRecord::PointScanning()
{
#ifdef PRINT_TECHNICAL_MESSAGES
  print("SystemDataScan start");
#endif

  for (int i = 0; i < Sensors.Environment.Data.Length(); i++)
  {
    //    print ("HeadScanMove start");
    Carriages[0]->ScanMove(Z, 1);
    //    print ("HeadScanMove end");
    //    Sensors.SetScanResult(i, SensorData(*Carriages[0].GetPosition(Z), analogRead(PIN_SENSOR_TIP)));
    Sensors.Scan.Data.PushNewScanResult(Carriages[0]->GetPosition()[Z], analogRead(PIN_SENSOR_Itip));
    if ( !CanContinueScan() ) {
      break;
    };
  };

#ifdef PRINT_TECHNICAL_MESSAGES
  print ("SystemDataScan end");
#endif
};

void MasterRecord::PointPricking()
{
  while (analogRead(PIN_SENSOR_Ptip) < Command.Data.Item(Ptipcrit))
  {
    Carriages[0]->ScanMove(Z, 1);
  };
};

void MasterRecord::PrintResult()
{
  print ("-----------------------------------------------------------------------------------------------------",'[');
  System.Data.State() = STATE_PREFIX_OUTPUT_RESULT;
  Serial.print("\nScanState  : ");
  Serial.print(System.Data.State());
  Serial.print("\nError      : ");
  Serial.print(System.Data.GetErrorSymbol());
  Serial.print("\nCommand    : ");
  Serial.print(Command.Data.Item(CommandName));
  Serial.print("\nParameters : ");
  for (int i = 0; i < Command.Data.Length(); i++)
  {
    Serial.print(Command.Data.Item(i));
    Serial.print(" / ");
  };
  Serial.print("\nSensors    : ");
  for (int i = 0; i < COUNT_OF_SENSORS; i++)
  {
    Serial.print(Sensors.Environment.Data.Item(i));
    Serial.print(" / ");
  };
  Serial.print("\nScanResult : ");
  for (int i = 0; (i < Sensors.Scan.Data.Length()) && (i < Command.Data.Item(ScanDeep)); i++)
  {
    Serial.print(Sensors.Scan.Data.Item(i).SensorValue);
    Serial.print(" / ");
  };
};

void MasterRecord::PrintFullResult()
{
  print ("-----------------------------------------------------------------------------------------------------");


  for (unsigned short int i = 0; i < Command.Chars.Length(); i++ )
  {
    Serial.write(Command.Chars[i]);
  };
  for (unsigned int i = 0; i < Sensors.Environment.Chars.Length(); i++ )
  {
    Serial.write(Sensors.Environment.Chars[i]);
  };
  for (unsigned int i = 0; i < Sensors.Scan.Chars.Length(); i++ )
  {
    Serial.write(Sensors.Scan.Chars[i]);
  };
  System.Data.State() = STATE_PREFIX_OUTPUT_RESULT;
};

void MasterRecord::PrintSystemConditions()
{
  Serial.write('\n');
  for (unsigned int i = 0; i < System.Chars.Length(); i++ )
  {
    Serial.write(System.Chars[i]);
  };

  for (unsigned int i = 0; i < Sensors.Environment.Chars.Length(); i++ )
  {
    Serial.write(Sensors.Environment.Chars[i]);
  };

  System.Data.State() = STATE_PREFIX_OUTPUT_RESULT;
};

MasterRecord MainSystem = MasterRecord(); // Объявление объекта для работы с данными

void setup()
{
  Serial.begin(SERIAL_SPEED); // Установка последовательного соединения
  MainSystem.StartupConfiguration();
}

void loop()
{
  delay(1000); 
  MainSystem.Processing();
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




