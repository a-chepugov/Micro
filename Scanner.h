class Scanner : 
protected ScanRecord {
public:
  Scanner();
  void Move(int TargetPosition[]); // Перемещение в новыю точку
  void AxisMove(int *currient_coordinate, int new_coordinate, int AxisNum); // Перемещение по одной оси
  void ScanMove(); // Пошаговое перемещение по оси Z во время сканирования
  int GetPosition(char axis); // Вывод положения
};

Scanner::Scanner() {
  for (char i = 0; i < sizeof(Position) / sizeof(Position[0]); i++)
  {
    Position[i] = 8000; 
  }; 
}; 

void Scanner::Move(int TargetPosition[]) {
  if ((Position[X] != TargetPosition[X]) // Проверка необходимости перемещания головки
  or ( Position[Y] != TargetPosition[Y])) {
    AxisMove (&Position[Z], 8000, Z); // Поднятие острия для безопасного перемещания
  }; 
  for (char i = 0; i < sizeof(Position) / sizeof(Position[0]); i++)
  {
    if (Position[i] != TargetPosition[i]) {
      AxisMove (&Position[i], TargetPosition[i], i); 
    }; 
  }; 
};

void Scanner::ScanMove() { // Пошаговое перемещение острия по оси Z во время сканирования
  AxisMove (&Position[Z], Position[Z] + 1, Z); 
}; 

int Scanner::GetPosition(char axis) { // Печать положения головки
  return Position[axis];
}; 

