#include <Stepper.h>
#include "SharedData.h"
#include "Carriage.h"
#include "Head.h"
#include "Tip.h"
#include "Sensors.h"

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

Sensors::Sensors() : 
SensorsData() {
  pinMode(PIN_SENSOR_TIP, INPUT); // Установка режима чтения контактов сенсора острия
  pinMode(PIN_SENSOR_Ptip, INPUT); // Установка режима чтения контактов сенсора давления
  pinMode(PIN_SENSOR_T, INPUT); // Установка режима чтения контактов сенсора температуры
  pinMode(PIN_SENSOR_HUMIDITY, INPUT); // Установка режима чтения контактов сенсора влажности
  pinMode(PIN_SENSOR_VIBRATION, INPUT); // Установка режима чтения контактов сенсора вибрации
};

void Sensors::GetEnvironmentProperties() {
  SensorsData();
  SensorsValue[T] = analogRead(PIN_SENSOR_T);
  SensorsValue[Humidity] = analogRead(PIN_SENSOR_HUMIDITY);
  SensorsValue[Vibration] = analogRead(PIN_SENSOR_VIBRATION);
};

//===================================================================================================

class SystemRecord : 
public Sensors , public SystemData, public OperationData {
private:
  Carriage Carriages[2]; // Массив суппортов
public:
  SystemRecord();
  void Processing(); // Ожидание команды
  void ReadCommand(); // Получение команды
  void CheckCommand(); // Проверка корректности комманды
  void ExecuteCommand(); // Выполнение полученной команды
  void MovePrepareActions(); // Перемещение в заданные координаты и снятие данных датчиков
  void SystemDataScan(); // Сканирование
  void PointPricking(); // Операция индентирование
  void PrintResult(); // Вывод данных операции
  void SetState(char state); // Установка статуса
  char GetState(); // Установка статуса  
  void SetError(char error);  // Установка статуса ошибки
  void PrintError();  // Вывод статуса ошибки
}; 

SystemRecord::SystemRecord() :
Sensors() , SystemData(), OperationData() {
  Carriages[0] = Carriage('S');
  Carriages[1] = Carriage('P');  
};

void SystemRecord::Processing() { // Ожидание команды
  Serial.print (GetState());
  if (Serial.find(COMMAND_PREFIX)) {
    unsigned long time = checktime(0); // Функция рассчета затраченого времени
    SetState(STATE_PREFIX_WORKING);
    ReadCommand();
    CheckCommand();
    if ( (!Errors[BadCommand]) or true) {
      ExecuteCommand();
      SystemData();
    };
    checktime(time);  // Функция рассчета затраченого времени
  }
  else {
    SetState(STATE_PREFIX_IDLE_STANDING);
  };
};

void SystemRecord::ReadCommand() {
  CommandName = Serial.read();
  for (char i = 0; i < sizeof(CommandParameters) / sizeof(CommandParameters[0]); i++)
  {
    char *temp = new char[sizeof(CommandParameters[i])];
    Serial.readBytes(temp, sizeof(CommandParameters[i]));
    SetCommandParameters(i, temp);// = CharToInt(temp);
    delete[] temp;
  };
};

void SystemRecord::CheckCommand() {
  if(!CheckCSum()) Errors[BadCommand] = true;
};

void SystemRecord::ExecuteCommand() { // Выполнение полученной команды
  print ("ExecuteCommand start");
  switch (CommandName) {
  case COMMAND_PREFIX_SCANNING:
    MovePrepareActions();
    SystemDataScan();
    PrintResult();
    break;
  case COMMAND_PREFIX_PRICKING:
    MovePrepareActions();
    // PointPricking();
    break;
  default:
    SetState(STATE_PREFIX_IDLE_STANDING);
  };
  print ("ExecuteCommand end");
};

void SystemRecord::MovePrepareActions () {
  print("MovePrepareActions start");

  print ("HeadMove start");
  Carriages[0].Move(&CommandParameters[0]);
  print ("HeadMove end");
  print ("TipMove start");
  Carriages[0].Move(&CommandParameters[3]);
  print ("TipMove end");
  GetEnvironmentProperties();
  print ("MovePrepareActions end");
};

void SystemRecord::SystemDataScan() {
  print("SystemDataScan start");
  for (int i = 0; i < sizeof(ScanResult) / sizeof(ScanResult[0]); i++)
  {
    print ("HeadScanMove start");
    Carriages[0].ScanMove(Z);
    print ("HeadScanMove end");
    ScanResult[i].ScanValue = analogRead(PIN_SENSOR_TIP);
    ScanResult[i].Point = Carriages[0].GetPosition(Z);

    if (! (ScanResult[i].ScanValue < CommandParameters[Icrit])) {
      Serial.print ("Overcurrent\n");
      break;
    };
  };
  print ("SystemDataScan end");
};

void SystemRecord::PointPricking(){ // Операция индентирование
  print("PointPricking start");
  while (analogRead(PIN_SENSOR_Ptip) < CommandParameters[Pcrit])
  {
    Carriages[0].ScanMove(Z);
    // Установить проверку положения острия (Если максимально возможное - вывести ошибку)
  };
  print ("PointPricking end");
};

void SystemRecord::PrintResult() { // Вывод данных операции
  print ("PrintResult start");
  print ("-----------------------------------------------------------------------------------------------------",'[');
  SetState (STATE_PREFIX_OUTPUT_RESULT);

  Serial.print("\nScanState  : ");
  Serial.print(State);
  Serial.print("\nError      : ");
  PrintError();
  Serial.print("\nCommand    : ");
  Serial.print(CommandName);
  Serial.print("\nParameters : ");
  for (int i = 0; i < sizeof(CommandParameters) / sizeof(CommandParameters[0]); i++)
  {
    Serial.print(CommandParameters[i]);
    Serial.print(" / ");
  };
  Serial.print("\nHead Pos   : ");
  for (int i = 0; i < 3; i++)
  {
    Serial.print(Carriages[0].GetPosition(i));
    Serial.print(" / ");
  };
  Serial.print("\nSensors    : ");
  for (int i = 0; i < COUNT_OF_SENSORS - 2; i++)
  {
    Serial.print(GetSensorsData(i));
    Serial.print(" / ");
  };

  Serial.print("\nScanResult : ");
  for (int i = 0; i < sizeof(ScanResult) / sizeof(ScanResult[0]); i++)
  {
    Serial.print(ScanResult[i].ScanValue);
    Serial.print(" / ");
  };
  print ("-----------------------------------------------------------------------------------------------------");
  print("PrintResult end");
};

void SystemRecord::SetState(char State) {
  State = State;
};

char SystemRecord::GetState() {
  return State;
};

void SystemRecord::SetError(char Error) {
  if (Error > sizeof(ErrorNames) / sizeof(ErrorNames[0]) )
  {
    print("ErrorNum not in Error Base");
  }
  else
  {
    Errors[Error] = true;
  }
};

void SystemRecord::PrintError() {
  char ErrorSymbol = 0;
  for (char i = 0; i < sizeof(ErrorNames) / sizeof(ErrorNames[0]); i++)
  {
    ErrorSymbol = ErrorSymbol + Errors[i]^2;
  };
  if (ErrorSymbol) {
    Serial.print(ErrorSymbol);
  }
  else Serial.print("OK");
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






