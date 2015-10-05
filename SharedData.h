#define COMMAND_PREFIX "i" // Команда начала работы
#define COMMAND_PREFIX_SCANNING 'S' // Код команды операции для туннельного сканирования
#define COMMAND_PREFIX_PROFILERING 'R' // Код команды операции снятия профиля (режим профилографа)
#define COMMAND_PREFIX_PRICKING 'P' // Код команды операции индентирования
#define COMMAND_PREFIX_CALIBRATION 'C' // Код команды операции перекалибровки устройства

#define STATE_PREFIX_IDLE_STANDING 'I'  // Сигнализация простоя
#define STATE_PREFIX_WORKING 'W'  // Сигнализация работы
#define STATE_PREFIX_OUTPUT_RESULT 'R' // Сигнализация выдачи результата
#define STATE_PREFIX_ERROR 'E'  // Сигнализация выдачи ошибки

#define HEAD_AXIS_NUMBER 3  // Количество осей головки

enum ScanInputParametersNames
{
  HeadX, HeadY, HeadZ, TipX, TipY, TipZ, U, Icrit, Pcrit, CCsum, COUNT_OF_INPUT_PARAMETERS  // COUNT_OF_INPUT_PARAMETERS - Количество входящих параметров
};

enum ErrorState
{
  NoHeadDrives, NoTipDrives, NoSensors, SerialOutputError, BadCommand, HeadPositioningError, TipPositioningError
};

struct HeadCoord {
};

class Head {
private :
  unsigned int HeadPosition[HEAD_AXIS_NUMBER]; // Положение головки
public :
  Head();
  void HeadMove(unsigned int TargetHeadPosition[]); // Передвижение головки
  void HeadAxisMove(unsigned int *currient_coordinate, unsigned int new_coordinate, int StepperNum); // Перемещение двигателя головки
  void HeadZMove(); // Пошаговое перемещение головки по оси Z во время сканирования
  unsigned int GetHeadPosition(char axis); // Печать положения головки  
};

class Tip {
private :
  int TipPosition[3]; // Положение острия
public :
  Tip();
  void TipAxisMove(int *currient_coordinate, int new_coordinate, int TipAxisNum); // Перемещение острия по одной координате
  void TipMove(unsigned int TargetTipPosition[]); // Перемещение острия
  int GetTipPosition(char axis); // Печать положения острия
}; 

class Sensors {
private :
  int SensorsData[4];
public :
  Sensors();
  void SensorsReset(); // Сброс данных сенсоров
  void GetEnvironmentProperties(); // Получание сенсорами параметров среды
  int GetSensorsData(char Num); // Получение данных сенсоров
}; 

class PointData :
public Head , public Tip , public Sensors {
private :
  char State; // Состояние системы
  bool ErrorState[8]; // Статус ошибок системы
  char Command; // Команда
  unsigned int ScanParameters[COUNT_OF_INPUT_PARAMETERS]; // Параметры сканирования
  int I_fx [8]; // Массив данных сканирования (зависимость тока от высоты)
public :
  PointData();
  void PointDataReset(); // Сброс данных точки
  void Processing(); // Ожидание команды
  void ReadCommand(); // Получение команды
  void CheckCommand(); // Проверка комманды
  void ExecuteCommand(); // Выполнение полученной команды
  void MovePrepareActions(); // Перемещение в заданные координаты и снятие данных датчиков
  void PointDataScan(); // Сканирование
  void PointPricking(); // Операция индентирование
  void PrintResult(); // Вывод данных операции
  void SetState(char state); // Установка статуса
  void SetError(char i);  // Установка статуса ошибки
  void PrintError();  // Вывод статуса ошибки
}; 

