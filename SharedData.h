#define COMMAND_PREFIX "i" // Команда начала работы
#define COMMAND_PREFIX_SCANNING 'S' // Код команды операции для туннельного сканирования
#define COMMAND_PREFIX_PROFILING 'R' // Код команды операции снятия профиля (режим профилографа)
#define COMMAND_PREFIX_PRICKING 'P' // Код команды операции индентирования
#define COMMAND_PREFIX_CALIBRATION 'C' // Код команды операции перекалибровки устройства

#define STATE_PREFIX_IDLE_STANDING 'I'  // Сигнализация простоя
#define STATE_PREFIX_WORKING 'W'  // Сигнализация работы
#define STATE_PREFIX_OUTPUT_RESULT 'R' // Сигнализация выдачи результата
#define STATE_PREFIX_ERROR 'E'  // Сигнализация выдачи ошибки

enum ScanInputParametersNames
{
    HeadX, HeadY, HeadZ, TipX, TipY, TipZ, U, Icrit, Pcrit, CCsum, COUNT_OF_INPUT_PARAMETERS  // COUNT_OF_INPUT_PARAMETERS - Количество входящих параметров
};

enum ScanSensorsNames
{
    T, Humidity, Vibration, COUNT_OF_SENSORS
};

enum Coordinates
{
    X, Y, Z // Синонимы для координат
};

enum ErrorState
{
    NoHeadDrives, NoTipDrives, NoSensors, SerialOutputError, BadCommand, HeadPositioningError, TipPositioningError
};

struct ScanRecord {
    int Position[3]; // Координата сканирования
};

struct SensorsRecord {
    int SensorsData[COUNT_OF_SENSORS]; // Данные сенсоров
};

struct PointRecord {
    char State; // Состояние системы
    char Command; // Команда
    int ScanParameters[COUNT_OF_INPUT_PARAMETERS]; // Параметры сканирования
    int I_fx [3]; // Массив данных сканирования (зависимость тока от высоты)
    bool ErrorState[8]; // Статус ошибок системы
};

int GetCCSum(int * ScanCommand)
{
    int CCsum = 0;
    for (char i = 0; i < COUNT_OF_INPUT_PARAMETERS - 1; i++)
    {
        CCsum += ScanCommand[i];
    };
    return CCsum;
};










