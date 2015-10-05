#define PIN_HEAD_X_1 2 // Контакт 1 для управления головкий по X
#define PIN_HEAD_X_2 3 // Контакт 2 для управления головкий по X
#define PIN_HEAD_X_3 4 // Контакт 3 для управления головкий по X
#define PIN_HEAD_X_4 5 // Контакт 4 для управления головкий по X
#define PIN_HEAD_Y_1 6 // Контакт 1 для управления головкий по Y
#define PIN_HEAD_Y_2 7 // Контакт 2 для управления головкий по Y
#define PIN_HEAD_Y_3 8 // Контакт 3 для управления головкий по Y
#define PIN_HEAD_Y_4 9 // Контакт 4 для управления головкий по Y
#define PIN_HEAD_Z_1 10 // Контакт 1 для управления головкий по Z
#define PIN_HEAD_Z_2 11 // Контакт 2 для управления головкий по Z
#define PIN_HEAD_Z_3 12 // Контакт 3 для управления головкий по Z
#define PIN_HEAD_Z_4 13 // Контакт 4 для управления головкий по Z
#define HEAD_STEPPER_X_STEPS 32 // Количество шагов двигателя головки по X на полный оборот
#define HEAD_STEPPER_Y_STEPS 32 // Количество шагов двигателя головки по Y на полный оборот
#define HEAD_STEPPER_Z_STEPS 32 // Количество шагов двигателя головки по Z на полный оборот
#define HEAD_STEPPER_X_SPEED 500 // Скорость двигателя головки по X
#define HEAD_STEPPER_Y_SPEED 500 // Скорость двигателя головки по Y
#define HEAD_STEPPER_Z_SPEED 500 // Скорость двигателя головки по Z

Stepper HeadSteppers[]= { // Изменен порядок подключения контактов с 1-2-3-4 на 1-3-2-4 (Необходимо только для двигателей 28YBJ-48 !!!
  Stepper (HEAD_STEPPER_X_STEPS, PIN_HEAD_X_1, PIN_HEAD_X_3, PIN_HEAD_X_2, PIN_HEAD_X_4),
  Stepper (HEAD_STEPPER_Y_STEPS, PIN_HEAD_Y_1, PIN_HEAD_Y_3, PIN_HEAD_Y_2, PIN_HEAD_Y_4),
  Stepper (HEAD_STEPPER_Z_STEPS, PIN_HEAD_Z_1, PIN_HEAD_Z_3, PIN_HEAD_Z_2, PIN_HEAD_Z_4)
  };

class HeadData : 
public Scanner { //
public:
  HeadData();
};

HeadData::HeadData() : 
Scanner () {
  HeadSteppers[HeadX].setSpeed(HEAD_STEPPER_X_SPEED); // Установка скорости двигателя X
  HeadSteppers[HeadY].setSpeed(HEAD_STEPPER_Y_SPEED); // Установка скорости двигателя Y
  HeadSteppers[HeadZ].setSpeed(HEAD_STEPPER_Z_SPEED); // Установка скорости двигателя Z
}; 


