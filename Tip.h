#define PIN_TIP_X 20 // Контакт для управления острием по X
#define PIN_TIP_Y 21 // Контакт для управления острием по Y
#define PIN_TIP_Z 22 // Контакт для управления острием по Z

#define PIN_SENSOR_TIP A0 //  Контакт для датчика острия
#define PIN_SENSOR_Ptip A1 // Контакт для датчика давления укола

void Tip::TipAxisMove(int * currient_coordinate, int new_coordinate, int TipAxisNum) { // Перемещение острия по одной координате
  // TipMover[TipAxisNum].move(new_coordinate - *currient_coordinate); 
  *currient_coordinate = new_coordinate; 
};

void Tip::TipMove(unsigned int TargetTipPosition[]) { // Перемещение острия
  int STargetTipPosition [3]; // Откорректированные значения целевых координат острия
  for (int i = 0; i < 2; i++)
  {
    STargetTipPosition[i] = TargetTipPosition[i] - (256 ^ (sizeof (TargetTipPosition[i])) / 2 ); // Коррекция координаты
  };
  if ((TipPosition[HeadX] != STargetTipPosition[HeadX])
    or ( TipPosition[HeadY] != STargetTipPosition[HeadY])) {
    TipAxisMove (&TipPosition[HeadZ], 0, HeadZ); // Поднятие острия для безопасного перемещания
  }; 
  for (char i = 0; i < sizeof(TipPosition) / sizeof(TipPosition[0]); i++) {
    if (TipPosition[i] != STargetTipPosition[i]) {
      TipAxisMove (&TipPosition[i], STargetTipPosition[i], i); 
    }; 
  }; 
};

int  Tip::GetTipPosition(char axis) { // Печать положения острия
  return TipPosition[axis];
};
