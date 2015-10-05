#ifndef Carriage_Piezo_h
#define Carriage_Piezo_h
//==============================================================================

#define PIN_TIP_X 20 // Контакт для управления острием по X
#define PIN_TIP_Y 21 // Контакт для управления острием по Y
#define PIN_TIP_Z 22 // Контакт для управления острием по Z

#include "Piezoer.h"
#include "Carriage.h"

class Piezo3 : 
public Carriage
{
  void * Movers[]; // Система перемещения
  Coordinate MoversPositions[]; // Текущее положение системы перемещения
public:
  inline Piezo3(DecartCoordinates NewDecartCoordinates);

  inline void MoversInitialization(); // Инициализация как системы пьезокерамики
  inline void AxisMove(char AxisNum, Coordinate & CurrientCoordinate, Coordinate NewCoordinate);  // Перемещение по одной оси
};


Piezo3::Piezo3(DecartCoordinates NewDecartCoordinates) : 
Carriage(NewDecartCoordinates)
{
  pinMode(PIN_TIP_X, OUTPUT); // Установка режима записи контактов для управления положнием по оси X острия
  pinMode(PIN_TIP_Y, OUTPUT); // Установка режима записи контактов для управления положнием по оси Y острия
  pinMode(PIN_TIP_Z, OUTPUT); // Установка режима записи контактов для управления положнием по оси Z острия
  MoversInitialization();
};

Piezoer Piezos[] = {
  Piezoer(PIN_TIP_X),
  Piezoer(PIN_TIP_Y),
  Piezoer(PIN_TIP_Z)
  };

  void Piezo3::MoversInitialization()
  {
    for (char i = 0; i < sizeof(Piezos)/sizeof(Piezos[0]); i++)
    {
      Movers[i] =  (Piezo3 *) &Piezos[i];
    };    
  };

//==============================================================================
#endif






