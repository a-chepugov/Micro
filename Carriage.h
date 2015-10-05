#ifndef Carriage_h
#define Carriage_h

#define SAFE_HEIGHT 8000
#define MOVERS_SYSTEM 4

class Carriage {
private:
public:  
  void * Movers[MOVERS_SYSTEM]; // Система перемещения
  Coordinate MoversPositions[MOVERS_SYSTEM]; // Текущее положение системы перемещения
  DecartCoordinates Location;

  inline void Move(Coordinate TargetPosition[]); // Перемещение в новыю точку
  virtual inline void AxisMove(char AxisNum, Coordinate & CurrientCoordinate, Coordinate NewCoordinate); // Перемещение по одной оси
  inline Coordinate GetPosition(char axis); // Вывод положения


  inline void ScanMove(char AxisNum); // Пошаговое перемещение по оси Z во время сканирования
};

void Carriage::Move(Coordinate TargetPosition[]) {
  if ( (Location.X() != TargetPosition[X]) // Проверка необходимости перемещания головки
  or   (Location.Y() != TargetPosition[Y]) )
  {
    AxisMove (Z, Location.Z(), SAFE_HEIGHT); // Поднятие острия для безопасного перемещания
  }; 
  for (char AxisNum = 0; AxisNum < 3 ; AxisNum++)
  {
    if (Location.Position(AxisNum) != TargetPosition[AxisNum]) {
      AxisMove (AxisNum, Location.Position(AxisNum), TargetPosition[AxisNum]);
    }; 
  }; 
};

Coordinate Carriage::GetPosition(char Axis) {
  return Location.Position(Axis);
};

void Carriage::ScanMove(char AxisNum) {
  AxisMove (AxisNum, Location.Position(AxisNum), Location.Position(AxisNum) + 1);
}; 


#endif
