#define PIN_SENSOR_TIP A0 //  Контакт для датчика острия
#define PIN_SENSOR_Ptip A1 // Контакт для датчика давления укола
#define PIN_SENSOR_T A2 // Контакт для датчика температуры
#define PIN_SENSOR_HUMIDITY A3 // Контакт для датчика влажности
#define PIN_SENSOR_VIBRATION A4 // Контакт для датчика вибрации

class Sensors : 
protected SensorsData {
public:
  Sensors();
  void GetEnvironmentProperties(); // Получание сенсорами параметров среды
  int GetSensorsData(char Num); // Получение данных сенсоров
}; 

int Sensors::GetSensorsData(char SensorNum)
{
  if (SensorNum > sizeof(SensorsValue) / sizeof(SensorsValue[0]) )
  {
    return -1;
  }
  return SensorsValue[SensorNum];
 
};

