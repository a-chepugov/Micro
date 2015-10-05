#ifndef Carriage_Piezo_h
#define Carriage_Piezo_h

#define PIN_TIP_X 20 // Контакт для управления острием по X
#define PIN_TIP_Y 21 // Контакт для управления острием по Y
#define PIN_TIP_Z 22 // Контакт для управления острием по Z

#include "Carriage.h"
#include "Piezoer.h"

class Piezo3 : 
public Carriage
{
public:
  inline Piezo3();
  inline void AxisMove(char AxisNum, Coordinate & CurrientCoordinate, Coordinate NewCoordinate); // Перемещение по одной оси
  inline void MoverInitialization(); // Инициализация как системы пьезокерамики
};

Piezo3::Piezo3()
{
  pinMode(PIN_TIP_X, OUTPUT); // Установка режима записи контактов для управления положнием по оси X острия
  pinMode(PIN_TIP_Y, OUTPUT); // Установка режима записи контактов для управления положнием по оси Y острия
  pinMode(PIN_TIP_Z, OUTPUT); // Установка режима записи контактов для управления положнием по оси Z острия
  
  MoverInitialization();
};

Piezo Piezos[] = {
  Piezo(PIN_TIP_X),
  Piezo(PIN_TIP_X),
  Piezo(PIN_TIP_X)
  };

  void Piezo3::MoverInitialization()
  {
    for (char i = 0; i < sizeof(Piezos)/sizeof(Piezos[0]); i++)
    {
      Movers[i] =  (Piezo3 *) &Piezos[i];
    };
  };
  

#endif

