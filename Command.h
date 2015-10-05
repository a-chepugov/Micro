#define COMMAND_PREFIX "i" // Команда начала работы
#define COMMAND_PREFIX_MOVE 'MM' // Код команды операции для туннельного сканирования
#define COMMAND_PREFIX_SCANNING 'SS' // Код команды операции для туннельного сканирования
#define COMMAND_PREFIX_PROFILING 'RR' // Код команды операции снятия профиля (режим профилографа)
#define COMMAND_PREFIX_INDENTATION 'PP' // Код команды операции индентирования
#define COMMAND_PREFIX_CALIBRATION 'CC' // Код команды операции перекалибровки устройства
#define STATE_PREFIX_IDLE_STANDING 'Z'  // Сигнализация простоя
#define STATE_PREFIX_WORKING 'W'  // Сигнализация работы
#define STATE_PREFIX_OUTPUT_RESULT 'R' // Сигнализация выдачи результата
#define STATE_PREFIX_ERROR 'E'  // Сигнализация выдачи ошибки

enum CommandParametersNames // Перечень входящих параметров
{
  CommandName, HeadX, HeadY, HeadZ, TipX, TipY, TipZ, ScanSensor, U, Icrit, Pcrit, CSum, COUNT_OF_COMMAND_PARAMETERS  // COUNT_OF_COMMAND_PARAMETERS - Количество входящих параметров
};

union CommandParametersData // Структура команды
{
  Coordinate Item[COUNT_OF_COMMAND_PARAMETERS];
  char String[];
  CommandParametersData()
  {
     for(char i = 0; i < sizeof(*this); i++)
    {
      this->String[i] = 0;
    };    
  };  
};

class CommandItem {  
  public:
  CommandParametersData Parameters;  
  public:
  void SetCSum(); // Установка контрольной суммы
  bool CheckCSum(); // Проверка контрольной суммы
  void SetCommandParametersUnit(char ParameterNum, char TempUnit); // Установка параметра команды
  int GetCommandParameters(char ParameterNum); // Получение параметра команды
  CommandItem * GetCommandDataItem(CommandItem * CommandDataItem);
};

void CommandItem::SetCSum()
{
  int tempCSum = 0;
  for (char i = 0; i < CSum; i++)
  {
    tempCSum  += Parameters.Item[i];
  };
  Parameters.Item[CSum] = tempCSum ;
};

bool CommandItem::CheckCSum()
{
  int CSum = 0;
  for (char i = 0; i < CSum; i++)
  {
    CSum += Parameters.Item[i];
  };
  if (CSum == Parameters.Item[CSum])
  {
    return true;
  }
  else
  {
    return false;
  };
};

void CommandItem::SetCommandParametersUnit(char ParameterNum, char TempUnit)
{
  Parameters.String[ParameterNum] = TempUnit;
};

int CommandItem::GetCommandParameters(char ParameterNum)
{
  if (ParameterNum > sizeof(Parameters.Item) / sizeof(Parameters.Item[0]) )
  {
  }
  else return Parameters.Item[ParameterNum];
};

CommandItem * CommandItem::GetCommandDataItem(CommandItem * CommandDataItem)
{
  return this;
};

