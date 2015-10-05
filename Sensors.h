#define PIN_SENSOR_TIP A0 //  Контакт для датчика острия
#define PIN_SENSOR_Ptip A1 // Контакт для датчика давления укола
#define PIN_SENSOR_T A2 // Контакт для датчика температуры
#define PIN_SENSOR_HUMIDITY A3 // Контакт для датчика влажности
#define PIN_SENSOR_VIBRATION A4 // Контакт для датчика вибрации

enum ScanSensorsNames
{
  Itip, Ptip, T, Humidity, Vibration, COUNT_OF_SENSORS // Перечень сенсоров
};

union SensorData // Структура данных сканирования
{
  struct
  {
    Coordinate X; // Параметр сканирования
    unsigned short int Fx; // Показание датчика
  };
  char String[];

  SensorData(Coordinate TempX = 0, int TempFx = 0)
  {
    X = TempX;
    Fx = TempFx;
  }
};

class SensorsItem
{
private:
  SensorData SensorsValue[COUNT_OF_SENSORS]; // Данные датчиков
  SensorData ScanResult[64]; // Данные датчика сканирования
public:
  SensorsItem();
  inline void SetSensorsValue(char SensorNum, SensorData TempSensorData); // Получение параметров сканирования
  inline SensorData * GetSensorsValue(char SensorNum); // Получение данных сенсоров
  inline int GetSensorsValueFx(char SensorNum); // Получение данных сенсоров
  inline int GetSensorsValueLength(); // Получение длины массива данных сенсоров

  inline void SetScanResult(char ScanNum, SensorData TempSensorData); // Получание сенсорами параметров среды    
  inline SensorData * GetScanResult(char ScanNum); // Получение данных сенсоров
  inline int GetScanResultFx(char ScanNum); // Получение данных сенсоров
  inline int GetScanResultLength(); // Получение длины массива данных сенсоров

  inline void GetEnvironmentProperties();
}; 

void SensorsItem::SetSensorsValue(char SensorNum, SensorData TempSensorData)
{
  if(SensorNum < (sizeof(SensorsValue)/ sizeof(SensorsValue[0])) )
  {
    SensorsValue[SensorNum].X = TempSensorData.X;
    SensorsValue[SensorNum].Fx = TempSensorData.Fx;
  };
};

  inline SensorData * SensorsItem::GetSensorsValue(char SensorNum)
  {
    return &SensorsValue[SensorNum];
  };
  

int SensorsItem::GetSensorsValueFx(char SensorNum)
{
  if (SensorNum < GetSensorsValueLength())
  {
    return SensorsValue[SensorNum].Fx;
  }
  else
  {
    return -1;
  };
};

int SensorsItem::GetSensorsValueLength()
{
  return (sizeof(SensorsValue)/sizeof(SensorsValue[0]));
};

void SensorsItem::SetScanResult(char ScanNum, SensorData TempSensorData)
{
  if(ScanNum < (sizeof(ScanResult)/ sizeof(ScanResult[0])) )
  {
    ScanResult[ScanNum].X = TempSensorData.X;
    ScanResult[ScanNum].Fx = TempSensorData.Fx;
  };
};

  inline SensorData * SensorsItem::GetScanResult(char ScanNum)
  {
    return &ScanResult[ScanNum];
  };

int SensorsItem::GetScanResultFx(char ScanNum)
{
  if (ScanNum < GetScanResultLength() )
  {
    return ScanResult[ScanNum].Fx;
  }
  else
  {
    return -1;
  };
};

int SensorsItem::GetScanResultLength()
{
  return (sizeof(ScanResult) / sizeof(ScanResult[0]));
};








