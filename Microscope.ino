// Скетч для работы микроскопа
/* TODO
 Определить возможные ошибки в процессе сканирования
 */

#include <Stepper.h>
#include "CharToInt.h"
#include "SharedData.h"
#include "Scanner.h"
#include "Head.h"
#include "Tip.h"
#include "Sensors.h"

void print(char * str, char stat = '@'); 
unsigned long checktime(unsigned long starttime = 0); 

void Scanner::AxisMove(int *currient_coordinate, int new_coordinate, int AxisNum) { // Перемещение головки по одной координате
  print ("AxisMove start");

  Serial.print ("Axis ");
  Serial.print (AxisNum);
  Serial.print (" : ");
  Serial.print (*currient_coordinate);
  Serial.print (" -> ");
  Serial.print (new_coordinate);
  Serial.print (" = ");
  Serial.print ( (int) (new_coordinate - *currient_coordinate) );
  HeadSteppers[AxisNum].step( (int) (new_coordinate - *currient_coordinate) );
  *currient_coordinate = new_coordinate;
  print ("AxisMove end");

};

//===================================================================================================

TipData::TipData() : 
Scanner() {
  pinMode(PIN_SENSOR_TIP, INPUT); // Установка режима чтения контактов сенсора острия
  pinMode(PIN_SENSOR_Ptip, INPUT); // Установка режима чтения контактов сенсора давления
};

//===================================================================================================

Sensors::Sensors() {
  pinMode(PIN_SENSOR_T, INPUT); // Установка режима чтения контактов сенсора температуры
  pinMode(PIN_SENSOR_HUMIDITY, INPUT); // Установка режима чтения контактов сенсора влажности
  pinMode(PIN_SENSOR_VIBRATION, INPUT); // Установка режима чтения контактов сенсора вибрации
  SensorsReset();
};

void Sensors::GetEnvironmentProperties() {
  SensorsData[T] = analogRead(PIN_SENSOR_T);
  SensorsData[Humidity] = analogRead(PIN_SENSOR_HUMIDITY);
  SensorsData[Vibration] = analogRead(PIN_SENSOR_VIBRATION);
};

//===================================================================================================

class PointData : 
public Sensors , private PointRecord {
public:
  HeadData Head;
  TipData Tip;

  PointData();
  void PointDataReset(); // Сброс данных точки
  void Processing(); // Ожидание команды
  void ReadCommand(); // Получение команды
  void CheckCommand(); // Проверка корректности комманды
  void ExecuteCommand(); // Выполнение полученной команды
  void MovePrepareActions(); // Перемещение в заданные координаты и снятие данных датчиков
  void PointDataScan(); // Сканирование
  void PointPricking(); // Операция индентирование
  void PrintResult(); // Вывод данных операции
  void SetState(char state); // Установка статуса
  void SetError(char error);  // Установка статуса ошибки
  void PrintError();  // Вывод статуса ошибки
}; 

PointData Scan = PointData(); // Объявление Объекта для работы с данными

PointData::PointData() :
Sensors() {
  PointDataReset();
};

void PointData::PointDataReset() { // Сброс данных точки
  State = 'i';
  for (char i = 0; i < sizeof(ErrorState) / sizeof(ErrorState[0]); i++)
  {
    ErrorState[i] = false;
  };
  Command = '\0';
  for (int i = 0; i < sizeof(ScanParameters) / sizeof(ScanParameters[0]); i++)
  {
    ScanParameters[i] = 0;
  };
  for (int i = 0; i < sizeof(I_fx) / sizeof(I_fx[0]); i++)
  {
    I_fx[i] = -1;
  };
  SensorsReset();
};

void PointData::Processing() { // Ожидание команды
  Serial.print ("Z");
  if (Serial.find(COMMAND_PREFIX)) {
    unsigned long time = checktime(0); // Функция рассчета затраченого времени
    SetState(STATE_PREFIX_WORKING);
    ReadCommand();
    CheckCommand();
    if ( (!ErrorState[BadCommand]) or true) {
      ExecuteCommand();
      PointDataReset();
    };
    checktime(time);  // Функция рассчета затраченого времени
  }
  else {
    SetState(STATE_PREFIX_IDLE_STANDING);
  };
};

