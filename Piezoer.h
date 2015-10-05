#ifndef Piezoer_h
#define Piezoer_h
//==============================================================================

#include "SharedData.h"

class Piezoer
{
private:
  unsigned char ControlPin;

public:
  inline Piezoer(unsigned char ControlPin);
  inline void GoToPosition(unsigned char Position);
};

Piezoer::Piezoer(unsigned char NewControlPin)
{
  ControlPin = NewControlPin;
  pinMode(ControlPin, OUTPUT); // Установка режима записи контакта для управления положнием
};

void Piezoer::GoToPosition(unsigned char Position)
{
  if ( (Position + 128) > 0 )
  {
    analogWrite(ControlPin, Position + 128);
  };
};

//==============================================================================
#endif

