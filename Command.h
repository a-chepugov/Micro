#ifndef Command_h
#define Command_h
//==============================================================================

#include "SharedData.h"

#define COMMAND_PREFIX "i" // Команда начала работы
#define COMMAND_PREFIX_MOVE 'MM' // Код команды операции для туннельного сканирования
#define COMMAND_PREFIX_SCANNING 'TT' // Код команды операции для туннельного сканирования
#define COMMAND_PREFIX_PROFILING 'RR' // Код команды операции снятия профиля (режим профилографа)
#define COMMAND_PREFIX_INDENTATION 'PP' // Код команды операции индентирования
#define COMMAND_PREFIX_CALIBRATION 'CC' // Код команды операции перекалибровки устройства

enum CommandParametersNames // Перечень входящих параметров
{
  CommandName, HeadX, HeadY, HeadZ, TipX, TipY, TipZ, U, Istart, Icrit, Ptipstart, Ptipcrit, Patmstart, Patmcrit, Vibrationstart, Vibrationcrit, ScanDeep, CSum, COUNT_OF_COMMAND_PARAMETERS  // COUNT_OF_COMMAND_PARAMETERS - Количество входящих параметров
};

class CommandData
{
  DataArray < Coordinate, COUNT_OF_COMMAND_PARAMETERS > Items; // Параметры команды
public:
  inline unsigned int Length(); // Получение размера команды
  inline void SetCSum(); // Установка контрольной суммы
  inline bool CheckCSum(); // Проверка контрольной суммы
  inline Coordinate & Item(unsigned char ItemNum); // Получение доступа к элементу
};

unsigned int CommandData::Length()
{
  return Items.Length();
};

void CommandData::SetCSum()
{
  int tempCSum = 0;
  for (char i = 0; i < CSum; i++)
  {
    tempCSum  += Items[i];
  };
  Items[CSum] = tempCSum ;
};

bool CommandData::CheckCSum()
{
  int TempCSum = 0;
  for (char i = 0; i < CSum; i++)
  {
    TempCSum += Items[i];
  };
  if (TempCSum == Items[CSum])
  {
    return true;
  }
  else
  {
    return false;
  };
};

Coordinate & CommandData::Item(unsigned char ItemNum)
{
  return Items[ItemNum];
};

union CommandData_C // Структура команды
{
  CommandData Data;
  DataArray < char, sizeof(CommandData) > Chars;

  inline CommandData_C(char NewChar);
};

CommandData_C::CommandData_C(char NewChar = '!')
{
  for(char i = 0; i < Chars.Length(); i++)
  {
    this->Chars[i] = NewChar;
  };    
};

//==============================================================================
#endif






