
enum Axles
{
  X, Y, Z // Синонимы для осей
};

typedef short int Coordinate; // Тип переменной для хранения координаты

union DecartCoordinates // Структура координаты
{
  struct
  {
    Coordinate X, Y, Z;
  };
  Coordinate Coordinates[];
  char String[];
  DecartCoordinates()
  {
    for(char i = 0; i < sizeof(*this); i++)
    {
      this->String[i] = '!';
    };  
  }
};

class LocationData { 
public:
  DecartCoordinates Location;
};

