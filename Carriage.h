#define SAFE_HEIGHT 8000
#define MOVERS_SYSTEM 4

class Carriage {
public:  
  void * Movers[MOVERS_SYSTEM]; // Система перемещения
  Coordinate MoversPositions[MOVERS_SYSTEM]; // Текущее положение системы перемещения
  DecartCoordinates Location;
//  LocationItem Location;

  Carriage();
  Carriage(char MoversType);
  inline void CoordinateTransformation(); // Преобразование координат
  inline void Move(Coordinate TargetPosition[]); // Перемещение в новыю точку
  inline void AxisMove(char AxisNum, Coordinate & CurrientCoordinate, Coordinate NewCoordinate); // Перемещение по одной оси
  inline void ScanMove(char AxisNum); // Пошаговое перемещение по оси Z во время сканирования
  inline Coordinate GetPosition(char axis); // Вывод положения
  void StepMoversInitialization(); // Инициализация как системы шаговых двигателей
  void PiezoMoverInitialization(); // Инициализация как системы пьезокерамики
};

Carriage::Carriage() {
};

Carriage::Carriage(char MoversType) {
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
  if ( (Location.X != TargetPosition[X]) // Проверка необходимости перемещания головки
  or   (Location.Y != TargetPosition[Y]) )
  {
    AxisMove (Z, Location.Z, SAFE_HEIGHT); // Поднятие острия для безопасного перемещания
  }; 
  for (char i = 0; i < 3 ; i++)
  {
    if (Location.Position[i] != TargetPosition[i]) {
      AxisMove (i, Location.Position[i], TargetPosition[i]);
    }; 
  }; 
};

void Carriage::ScanMove(char AxisNum) {
  AxisMove (AxisNum, Location.Position[AxisNum], Location.Position[AxisNum] + 1);
}; 

Coordinate Carriage::GetPosition(char Axis) {
  return Location.Position[Axis];
};
