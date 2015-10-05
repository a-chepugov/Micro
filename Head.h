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
#define HEAD_AXIS_NUMBER 3  // Количество осей головки

Stepper HeadSteppers[HEAD_AXIS_NUMBER]= { // Изменен порядок подключения контактов с 1-2-3-4 на 1-3-2-4 (Необходимо только для двигателей 28YBJ-48 !!!
  Stepper (HEAD_STEPPER_X_STEPS, PIN_HEAD_X_1, PIN_HEAD_X_3, PIN_HEAD_X_2, PIN_HEAD_X_4),
  Stepper (HEAD_STEPPER_Y_STEPS, PIN_HEAD_Y_1, PIN_HEAD_Y_3, PIN_HEAD_Y_2, PIN_HEAD_Y_4),
  Stepper (HEAD_STEPPER_Z_STEPS, PIN_HEAD_Z_1, PIN_HEAD_Z_3, PIN_HEAD_Z_2, PIN_HEAD_Z_4)
  }; 

  Head::Head() {
    HeadSteppers[HeadX].setSpeed(HEAD_STEPPER_X_SPEED); // Установка скорости двигателя X
    HeadSteppers[HeadY].setSpeed(HEAD_STEPPER_Y_SPEED); // Установка скорости двигателя Y
    HeadSteppers[HeadZ].setSpeed(HEAD_STEPPER_Z_SPEED); // Установка скорости двигателя Z
    for (char i = 0; i < sizeof(HeadSteppers) / sizeof(HeadSteppers[0]); i++)
    {
      HeadPosition[i] = 8000; 
    }; 
  }; 

void Head::HeadMove(unsigned int TargetHeadPosition[]) {
  if ((HeadPosition[HeadX] != TargetHeadPosition[HeadX]) // Проверка необходимости перемещания головки
  or ( HeadPosition[HeadY] != TargetHeadPosition[HeadY])) {
    HeadAxisMove (&HeadPosition[HeadZ], 8000, HeadZ); // Поднятие острия для безопасного перемещания
  }; 
  for (char i = 0; i < sizeof(HeadPosition) / sizeof(HeadPosition[0]); i++)
  {
    if (HeadPosition[i] != TargetHeadPosition[i]) {
      HeadAxisMove (&HeadPosition[i], TargetHeadPosition[i], i); 
    }; 
  }; 
};

void Head::HeadZMove() { // Пошаговое перемещение острия по оси Z во время сканирования
  HeadAxisMove (&HeadPosition[HeadZ], HeadPosition[HeadZ] + 1, HeadZ); 
}; 

unsigned int Head::GetHeadPosition(char axis) { // Печать положения головки
  return HeadPosition[axis];
}; 

