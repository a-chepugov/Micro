#ifndef Coordinates_h
#define Coordinates_h

enum Axles
{
  X, Y, Z // Синонимы для осей
};

//==========================================================

union DecartCoordinates // Структура координаты
{
private:
  DataArray < Coordinate, 3 > Position_;
public:
  inline DecartCoordinates(Coordinate TempX, Coordinate TempY, Coordinate TempZ);
  inline Coordinate & Position(char &Axis);
  inline Coordinate & X();
  inline Coordinate & Y();
  inline Coordinate & Z();
};

DecartCoordinates::DecartCoordinates(Coordinate TempX = '!!', Coordinate TempY = '!!', Coordinate TempZ = '!!')
{
  Position_[::X] = TempX;
  Position_[::Y] = TempY;
  Position_[::Z] = TempZ;    
};

Coordinate & DecartCoordinates::Position(char &Axis)
{
  return Position_[Axis];
};

Coordinate & DecartCoordinates::X()
{
  return Position_[::X];
};

Coordinate & DecartCoordinates::Y()
{
  return Position_[::Y];
};

Coordinate & DecartCoordinates::Z()
{
  return Position_[::Z];
};


#endif
