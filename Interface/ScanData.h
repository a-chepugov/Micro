#include <iostream>
#include <stdlib.h>
#include <vector>
#include "ScanField.h"
#include "./../Command.h"
#include "./../Coordinates.h"

class ScanRecord {
private:
public:
    std::vector<DecartCoordinates> PointDataGrid; // Массив данных из точек сканирования
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
    ScanParameters.Parameters[CommandName] = COMMAND_PREFIX_SCANNING;
    ScanParameters.Parameters[HeadX] = Fields[0].GetScanTrajectoryPoint(Point).Position[X];
    ScanParameters.Parameters[HeadY] = Fields[0].GetScanTrajectoryPoint(Point).Position[Y];
    ScanParameters.Parameters[HeadZ] = Fields[0].GetScanTrajectoryPoint(Point).Position[Z];
    ScanParameters.Parameters[TipX] = Fields[0].GetScanTrajectoryPoint(Point).Position[X];
    ScanParameters.Parameters[TipY] = Fields[0].GetScanTrajectoryPoint(Point).Position[Y];
    ScanParameters.Parameters[TipZ] = Fields[0].GetScanTrajectoryPoint(Point).Position[Z];
    ScanParameters.Parameters[U] = 7000;
    ScanParameters.Parameters[Icrit] = 500;
    ScanParameters.Parameters[Pcrit] = 600;
    ScanParameters.SetCSum();

    QString CommandRecord = COMMAND_PREFIX;
    for(char i = 0; i < ScanParameters.CharsNum(); i++)
    {
        CommandRecord.append(ScanParameters.Chars[i]);
    };
    return CommandRecord;
};
