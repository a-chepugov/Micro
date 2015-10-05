#define SAFE_HEIGHT 8000

class Carriage : 
public LocationData {
  private:
  void * Movers[4]; // Система перемещения
  Coordinate MoversPosition[4]; // Текущее положение системы перемещения
public:
  Carriage();
  Carriage(char MoversType);
  void Move(int TargetPosition[]); // Перемещение в новыю точку
  void CoordinateTransformation(); // Преобразование координат
  void AxisMove(Coordinate *currient_coordinate, Coordinate new_coordinate, char AxisNum); // Перемещение по одной оси
  void ScanMove(char AxisNum); // Пошаговое перемещение по оси Z во время сканирования
  void Approach(); // Подведение к образцу;
  int GetPosition(char axis); // Вывод положения
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

void Carriage::Move(int TargetPosition[]) {
  if ((Position[X] != TargetPosition[X]) // Проверка необходимости перемещания головки
  or ( Position[Y] != TargetPosition[Y])) {
    AxisMove (&Position[Z], SAFE_HEIGHT, Z); // Поднятие острия для безопасного перемещания
  }; 
  for (char i = 0; i < sizeof(Position) / sizeof(Position[0]); i++)
  {
    if (Position[i] != TargetPosition[i]) {
      AxisMove (&Position[i], TargetPosition[i], i); 
    }; 
  }; 
};

void Carriage::ScanMove(char AxisNum) {
  AxisMove (&Position[AxisNum], Position[AxisNum] + 1, AxisNum); 
}; 

int Carriage::GetPosition(char axis) {
  return Position[axis];
};
