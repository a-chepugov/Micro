#define PIN_TIP_X 20 // Контакт для управления острием по X
#define PIN_TIP_Y 21 // Контакт для управления острием по Y
#define PIN_TIP_Z 22 // Контакт для управления острием по Z

class Piezo
{
public:
  Piezo()
  {
  };
  void step(int); // Перемещение 
};

void Piezo::step(int Speed)
{
};

Piezo Piezos[] = {
  Piezo(),
  Piezo(),
  Piezo()
  };

  void Carriage::PiezoMoverInitialization()
  {
    for (char i = 0; i < sizeof(Piezos)/sizeof(Piezos[0]); i++)
    {
      Movers[i] =  (Piezo *) &Piezos[i];
    };
  };
