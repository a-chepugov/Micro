#include <iostream>
#include <stdlib.h>
#include <vector>
//#include "./../SharedData.h"
#include "ScanField.h"

class ScanRecord {
private:
    std::vector<LocationData> PointDataGrid; // Массив данных из точек сканирования
    bool InterfaceErrorState[8]; // Статус ошибок системы
public:
    ScanField Fields[2];
    ScanRecord ()
    {
        PointDataGrid.clear();
        for(char i = 0; i < sizeof(InterfaceErrorState)/sizeof(InterfaceErrorState[0]) ; i++)
        {
            InterfaceErrorState[i] = false;
        };
        for(char i = 0; i < sizeof(Fields)/sizeof(Fields[0]) ; i++)
        {
            Fields[i] = ScanField();
        };
    }

    QString CreateScanCommand (unsigned long Point);
} Data;

int GetCSum(int * ScanCommand);

QString ScanRecord::CreateScanCommand (unsigned long Point)
{
    CommandData ScanParameters;
    ScanParameters.CommandParameters[HeadX] = Fields[0].GetScanTrajectoryPoint(Point).Position[X];
    ScanParameters.CommandParameters[HeadY] = Fields[0].GetScanTrajectoryPoint(Point).Position[Y];
    ScanParameters.CommandParameters[HeadZ] = Fields[0].GetScanTrajectoryPoint(Point).Position[Z];
    ScanParameters.CommandParameters[TipX] = Fields[0].GetScanTrajectoryPoint(Point).Position[X];
    ScanParameters.CommandParameters[TipY] = Fields[0].GetScanTrajectoryPoint(Point).Position[Y];
    ScanParameters.CommandParameters[TipZ] = Fields[0].GetScanTrajectoryPoint(Point).Position[Z];
    ScanParameters.CommandParameters[U] = 7000;
    ScanParameters.CommandParameters[Icrit] = 500;
    ScanParameters.CommandParameters[Pcrit] = 600;
    ScanParameters.SetCSum();

    QString CommandRecord = COMMAND_PREFIX;
    char CommandName;
    CommandName = COMMAND_PREFIX_SCANNING;
    CommandRecord.append(CommandName);
    for(char i = 0; i < COUNT_OF_COMMAND_PARAMETERS; i++)
    {
        CommandRecord.append(ScanParameters.CommandParameters[i]);
    };
    return CommandRecord;
};
