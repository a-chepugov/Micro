#ifndef Carriage_h
#define Carriage_h
//==============================================================================

#include "Coordinates.h"

#define SAFE_HEIGHT 8000

class Carriage {
protected:
  DecartCoordinates Position;
public:
  inline Carriage(DecartCoordinates NewDecartCoordinates);

  virtual void MoversInitialization() = 0; // Инициализация системы перемещения  
  virtual void AxisMove(char AxisNum, Coordinate & CurrientCoordinate, Coordinate NewCoordinate) = 0; // Перемещение по одной оси

  inline void Move(Coordinate TargetPosition[]); // Перемещение каретки в новыю точку  
  inline void ScanMove(char AxisNum, Coordinate StepSize); // Пошаговое перемещение по одной оси  

  inline DecartCoordinates GetPosition(); // Вывод положения каретки
};

Carriage::Carriage(DecartCoordinates NewDecartCoordinates = DecartCoordinates())
{
  Position = NewDecartCoordinates;
};

void Carriage::Move(Coordinate TargetPosition[])
{
  if ( (Position[X] != TargetPosition[X]) // Проверка необходимости перемещания головки
  or   (Position[Y] != TargetPosition[Y]) )
  {
    AxisMove (Z, Position[Z], SAFE_HEIGHT); // Поднятие острия для безопасного перемещания
  }; 
  for (char AxisNum = 0; AxisNum < 3 ; AxisNum++)
  {
    if (Position[AxisNum] != TargetPosition[AxisNum]) {
      AxisMove (AxisNum, Position[AxisNum], TargetPosition[AxisNum]);
    }; 
  }; 
};

void Carriage::ScanMove(char AxisNum, Coordinate StepSize)
{
  AxisMove (AxisNum, Position[AxisNum], Position[AxisNum] + StepSize);
}; 

DecartCoordinates Carriage::GetPosition()
{
  return Position;
};

//==============================================================================
#endif




