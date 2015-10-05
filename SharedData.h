#define COMMAND_PREFIX "i" // Команда начала работы
#define COMMAND_PREFIX_MOVE 'M' // Код команды операции для туннельного сканирования
#define COMMAND_PREFIX_SCANNING 'S' // Код команды операции для туннельного сканирования
#define COMMAND_PREFIX_PROFILING 'R' // Код команды операции снятия профиля (режим профилографа)
#define COMMAND_PREFIX_APPROACHING 'A' // Код команды приближения зонда к образцу
#define COMMAND_PREFIX_PRICKING 'P' // Код команды операции индентирования
#define COMMAND_PREFIX_CALIBRATION 'C' // Код команды операции перекалибровки устройства
#define STATE_PREFIX_IDLE_STANDING 'M'  // Сигнализация простоя
#define STATE_PREFIX_WORKING 'W'  // Сигнализация работы
#define STATE_PREFIX_OUTPUT_RESULT 'R' // Сигнализация выдачи результата
#define STATE_PREFIX_ERROR 'E'  // Сигнализация выдачи ошибки

enum CommandParametersNames
{
    HeadX, HeadY, HeadZ, TipX, TipY, TipZ, U, Icrit, Pcrit, CSum, COUNT_OF_COMMAND_PARAMETERS  // COUNT_OF_COMMAND_PARAMETERS - Количество входящих параметров
};

enum ScanSensorsNames
{
    T, Humidity, Vibration, Itip, Ptip, COUNT_OF_SENSORS // Перечень сенсоров
};

enum Coordinates
{
    X, Y, Z // Синонимы для координат
};

enum ErrorNames
{
    SerialOutputError, BadCommand, PositioningErrorHead, PositioningErrorTip, NoHeadDrives, NoTipDrives, NoSensors // Перечень ошибок
};

typedef short int Coordinate;


union DecartCoordinates
{
    struct
    {
        Coordinate X, Y, Z;
    };
    Coordinate Coordinates[];
    char String[];
};

union CommandParameters
{
    Coordinate Parameters[COUNT_OF_COMMAND_PARAMETERS];
    char String[];
};

class LocationData { // Структура координаты сканирования
public:
    Coordinate Position[3];
    LocationData()
    {
        for(char i = 0; i < sizeof(Position)/sizeof(Position[0]); i++)
        {
            Position[i] = 8000;
        };
    };
};

struct ScanData { // Структура данных сканирования
public:
    Coordinate Point;
    int ScanValue;
    ScanData()
    {
        Point = ScanValue = 0;
    };
};

struct SystemData { // Структура состояния системы
public:
    char State; // Состояние системы
    bool Errors[8]; // Статус ошибок системы
    SystemData()
    {
        State = STATE_PREFIX_IDLE_STANDING;

        for(char i = 0; i < sizeof(Errors)/sizeof(Errors[0]); i++)
        {
            Errors[i] = 0;
        };
    };
};

struct SensorsData { // Структура показаний сенсоров
public:
    int SensorsValue[COUNT_OF_SENSORS - 2]; // Показания датчиков
    ScanData ScanResult[3]; // Данные сканирования
    SensorsData()
    {
        for(char i = 0; i < sizeof(SensorsValue)/sizeof(SensorsValue[0]); i++)
        {
            SensorsValue[i] = 0;
        };
        for(char i = 0; i < sizeof(ScanResult)/sizeof(ScanResult[0]); i++)
        {
            ScanResult[i] = ScanData();
        };
    };
};

struct CommandData {  // Структура команды
public:
    char CommandName; // Команда
    int CommandParameters[COUNT_OF_COMMAND_PARAMETERS]; // Параметры сканирования
    CommandData()
    {
        CommandName = '\0';
        for(char i = 0; i < sizeof(CommandParameters)/sizeof(CommandParameters[0]); i++)
        {
            CommandParameters[i] = 0;
        };
    }

    void SetCSum();
    bool CheckCSum();
    void SetCommandParameters(char ParameterNum, char *Array); // Установка параметра команды
    int GetCommandParameters(char ParameterNum); // Получение параметра команды
};

void CommandData::SetCSum()
{
    int tempCSum = 0;
    for (char i = 0; i < CSum; i++)
    {
        tempCSum  += CommandParameters[i];
    };
    CommandParameters[CSum] = tempCSum ;
};

bool CommandData::CheckCSum()
{
    int CSum = 0;
    for (char i = 0; i < CSum; i++)
    {
        CSum += CommandParameters[i];
    };
    if (CSum == CommandParameters[CSum])
    {
        return true;
    }
    else
    {
        return false;
    };
};

void CommandData::SetCommandParameters(char ParameterNum, char *Array)
{
    CommandParameters[ParameterNum] = *(int *) Array;
};

int CommandData::GetCommandParameters(char ParameterNum)
{
    if (ParameterNum > sizeof(CommandParameters) / sizeof(CommandParameters[0]) )
    {
    }
    else return CommandParameters[ParameterNum];
};

struct OperationData : 
        public CommandData{ // Сводная структура для вывода данных
public:
    OperationData() :
        CommandData()
    {
    };
};





