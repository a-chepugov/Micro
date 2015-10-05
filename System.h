#define STATE_PREFIX_IDLE_STANDING 'Z'  // Сигнализация простоя
#define STATE_PREFIX_WORKING 'W'  // Сигнализация работы
#define STATE_PREFIX_OUTPUT_RESULT 'R' // Сигнализация выдачи результата
#define STATE_PREFIX_ERROR 'E'  // Сигнализация выдачи ошибки

enum ErrorNames
{
  SerialOutputError, BadCommand, PositioningErrorHead, PositioningErrorTip, NoHeadDrives, NoTipDrives, NoSensors, COUNT_OF_ERRORS // Перечень ошибок
};

union SystemData // Структура координаты
{
  struct
  {
    char State; // Состояние системы
    bool Errors[COUNT_OF_ERRORS]; // Статус ошибок системы
    char Properties[]; // Свойства системы
  };
  char String[];

  SystemData()
  {
    for(char i = 0; i < sizeof(*this); i++)
    {
      this->String[i] = STATE_PREFIX_IDLE_STANDING;
    };  
  }
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

void SystemItem::SetState(char State) {
  State = State;
};

char SystemItem::GetState() {
  return Values.State;
};

void SystemItem::SetError(char ErrorNum) {
  if (ErrorNum > COUNT_OF_ERRORS )
  {
  }
  else
  {
    Values.Errors[ErrorNum] = true;
  }
};

bool SystemItem::GetError(char ErrorNum) {
  if (ErrorNum < COUNT_OF_ERRORS )
  {
    return Values.Errors[ErrorNum];
  };
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




