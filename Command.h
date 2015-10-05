#ifndef Command_h
#define Command_h

#define COMMAND_PREFIX "i" // Команда начала работы
#define COMMAND_PREFIX_MOVE 'MM' // Код команды операции для туннельного сканирования
#define COMMAND_PREFIX_SCANNING 'TT' // Код команды операции для туннельного сканирования
#define COMMAND_PREFIX_PROFILING 'RR' // Код команды операции снятия профиля (режим профилографа)
#define COMMAND_PREFIX_INDENTATION 'PP' // Код команды операции индентирования
#define COMMAND_PREFIX_CALIBRATION 'CC' // Код команды операции перекалибровки устройства

void abort(int i);

enum CommandParametersNames // Перечень входящих параметров
{
  CommandName, HeadX, HeadY, HeadZ, TipX, TipY, TipZ, U, Istart, Icrit, Pstart, Pcrit, ScanDeep, CSum, COUNT_OF_COMMAND_PARAMETERS  // COUNT_OF_COMMAND_PARAMETERS - Количество входящих параметров
};

union CommandData  // Структура команды
{
  DataArray < Coordinate, COUNT_OF_COMMAND_PARAMETERS > Parameters; // Параметры команды

private:
  char Chars[];
public:
  CommandData();
  inline void SetCSum(); // Установка контрольной суммы
  inline bool CheckCSum(); // Проверка контрольной суммы
  inline char DataToChar(unsigned char i); // Данные в формате char
  inline char * fromChar(); // Указатель на структуру команды в формате char
  inline unsigned char Size(); // Размер структуры данных команды
};

CommandData::CommandData()
{
  for(char i = 0; i < sizeof(*this); i++)
  {
    this->Chars[i] = '!';
  };    
};

void CommandData::SetCSum()
{
  int tempCSum = 0;
  for (char i = 0; i < CSum; i++)
  {
    tempCSum  += Parameters[i];
  };
  Parameters[CSum] = tempCSum ;
};

bool CommandData::CheckCSum()
{
  int TempCSum = 0;
  for (char i = 0; i < CSum; i++)
  {
    TempCSum += Parameters[i];
  };
  if (TempCSum == Parameters[CSum])
  {
    return true;
  }
  else
  {
    return false;
  };
};

char CommandData::DataToChar(unsigned char i)
{
  return Chars[i];
};

char * CommandData::fromChar()
{
  return Chars;
};

unsigned char CommandData::Size()
{
  return sizeof *this;
};


#endif

