#define PIN_TIP_X 20 // Контакт для управления острием по X
#define PIN_TIP_Y 21 // Контакт для управления острием по Y
#define PIN_TIP_Z 22 // Контакт для управления острием по Z

#define PIN_SENSOR_TIP A0 //  Контакт для датчика острия
#define PIN_SENSOR_Ptip A1 // Контакт для датчика давления укола

class TipData : 
public Scanner {
public:
  TipData();
  void AxisMove(int *currient_coordinate, int new_coordinate, int AxisNum); // Перемещение по одной оси
}; 

void TipData::AxisMove(int *currient_coordinate, int new_coordinate, int AxisNum) {
};
