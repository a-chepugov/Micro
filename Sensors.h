#ifndef Sensors_h
#define Sensors_h

#define PIN_SENSOR_TIP A0 //  Контакт для датчика острия
#define PIN_SENSOR_Ptip A1 // Контакт для датчика давления укола
#define PIN_SENSOR_T A2 // Контакт для датчика температуры
#define PIN_SENSOR_HUMIDITY A3 // Контакт для датчика влажности
#define PIN_SENSOR_VIBRATION A4 // Контакт для датчика вибрации
#define SCANNING_DEEP 100 // Количество точек сканирования
#define COUNT_OF_SCAN_SENSORS 1 // Сканирующие сенсоры

enum ScanSensorsNames
{
  Itip, Ptip, T, Humidity, Vibration, COUNT_OF_SENSORS // Перечень сенсоров
};

class SensorData1  // Структура данных сенсора
{
short int Data; // Показание датчика
public:  
  SensorData1(short int NewData = 0)
  {
    Data = NewData;
  };
};

class SensorsData
{
    DataArray < SensorData1, COUNT_OF_SENSORS > SensorsValue; // Данные датчиков
public:  
  SensorsData()
  {
    for(unsigned char i = 0; i < SensorsValue.Length(); i++)
    {
    };
//    Data = NewData;
  };
};

class ScanPointData
{
  Coordinate X; // Параметр сканирования
  DataArray < SensorData1, COUNT_OF_SCAN_SENSORS > ScanersValue; // Данные датчиков
  
  ScanPointData(Coordinate TempX, SensorData1 TempSensorData1)
  {
    
  };
};

class ScanData 
{
   DataArray < ScanPointData, SCANNING_DEEP > ScanValue; // Данные сканирования
};




union SensorData // Структура данных сканирования
{
  struct
  {
    Coordinate X; // Параметр сканирования
    unsigned short int Fx; // Показание датчика
    bool Direction; // Знак сканирования
  };
  char Chars[];

  SensorData(Coordinate TempX, int TempFx);
  char & operator[](char Item);
  char CharsLength();
};

SensorData::SensorData(Coordinate TempX = 0, int TempFx = 0)
{
  X = TempX;
  Fx = TempFx;
  Direction = 0;
};

char & SensorData::operator[](char Item)
{
  if (Item >= 0 && Item < CharsLength() )
  {
    return Chars[Item];
  }
  else
  {
    ::abort(0);
  };
};

char SensorData::CharsLength()
{
  return sizeof(*this) / sizeof(Chars[0]);
};

struct SensorsSystemData
{
public:
  DataArray < SensorData, COUNT_OF_SENSORS > SensorsValue; // Данные датчиков
  DataArray < SensorData, SCANNING_DEEP > ScanResult; // Данные датчиков
public:
  //!!!!!!!!  DataArray < char, sizeof(SensorData) * (COUNT_OF_SENSORS + SCANNING_DEEP)> Chars2;
  SensorsSystemData();
  void SensorsDataReset(); // Сброс данных датчиков
  inline void GetSensorsValue(); // Получение данных датчиков  
  inline void SetScanResult(SensorData TempSensorData); // Получение параметров сканирования

}; 

void SensorsSystemData::SensorsDataReset()
{
  for (char i = 0; i < SensorsValue.Length(); i++)
  { 
    SensorsValue[i] = SensorData();
  };
  for (char i = 0; i < ScanResult.Length(); i++)
  { 
    ScanResult[i] = SensorData();
  };
};

void SensorsSystemData::SetScanResult(SensorData TempSensorData)
{
  for (char i = ScanResult.Length() - 1; i > 0; i--)
  { 
    ScanResult[i] = ScanResult[i - 1];
  };
  ScanResult[0] = SensorData(TempSensorData.X, TempSensorData.Fx);
};

#endif


