#define PIN_SENSOR_T A2 // Контакт для датчика температуры
#define PIN_SENSOR_HUMIDITY A3 // Контакт для датчика влажности
#define PIN_SENSOR_VIBRATION A4 // Контакт для датчика вибрации


class Sensors : 
protected SensorsRecord {
public:
  Sensors();
  void SensorsReset(); // Сброс данных сенсоров
  void GetEnvironmentProperties(); // Получание сенсорами параметров среды
  int GetSensorsData(char Num); // Получение данных сенсоров
}; 

void Sensors::SensorsReset() { // Сброс данных сенсоров
  for (char i = 0; i < sizeof(SensorsData) / sizeof(SensorsData[0]); i++)
  {
    SensorsData[i] = 0; 
  }; 
};

int Sensors::GetSensorsData(char Num)
{
  return SensorsData[Num];
};
