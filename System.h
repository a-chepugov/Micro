#ifndef System_h
#define System_h
//==============================================================================

#define STATE_PREFIX_IDLE_STANDING 'Z'  // Сигнализация простоя
#define STATE_PREFIX_WORKING 'W'  // Сигнализация работы
#define STATE_PREFIX_OUTPUT_RESULT 'R' // Сигнализация выдачи результата
#define STATE_PREFIX_ERROR 'E'  // Сигнализация выдачи ошибки

#define SERIAL_SPEED 57600
#define EMPTY_SYNC_LENGTH 100

enum ErrorNames
{
  SerialOutputError, BadCommand, PositioningErrorHead, PositioningErrorTip, NoHeadDrives, NoTipDrives, NoSensors, COUNT_OF_ERRORS // Перечень ошибок
};

enum SystemPropertiesNames
{
  HeadXMax, HeadYMax, HeadZMax, TipXMax, TipYMax, TipZXMax, COUNT_OF_SYSTEM_PROPERTIES
};

class SystemData
{
  char State_; // Состояние системы
  DataArray < char, COUNT_OF_SYSTEM_PROPERTIES > Properties_; // Свойства системы
  DataArray < bool, COUNT_OF_ERRORS > Errors_; // Статус ошибок системы
public:
  inline char & State(); // Состояние системы
  inline char & Properties(unsigned char); // Состояние системы
  inline bool & Errors(unsigned char); // Состояние системы
  inline char GetErrorSymbol(); // Получение статуса ошибки
};

char & SystemData::State()
{
  return State_;
};

char & SystemData::Properties(unsigned char i)
{
  return Properties_[i];
};

bool & SystemData::Errors(unsigned char i)
{
  return Errors_[i];
};

char SystemData::GetErrorSymbol()
{
  char ErrorSymbol = 0;
  for (char i = 0; i < COUNT_OF_ERRORS; i++)
  {
    ErrorSymbol = ErrorSymbol + Errors_[i] * (i^2);
  };
  ErrorSymbol = ErrorSymbol + '!';
  return ErrorSymbol;
};

union SystemData_C // Структура данных о системе
{
  SystemData Data;
  DataArray < char, sizeof(SystemData)> Chars;

  inline SystemData_C(char NewChar);
};

SystemData_C::SystemData_C(char NewChar = '!')
{
  for(char i = 0; i < Chars.Length(); i++)
  {
    this->Chars[i] = NewChar;
  };    
  Data.State() = STATE_PREFIX_IDLE_STANDING;
};

//==============================================================================
#endif







