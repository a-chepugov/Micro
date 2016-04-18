#ifndef Sensors_h
#define Sensors_h
//==============================================================================

#define PIN_SENSOR_Itip A0 //  Контакт для датчика острия
#define PIN_SENSOR_Ptip A1 // Контакт для датчика давления укола
#define PIN_SENSOR_Tatm A2 // Контакт для датчика температуры
#define PIN_SENSOR_Patm A3 // Контакт для датчика давления окружающей среды
#define PIN_SENSOR_HUMIDITY A4 // Контакт для датчика влажности
#define PIN_SENSOR_VIBRATION A5 // Контакт для датчика вибрации
#define SCANPOINTS_NUM 100 // Количество точек сканирования

enum ScanSensorsNames
{
  Itip, Ptip, COUNT_OF_SCAN_SENSORS, // Перечень сенсоров для сканирования
  Tatm = COUNT_OF_SCAN_SENSORS, Patm, Humidity, Vibration, COUNT_OF_SENSORS, COUNT_OF_ENV_SENSORS = COUNT_OF_SENSORS - COUNT_OF_SCAN_SENSORS // Перечень сенсоров среды
};

const char SensorsPins[COUNT_OF_SENSORS] = {
  PIN_SENSOR_Itip, PIN_SENSOR_Ptip, PIN_SENSOR_Tatm, PIN_SENSOR_Patm, PIN_SENSOR_HUMIDITY, PIN_SENSOR_VIBRATION
};

//==============================================================================

typedef unsigned short int SensorData;

//==============================================================================

class Sensor
{
  unsigned char Pin;
public:
  Sensor(unsigned char PinNum);
  SensorData Value();
};

Sensor::Sensor(unsigned char PinNum = 0)
{
  Pin = PinNum;
  pinMode(Pin, INPUT); // Установка режима чтения контактов сенсора острия
};

SensorData Sensor::Value()
{
  return analogRead(Pin);
};

class ScanSensor : 
public Sensor
{
  SensorData ScanStart;
  SensorData ScanStop;
public:
  ScanSensor(unsigned char PinNum, SensorData NewScanStart, SensorData NewScanStop);
};

ScanSensor::ScanSensor(unsigned char PinNum, SensorData NewScanStart, SensorData NewScanStop) : 
Sensor(PinNum)
{
  ScanStart = NewScanStart;
  ScanStop = NewScanStop;
};


class SensorsSystem
{
  DataArray < Sensor * , COUNT_OF_SCAN_SENSORS > Sensors;
public:
  SensorsSystem()
  {
    Sensors[Itip] = new ScanSensor(11,1,1);
    Sensors[Ptip] = new ScanSensor(0,0,0);
    Sensors[Tatm] = new Sensor(0);
    Sensors[Patm] = new Sensor(0);
    Sensors[Humidity] = new Sensor(0);
    Sensors[Vibration] = new Sensor(0);
  };
};




SensorsSystem SensorsSystem1;


class EnvironmentData
{
  DataArray < SensorData, COUNT_OF_SENSORS > SensorsValue; // Данные датчиков

public:
  void GetEnvironmentData();
  SensorData & Item(unsigned short int ItemNum);
  unsigned int Length();
};

unsigned int EnvironmentData::Length()
{
  return SensorsValue.Length();
};

SensorData & EnvironmentData::Item(unsigned short int ItemNum)
{
  return SensorsValue[ItemNum];
};

union EnvironmentData_C
{
  EnvironmentData Data;
  DataArray < char, sizeof(EnvironmentData) > Chars;

  EnvironmentData_C(char NewChar);   
};

EnvironmentData_C::EnvironmentData_C(char NewChar = '!')
{
  for(char i = 0; i < Chars.Length(); i++)
  {
    this->Chars[i] = NewChar;
  };    
};

//==============================================================================

struct ScanPointData
{
  Coordinate X; // Параметр сканирования
  SensorData SensorValue; // Данные датчиков
};

class ScanData 
{
  DataArray < ScanPointData, SCANPOINTS_NUM > ScanValue; // Данные сканирования

public:
  void PushNewScanResult(Coordinate TempX, SensorData TempSensorData);
  unsigned int Length();
  ScanPointData & Item(unsigned short int ItemNum);
};

unsigned int ScanData::Length()
{
  return ScanValue.Length();
};

ScanPointData & ScanData::Item(unsigned short int ItemNum)
{
  return ScanValue[ItemNum];
};

void ScanData::PushNewScanResult(Coordinate TempX, SensorData TempSensorData)
{
  for (char i = ScanValue.Length() - 1; i > 0; i--)
  { 
    ScanValue[i] = ScanValue[i - 1];
  };
  ScanValue[0].X = TempX;
  ScanValue[0].SensorValue = TempSensorData;

};

union ScanData_C // Структура данных сканирования
{
  ScanData Data;
  DataArray < char, sizeof(ScanData) > Chars;

  ScanData_C(char NewChar);   
};

ScanData_C::ScanData_C(char NewChar = 0)
{
  for(char i = 0; i < Chars.Length(); i++)
  {
    this->Chars[i] = NewChar;
  };
};

//==============================================================================
class SensorsData
{
public:
  EnvironmentData_C Environment; // Данные датчиков
  ScanData_C Scan; // Результат сканирования

  SensorsData ();
  void SensorsDataReset();
};

void SensorsData ::SensorsDataReset()
{
  Environment = EnvironmentData_C();
  Scan = ScanData_C();
};

//==============================================================================
#endif















