#ifndef Coordinates_h
#define Coordinates_h

enum Axles
{
  X, Y, Z // Синонимы для осей
};

union DecartCoordinates2 // Структура координаты
{
private:
  struct
  {
    //    Coordinate X, Y, Z;
  };
  DataArray < Coordinate, 3 > Position_;
  char Chars[];
public:
  DecartCoordinates2();
  DecartCoordinates2(Coordinate TempX, Coordinate TempY, Coordinate TempZ);
  char & operator[](unsigned char  Item);
  inline char CharsLength();

  Coordinate & Position(char &PositionNum)
  {
    return Position_[PositionNum];
  };

  Coordinate & X()
  {
    return Position_[::X];
  };

  Coordinate & Y()
  {
    return Position_[::Y];
  };

  Coordinate & Z()
  {
    return Position_[::Z];
  };

};

union DecartCoordinates // Структура координаты
{
  struct
  {
    Coordinate X, Y, Z;
  };
  DataArray < Coordinate, 3 > Position;
  char Chars[];

  DecartCoordinates();
  DecartCoordinates(Coordinate TempX, Coordinate TempY, Coordinate TempZ);
  char & operator[](unsigned char  Item);
  inline char CharsLength();
};

DecartCoordinates::DecartCoordinates()
{
  for(unsigned char i = 0; i < sizeof(*this); i++)
  {
    this->Chars[i] = '!';
  };
};

DecartCoordinates::DecartCoordinates(Coordinate TempX, Coordinate TempY, Coordinate TempZ)
{
  X = TempX;
  Y = TempY;
  Z = TempZ;
};

char & DecartCoordinates::operator[](unsigned char Item)
{
  if (Item >= 0 and Item < CharsLength() )
  {
    return Chars[Item];
  }
  else
  {
    ::abort(0);
  };
};

char DecartCoordinates::CharsLength()
{
  return sizeof(*this) / sizeof(Chars[0]);
};

#endif

