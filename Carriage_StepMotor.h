#ifndef Carriage_StepMotor_h
#define Carriage_StepMotor_h

#define STEPPER_X_PIN_1 2 // Контакт 1 для управления головкий по X
#define STEPPER_X_PIN_2 3 // Контакт 2 для управления головкий по X
#define STEPPER_X_PIN_3 4 // Контакт 3 для управления головкий по X
#define STEPPER_X_PIN_4 5 // Контакт 4 для управления головкий по X
#define STEPPER_Y_PIN_1 6 // Контакт 1 для управления головкий по Y
#define STEPPER_Y_PIN_2 7 // Контакт 2 для управления головкий по Y
#define STEPPER_Y_PIN_3 8 // Контакт 3 для управления головкий по Y
#define STEPPER_Y_PIN_4 9 // Контакт 4 для управления головкий по Y
#define STEPPER_Z_PIN_1 10 // Контакт 1 для управления головкий по Z
#define STEPPER_Z_PIN_2 11 // Контакт 2 для управления головкий по Z
#define STEPPER_Z_PIN_3 12 // Контакт 3 для управления головкий по Z
#define STEPPER_Z_PIN_4 13 // Контакт 4 для управления головкий по Z
#define STEPPER_X_STEPS 32 // Количество шагов двигателя головки по X на полный оборот
#define STEPPER_Y_STEPS 32 // Количество шагов двигателя головки по Y на полный оборот
#define STEPPER_Z_STEPS 32 // Количество шагов двигателя головки по Z на полный оборот
#define STEPPER_X_SPEED 500 // Скорость двигателя головки по X
#define STEPPER_Y_SPEED 500 // Скорость двигателя головки по Y
#define STEPPER_Z_SPEED 500 // Скорость двигателя головки по Z

Stepper StepMotors[]= { // Изменен порядок подключения контактов с 1-2-3-4 на 1-3-2-4 (Необходимо только для двигателей 28YBJ-48 !!!
  Stepper (STEPPER_X_STEPS, STEPPER_X_PIN_1, STEPPER_X_PIN_3, STEPPER_X_PIN_2, STEPPER_X_PIN_4),
  Stepper (STEPPER_Y_STEPS, STEPPER_Y_PIN_1, STEPPER_Y_PIN_3, STEPPER_Y_PIN_2, STEPPER_Y_PIN_4),
  Stepper (STEPPER_Z_STEPS, STEPPER_Z_PIN_1, STEPPER_Z_PIN_3, STEPPER_Z_PIN_2, STEPPER_Z_PIN_4)
  };

void Carriage::StepMoversInitialization()
{
    for (char i = 0; i < sizeof(StepMotors)/sizeof(StepMotors[0]); i++)
  {
    Movers[i] =  (Stepper *) &StepMotors[i];
  };
  (*(Stepper *)Movers[X]).setSpeed(STEPPER_X_SPEED); // Установка скорости двигателя X
  (*(Stepper *)Movers[Y]).setSpeed(STEPPER_Y_SPEED); // Установка скорости двигателя Y
  (*(Stepper *)Movers[Z]).setSpeed(STEPPER_Z_SPEED); // Установка скорости двигателя Z
};

#endif