void PointData::ReadCommand() {
  Command = Serial.read();
  for (char i = 0; i < sizeof(ScanParameters) / sizeof(ScanParameters[0]); i++)
  {
    char *temp = new char[sizeof(ScanParameters[i])];
    Serial.readBytes(temp, sizeof(ScanParameters[i]));
    ScanParameters[i] = CharToInt(temp);
    delete[] temp;
  };
};

void PointData::CheckCommand() {
  if(GetCCSum(ScanParameters) != ScanParameters[CCsum]) ErrorState[BadCommand] = true;
};

void PointData::ExecuteCommand() { // Выполнение полученной команды
  print ("ExecuteCommand start");
  switch (Command) {
  case COMMAND_PREFIX_SCANNING:
    MovePrepareActions();
    PointDataScan();
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

void PointData::MovePrepareActions () {
  print("MovePrepareActions start");

  print ("HeadMove start");
  Head.Move(&ScanParameters[0]);
  print ("HeadMove end");
  print ("TipMove start");
  Tip.Move(&ScanParameters[3]);
  print ("TipMove end");
  GetEnvironmentProperties();
  print ("MovePrepareActions end");
};

void PointData::PointDataScan() {
  print("PointDataScan start");
  for (int i = 0; i < sizeof(I_fx) / sizeof(I_fx[0]); i++)
  {
    print ("HeadScanMove start");
    Head.ScanMove();
    print ("HeadScanMove end");
    I_fx[i] = analogRead(PIN_SENSOR_TIP);
    if (! (I_fx[i] < ScanParameters[Icrit])) {
      Serial.print ("Overcurrent\n");
      break;
    };
  };
  print ("PointDataScan end");
};

void PointData::PointPricking(){ // Операция индентирование
  print("PointPricking start");
  while (analogRead(PIN_SENSOR_Ptip) < ScanParameters[Pcrit])
  {
    Head.ScanMove();
    // Установить проверку положения острия (Если максимально возможное - вывести ошибку)
  };
  print ("PointPricking end");
};

void PointData::PrintResult() { // Вывод данных операции
  print ("PrintResult start");
  print ("-----------------------------------------------------------------------------------------------------",'[');
  SetState (STATE_PREFIX_OUTPUT_RESULT);

  Serial.print("\nScanState  : ");
  Serial.print(State);
  Serial.print("\nError      : ");
  PrintError();
  Serial.print("\nCommand    : ");
  Serial.print(Command);
  Serial.print("\nParameters : ");
  for (int i = 0; i < sizeof(ScanParameters) / sizeof(ScanParameters[0]); i++)
  {
    Serial.print(ScanParameters[i]);
    Serial.print(" / ");
  };
  Serial.print("\nHead Pos   : ");
  for (int i = 0; i < 3; i++)
  {
    Serial.print(Head.GetPosition(i));
      Serial.print(" / ");
  };
  Serial.print("\nTip  Pos   : ");
  for (int i = 0; i < 3; i++)
  {
    Serial.print(Tip.GetPosition(i));
    Serial.print(" / ");
  };
  Serial.print("\nSensors    : ");
  for (int i = 0; i < COUNT_OF_SENSORS; i++)
  {
    Serial.print(GetSensorsData(i));
    Serial.print(" / ");
  };

  Serial.print("\nI_fx       : ");
  for (int i = 0; i < sizeof(I_fx) / sizeof(I_fx[0]); i++)
  {
    Serial.print(I_fx[i]);
    Serial.print(" / ");
  };
  print ("-----------------------------------------------------------------------------------------------------");
  print("PrintResult end");
};

void PointData::SetState(char state) {
  State = state;
};

void PointData::SetError(char error) {
  if (error > sizeof(ErrorState) / sizeof(ErrorState[0]) )
  {
  }
  else
  {
  ErrorState[error] = true;
  }
};

void PointData::PrintError() {
  char ErrorSymbol = 0;
  for (char i = 0; i < sizeof(ErrorState) / sizeof(ErrorState[0]); i++)
  {
    ErrorSymbol = ErrorSymbol + ErrorState[i]^2;
  };
  if (ErrorSymbol) {
    Serial.print(ErrorSymbol);
  }
  else Serial.print("OK");
};


void setup() {
  Serial.begin(57600); // Устанавливаем последовательное соединение
}

void loop() {
  delay(1000); 
  Scan.Processing (); 
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


