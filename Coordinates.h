#ifndef Coordinates_h
#define Coordinates_h

#include "SharedData.h"
//==============================================================================

enum DecartCoordinatesAxles
{
  X, Y, Z // Синонимы для осей
};

//==========================================================

class DecartCoordinates // Структура координаты
{
private:
  DataArray < Coordinate, 3 > Position;
public:
  inline DecartCoordinates(Coordinate NewX, Coordinate NewY, Coordinate NewZ);
  inline Coordinate & operator[](unsigned char Axis);
};

DecartCoordinates::DecartCoordinates(Coordinate NewX = 0 , Coordinate NewY = 0, Coordinate NewZ = 0)
{
  Position[::X] = NewX;
  Position[::Y] = NewY;
  Position[::Z] = NewZ;
};

Coordinate & DecartCoordinates::operator[](unsigned char Axis)
{
  return Position[Axis];
};

//==============================================================================
#endif
