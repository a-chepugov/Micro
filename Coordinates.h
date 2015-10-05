void exit();

enum Axles
{
  X, Y, Z // Синонимы для осей
};

enum 
{
  ScanStart, ScanEnd  // Синонимы положения
};

typedef short int Coordinate; // Тип переменной для хранения координаты


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
  char & operator[](char Item);
  inline char CharsLength();  
};

DecartCoordinates::DecartCoordinates()
{
  for(char i = 0; i < sizeof(*this); i++)
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

char & DecartCoordinates::operator[](char Item)
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

char DecartCoordinates::CharsLength()
{
  return sizeof(*this) / sizeof(Chars[0]);
};
