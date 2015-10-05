#define STATE_PREFIX_IDLE_STANDING '3Z'  // Сигнализация простоя
#define STATE_PREFIX_WORKING 'wW'  // Сигнализация работы
#define STATE_PREFIX_OUTPUT_RESULT 'rR' // Сигнализация выдачи результата
#define STATE_PREFIX_ERROR 'eE'  // Сигнализация выдачи ошибки

#define SERIAL_SPEED 57600
#define EMPTY_SYNC_LENGTH 100

enum ErrorNames
{
  SerialOutputError, BadCommand, PositioningErrorHead, PositioningErrorTip, NoHeadDrives, NoTipDrives, NoSensors, COUNT_OF_ERRORS // Перечень ошибок
};

enum SystemPropertiesNames
{
  COUNT_OF_SYSTEM_PROPERTIES
};

union SystemData // Структура координаты
{
  struct
  {
    short int State; // Состояние системы
    DataArray < bool, COUNT_OF_ERRORS > Errors; // Статус ошибок системы
    DataArray < char, COUNT_OF_SYSTEM_PROPERTIES > Properties; // Свойства системы
  };
  char Chars[];

  SystemData();
  char & operator[](char Item);
  inline char CharsLength(); // Размер Данных команды
};

SystemData::SystemData()
{
  for(char i = 0; i < sizeof(*this); i++)
  {
    this->Chars[i] = 0;
  };  
  State = STATE_PREFIX_IDLE_STANDING;
};


char & SystemData::operator[](char Item)
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

char SystemData::CharsLength()
{
  return sizeof(*this) / sizeof(Chars[0]);
};

class SystemItem { // Структура состояния системы
private:
  SystemData Values;
public:
  inline void SetState(char state); // Установка статуса
  inline char GetState(); // Получение статуса
  inline void SetError(char ErrorNum);  // Установка статуса ошибки
  inline bool GetError(char ErrorNum); // Получение статуса ошибки
  inline char GetErrorSymbol(); // Получение статуса ошибки  
  inline void SetProrerties();
  inline char GetProrerties();
};

void SystemItem::SetState(char NewState) {
  Values.State = NewState;
};

char SystemItem::GetState() {
  return Values.State;
};

void SystemItem::SetError(char ErrorNum) {
    Values.Errors[ErrorNum] = true;
};

bool SystemItem::GetError(char ErrorNum) {
    return Values.Errors[ErrorNum];
};

char SystemItem::GetErrorSymbol()
{
  char ErrorSymbol = 0;
  for (char i = 0; i < COUNT_OF_ERRORS; i++)
  {
    ErrorSymbol = ErrorSymbol + Values.Errors[i] * (i^2);
  };
  ErrorSymbol = ErrorSymbol + '!';
  return ErrorSymbol;
};



