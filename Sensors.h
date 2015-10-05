#define PIN_SENSOR_T A2 // Контакт для датчика температуры
#define PIN_SENSOR_HUMIDITY A3 // Контакт для датчика влажности
#define PIN_SENSOR_VIBRATION A4 // Контакт для датчика вибрации

enum ScanSensorsNames
{
  T, Humidity, Vibration, COUNT_OF_SENSORS
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
