#ifndef System_h
#define System_h

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

union SystemData // Структура данных о системе
{
private:  
  struct
  {
    char State_; // Состояние системы
    DataArray < bool, COUNT_OF_ERRORS > Errors_; // Статус ошибок системы
    DataArray < char, COUNT_OF_SYSTEM_PROPERTIES > Properties_; // Свойства системы
  };
  char Chars[];
public:
  SystemData();
  inline char GetErrorSymbol(); // Получение статуса ошибки  
  inline char & State(); // Состояние системы
  inline bool & Errors(unsigned char); // Состояние системы  
  inline char & Properties(unsigned char); // Состояние системы
  inline char DataChar(unsigned char); // Данные в формате char
  inline unsigned char Size(); // Размер структуры данных о системе
};

SystemData::SystemData()
{
  for(char i = 0; i < sizeof(*this); i++)
  {
    this->Chars[i] = 0;
  };  
  State_ = STATE_PREFIX_IDLE_STANDING;
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

char & SystemData::State()
{
  return State_;
};

bool & SystemData::Errors(unsigned char i)
{
  return Errors_[i];
};

char & SystemData::Properties(unsigned char i)
{
  return Properties_[i];
};

unsigned char SystemData::Size()
{
  return sizeof *this;
};

char SystemData::DataChar(unsigned char i)
{
  if (i >= 0 && i < this->Size() )
  {
    return Chars[i];
  }
  else
  {
    ::abort();
  };
};


#endif





