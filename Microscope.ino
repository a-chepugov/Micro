// Скетч для работы микроскопа
// Версия 0.1.5
/* TODO
 Определить возможные ошибки в процессе сканирования
*/

#include <Stepper.h>
#include "CharToInt.h"
#include "SharedData.h"
#include "Head.h"
#include "Tip.h"
#include "Sensors.h"

void print(char * str, char stat = '@'); 
unsigned long checktime(unsigned long starttime = 0); 

void Head::HeadAxisMove(unsigned int *currient_coordinate, unsigned int new_coordinate, int StepperNum) { // Перемещение головки по одной координате
  print ("HeadAxisMove start"); 

  Serial.print ("Head "); 
  Serial.print (StepperNum); 
  Serial.print (" : "); 
  Serial.print (*currient_coordinate); 
  Serial.print (" -> "); 
  Serial.print (new_coordinate); 
  Serial.print (" = "); 
  Serial.print ( (int) (new_coordinate - *currient_coordinate) ); 

  HeadSteppers[StepperNum].step( (int) (new_coordinate - *currient_coordinate) );
  *currient_coordinate = new_coordinate; 
  print ("HeadAxisMove end"); 
};

//===================================================================================================

Tip::Tip() {
  pinMode(PIN_SENSOR_TIP, INPUT); // Установка режима чтения контактов сенсора острия
  pinMode(PIN_SENSOR_Ptip, INPUT); // Установка режима чтения контактов сенсора давления
  for (char i = 0; i < sizeof(TipPosition) / sizeof(TipPosition[0]); i++)
  {
    TipPosition[i] = 0; 
  }; 
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

PointData::PointData() : 
Head() , Tip() , Sensors() {
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
    if (!ErrorState[BadCommand]) {
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
  unsigned int CCsum = 0;
  for (char i = 0; i < sizeof(ScanParameters) / sizeof(ScanParameters[0]) - 1; i++)
  {
    CCsum += ScanParameters[i];
  };
  if(CCsum != ScanParameters[CCsum]) ErrorState[BadCommand] = true;
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
  HeadMove(&ScanParameters[0]); 
  print ("HeadMove end"); 

  TipMove(&ScanParameters[3]); 
  GetEnvironmentProperties(); 
  print ("MovePrepareActions end"); 
}; 

void PointData::PointDataScan() {
  print("PointDataScan start"); 
  for (int i = 0; i < sizeof(I_fx) / sizeof(I_fx[0]); i++)
  {
    print ("HeadZMove start"); 
    HeadZMove(); 
    print ("HeadZMove end"); 
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
    HeadZMove(); 
    // Установить проверку положения острия (Если максимально возможное - вывести ошибку)
  }; 
  print ("PointPricking end"); 
}; 

void PointData::PrintResult() { // Вывод данных операции
  print ("PrintResult start"); 
  print ("-----------------------------------------------------------------------------------------------------"); 
  SetState (STATE_PREFIX_OUTPUT_RESULT); 

  Serial.print("\nScanState : "); 
  Serial.print(State); 
  Serial.print("\nError : "); 
  PrintError(); 
  Serial.print("\nCommand : "); 
  Serial.print(Command); 
  Serial.print("\nScan Parameters : "); 
  for (int i = 0; i < sizeof(ScanParameters) / sizeof(ScanParameters[0]); i++)
  {
    Serial.print(ScanParameters[i]); 
    Serial.print(" / "); 
  }; 
  Serial.print("\nHead Position : "); 
  for (int i = 0; i < 3; i++)
  {
    GetHeadPosition(i); 
  };
  Serial.print("\nTip Position : "); 
  for (int i = 0; i < 3; i++)
  {
    GetTipPosition(i); 
  };
  Serial.print("\nSensors Data : "); 
  for (int i = 0; i < COUNT_OF_SENSORS; i++)
  {
    GetSensorsData(i);
  };

  Serial.print("\nI_fx : "); 
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

void PointData::SetError(char i) {
  ErrorState[i] = true; 
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


PointData Scan = PointData(); // Объявление Объекта для работы с данными

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

