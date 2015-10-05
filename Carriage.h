#define SAFE_HEIGHT 8000

class Carriage : 
public LocationData {
private:
  void * Movers[4]; // Система перемещения
  Coordinate MoversPosition[3]; // Текущее положение системы перемещения
public:
  Carriage();
  Carriage(char MoversType);
  inline void Move(Coordinate TargetPosition[]); // Перемещение в новыю точку
  inline void CoordinateTransformation(); // Преобразование координат
  inline void AxisMove(Coordinate *currient_coordinate, Coordinate new_coordinate, char AxisNum); // Перемещение по одной оси
  inline void ScanMove(char AxisNum); // Пошаговое перемещение по оси Z во время сканирования
  inline void Approach(); // Подведение к образцу;
  inline int GetPosition(char axis); // Вывод положения
  void StepMoversInitialization(); // Инициализация как системы шаговых двигателей
  void PiezoMoverInitialization(); // Инициализация как системы пьезокерамики
};

Carriage::Carriage() : 
LocationData() {
};

Carriage::Carriage(char MoversType) : 
LocationData() {
  switch (MoversType) {
  case 'S':
    StepMoversInitialization();
    break;
  case 'P':
    PiezoMoverInitialization();
    break;
  };
}; 

void CoordinateTransformation()
{
};

void Carriage::Move(Coordinate TargetPosition[]) {
  if ((Location.X != TargetPosition[X]) // Проверка необходимости перемещания головки
  or ( Location.Y != TargetPosition[Y])) {
    AxisMove (&Location.Z, SAFE_HEIGHT, Z); // Поднятие острия для безопасного перемещания
  }; 
  for (char i = 0; i < sizeof(Location) / sizeof(Location.X); i++)
  {
    if (Location.Coordinates[i] != TargetPosition[i]) {
      AxisMove (&Location.Coordinates[i], TargetPosition[i], i); 
    }; 
  }; 
};

void Carriage::ScanMove(char AxisNum) {
  AxisMove (&Location.Coordinates[AxisNum], Location.Coordinates[AxisNum] + 1, AxisNum); 
}; 

int Carriage::GetPosition(char Axis) {
  return Location.Coordinates[Axis];
};

