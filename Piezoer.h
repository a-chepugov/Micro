#ifndef Piezoer_h
#define Piezoer_h

class Piezo
{
private:
  unsigned char ControlPin;

public:
  inline Piezo(unsigned char ControlPin);
  inline void TakePosition(unsigned char Position);
};

void Piezo::TakePosition(unsigned char Position)
{
  analogWrite(ControlPin, Position);
};

Piezo::Piezo(unsigned char NewControlPin)
{
  ControlPin = NewControlPin;
  pinMode(ControlPin, OUTPUT); // Установка режима записи контакта для управления положнием
};


#endif
