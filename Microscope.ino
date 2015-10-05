#include <Stepper.h>
#include "SharedData.h"
#include "Sensors.h"
#include "Command.h"
#include "System.h"
#include "Carriage.h"
#include "Carriage_StepMotor.h"
#include "Carriage_Piezo.h"

void print(char * str, char stat = '@'); 
unsigned long checktime(unsigned long starttime = 0); 

void Carriage::AxisMove(Coordinate *currient_coordinate, Coordinate new_coordinate, char AxisNum) { // Перемещение головки по одной координате
  print ("AxisMove start");
  Serial.print ("Axis ");
  Serial.print (AxisNum);
  Serial.print (" : ");
  Serial.print (*currient_coordinate);
  Serial.print (" -> ");
  Serial.print (new_coordinate);
  Serial.print (" = ");
  Serial.print ( (int) (new_coordinate - *currient_coordinate) );
  (*(Stepper *)Movers[AxisNum]).step( (int) (new_coordinate - *currient_coordinate) );
  *currient_coordinate = new_coordinate;
  print ("AxisMove end");
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

void SensorsItem::GetEnvironmentProperties() {
  for (char i = 0; i < sizeof(sizeof(SensorsValue) / sizeof(SensorsValue[0])); i++)
  { 
    SensorsValue[i] = SensorData();
  };
  SensorsValue[Itip].Fx = analogRead(PIN_SENSOR_TIP);
  SensorsValue[Ptip].Fx = analogRead(PIN_SENSOR_Ptip);   
  SensorsValue[T].Fx = analogRead(PIN_SENSOR_T);
  SensorsValue[Humidity].Fx = analogRead(PIN_SENSOR_HUMIDITY);
  SensorsValue[Vibration].Fx = analogRead(PIN_SENSOR_VIBRATION);
};

//===================================================================================================

class SystemRecord {
private:
  SystemItem System;
  CommandItem Command;
  SensorsItem Sensors;
  Carriage Carriages[2]; // Массив суппортов
public:
  SystemRecord();
  inline void Processing(); // Ожидание команды
  inline void ReadCommand(); // Получение команды
  inline void CheckCommand(); // Проверка корректности комманды
  inline void ExecuteCommand(); // Выполнение полученной команды
  inline void MovePrepareActions(); // Перемещение в заданные координаты и снятие данных датчиков
  inline void SystemDataScan(); // Сканирование
  inline void PointPricking(); // Операция индентирование
  inline void PrintResult(); // Вывод данных операции
  inline void PrintFullResult(); // Вывод данных операции
}; 

SystemRecord::SystemRecord() :
Sensors()  {
  Carriages[0] = Carriage('S');
  Carriages[1] = Carriage('P');  
};

void SystemRecord::Processing() {
  Serial.print (System.GetState());
  if (Serial.find(COMMAND_PREFIX)) {
    unsigned long time = checktime(0); // Функция рассчета затраченого времени
    System.SetState(STATE_PREFIX_WORKING);
    ReadCommand();
    CheckCommand();
    if ( !System.GetError(BadCommand) or true) {
      ExecuteCommand();
      SystemData();
    };
    checktime(time);  // Функция рассчета затраченого времени
  }
  else {
    System.SetState(STATE_PREFIX_IDLE_STANDING);
  };
};

void SystemRecord::ReadCommand() {
  Serial.readBytes(Command.Parameters.String, sizeof(CommandParametersData));
};

void SystemRecord::CheckCommand() {
  if(!Command.CheckCSum()) 
  {
    System.SetError(BadCommand);
  };
};

void SystemRecord::ExecuteCommand() {
  print ("ExecuteCommand start");
  switch (Command.Parameters.Item[CommandName]) {
  case COMMAND_PREFIX_MOVE:
    MovePrepareActions();
    SystemDataScan();
    PrintResult();
    PrintFullResult();
    break;
  case COMMAND_PREFIX_SCANNING:
    MovePrepareActions();
    SystemDataScan();
    PrintResult();
    break;
  case COMMAND_PREFIX_PROFILING:
    MovePrepareActions();
    SystemDataScan();
    PrintResult();
    break;
  case COMMAND_PREFIX_INDENTATION:
    MovePrepareActions();
    SystemDataScan();
    PrintResult();
    break;
  case COMMAND_PREFIX_CALIBRATION:
    MovePrepareActions();
    SystemDataScan();
    PrintResult();
    break;

  default:
    System.SetState(STATE_PREFIX_IDLE_STANDING);   
  };
  print ("ExecuteCommand end");
};

void SystemRecord::MovePrepareActions () {
  print("MovePrepareActions start");

  print ("CarriagesMove start");
  for (char i = 0; i < sizeof(Carriages)/sizeof(Carriages[0]); i++)
  {
    print ("Carriage", '[');
    Serial.print (i);
    Serial.print ("\n");
    Carriages[i].Move(&(Command.Parameters.Item[HeadX + 3 * i]));
  };
  print ("CarriagesMove end");
  Sensors.GetEnvironmentProperties();
  print ("MovePrepareActions end");
};

void SystemRecord::SystemDataScan() {
  print("SystemDataScan start");
  for (int i = 0; i < Sensors.GetScanResultLength(); i++)
  {
    print ("HeadScanMove start");
    Carriages[0].ScanMove(Z);
    print ("HeadScanMove end");

    Sensors.SetScanResult(i, SensorData(Carriages[0].GetPosition(Z), analogRead(PIN_SENSOR_TIP)));
    if ( Sensors.GetScanResultFx(i) > Command.Parameters.Item[Icrit] ) {
      Serial.print ("Overcurrent\n");
      break;
    };
  };
  print ("SystemDataScan end");
};

void SystemRecord::PointPricking(){
  print("PointPricking start");
  while (analogRead(PIN_SENSOR_Ptip) < Command.Parameters.Item[Pcrit])
  {
    Carriages[0].ScanMove(Z);
    // Установить проверку положения острия (Если максимально возможное - вывести ошибку)
  };
  print ("PointPricking end");
};

void SystemRecord::PrintResult() { // Вывод данных операции
  print ("PrintResult start");
  print ("-----------------------------------------------------------------------------------------------------",'[');
  System.SetState (STATE_PREFIX_OUTPUT_RESULT);

  Serial.print("\nScanState  : ");
  Serial.print(System.GetState());
  Serial.print("\nError      : ");
  Serial.print(System.GetErrorSymbol());
  Serial.print("\nCommand    : ");
  Serial.print(Command.Parameters.Item[CommandName]);
  Serial.print("\nParameters : ");
  for (int i = 0; i < sizeof(Command.Parameters.Item) / sizeof(Command.Parameters.Item[0]); i++)
  {
    Serial.print(Command.Parameters.Item[i]);
    Serial.print(" / ");
  };
  Serial.print("\nSensors    : ");
  for (int i = 0; i < COUNT_OF_SENSORS; i++)
  {
    Serial.print(Sensors.GetSensorsValueFx(i));
    Serial.print(" / ");
  };
  Serial.print("\nScanResult : ");
  for (int i = 0; i < Sensors.GetScanResultLength(); i++)
  {
    Serial.print(Sensors.GetScanResultFx(i));
    Serial.print(" / ");
  };
  print ("-----------------------------------------------------------------------------------------------------");
  print("PrintResult end");
};

void SystemRecord::PrintFullResult() {
  print ("PrintFullResult start");
  print ("-----------------------------------------------------------------------------------------------------");
  CommandParametersData *TempCommand;
  TempCommand = (CommandParametersData *)(& Command);
  for (unsigned int i = 0; i < sizeof(CommandParametersData); i++ )
  {
    Serial.write(TempCommand->String[i]);
  };
  delete TempCommand;
  SensorData *TempSensorData;
  for (unsigned int i = 0; i < Sensors.GetSensorsValueLength(); i++ )
  {
    TempSensorData = Sensors.GetSensorsValue(i);
    for (unsigned int j = 0; j < sizeof(SensorData); j++ )
    {
      Serial.write(TempSensorData->String[j]);
    };
  };
  for (unsigned int i = 0; i < Sensors.GetScanResultLength(); i++ )
  {
    TempSensorData = Sensors.GetScanResult(i);
    for (unsigned int j = 0; j < sizeof(SensorData); j++ )
    {
      Serial.write(TempSensorData->String[j]);
    };
    delete TempSensorData;
  };
  System.SetState (STATE_PREFIX_OUTPUT_RESULT);
  print ("-----------------------------------------------------------------------------------------------------");      
  print ("PrintFullResult end");
};

SystemRecord System = SystemRecord(); // Объявление Объекта для работы с данными

void setup() {
  Serial.begin(57600); // Устанавливаем последовательное соединение
}

void loop() {
  delay(1000); 
  System.Processing ();
}

unsigned long checktime(unsigned long starttime)
{
  unsigned long nowtime = micros(); 
  if (starttime)
  {
    Serial.print("\n=== Used time: "); 
    Serial.print( ( nowtime - starttime ) / 1000 ); 
    Serial.print(" msec "); 
    Serial.print("=====================================================================================\n"); 
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




