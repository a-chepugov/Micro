#include <iostream>
#include <stdlib.h>
#include <vector>
#include "./../SharedData.h"

enum ScanTrajectoryTypes {TZ = 'Z', TN = 'N', TX = 'X', TY = 'Y'};

class ScanField  { // Данные о поле сканирования
private:
    LocationData ScanRange[2]; // Размер поля сканирования
    LocationData ScanWindow[2]; // Границы текущего поля сканирования
    char ScanTrajectoryType; // Вид траектории
    std::vector<LocationData> Trajectory; // Траектория перемещения
    unsigned long ScanPosition; // Текущее положение на траектории
public:
    ScanField();
    void SetScanFieldRange(); // Задать максимальный размер поля сканирования
    LocationData GetFieldLowLimit(); // Получить нижний предел поля сканирования
    LocationData GetFieldHighLimit(); // Получить верхний предел поля сканирования
    void SetScanWindowRange(); // Задать положение текущего поля сканирования
    void SetScanWindowRange(Coordinate Xmin, Coordinate Xmax, Coordinate Ymin, Coordinate Ymax); // Задать положение текущего поля сканирования
    LocationData GetWindowLowLimit(); // Получить нижний предел окна сканирования
    LocationData GetWindowHighLimit(); // Получить верхний предел окна сканирования

    void SetScanTrajectoryType(char NewScanTrajectoryType); // Задать тип траектории сканирования
    char GetScanTrajectoryType(); // Получить тип траектории сканирования
    void SetScanTrajectory(); // Задать траекторию сканирования

    void CreateTrajectoryX();
    void CreateTrajectoryY();
    void CreateTrajectoryZ();
    void CreateTrajectoryN();

    unsigned long GetScanTrajectoryLength(); // Получить длину траектории
    LocationData GetScanTrajectoryPoint(unsigned long Point); // Получить координату траектории по точке
};

ScanField::ScanField()
{
    LocationData ScanRange[] = {LocationData(), LocationData()};
    LocationData ScanWindow[] = {LocationData(), LocationData()};
    char ScanTrajectoryType = TZ;
    Trajectory.clear();
    ScanPosition = 0;
};

void ScanField::SetScanFieldRange()
{
    for (int i = 0; i < sizeof(ScanRange[0].Position) / sizeof(ScanRange[0].Position[i]); i++)
    {
        again:
        ScanRange[0].Position[i] = rand () %50;
        ScanRange[1].Position[i] = rand () %50;
        if (ScanRange[0].Position[i] == ScanRange[1].Position[i])
        {
            goto again;
        };
        if (ScanRange[0].Position[i] > ScanRange[1].Position[i])
        {
            unsigned int temp = ScanRange[0].Position[i];
            ScanRange[0].Position[i] = ScanRange[1].Position[i];
            ScanRange[1].Position[i] = temp;
        }
    }
};
void ScanField::SetScanWindowRange()
{
//    SetScanFieldRange();
    for (int i = 0; i < sizeof(ScanWindow[0].Position) / sizeof(ScanWindow[0].Position[i]); i++)
    {
        ScanWindow[0].Position[i] = rand () % (ScanRange[1].Position[i] - ScanRange[0].Position[i]) + ScanRange[0].Position[i];
        ScanWindow[1].Position[i] = rand () % (ScanRange[1].Position[i] - ScanRange[0].Position[i]) + ScanRange[0].Position[i];
        if (ScanWindow[0].Position[i] > ScanWindow[1].Position[i])
        {
            unsigned int temp = ScanWindow[0].Position[i];
            ScanWindow[0].Position[i] = ScanWindow[1].Position[i];
            ScanWindow[1].Position[i] = temp;
        };
    };
};

void ScanField::SetScanWindowRange(Coordinate Xmin, Coordinate Xmax, Coordinate Ymin, Coordinate Ymax)
{
    ScanWindow[0].Position[0] = Xmin;
    ScanWindow[1].Position[0] = Xmax;
    ScanWindow[0].Position[1] = Ymin;
    ScanWindow[1].Position[1] = Ymax;
};


LocationData ScanField::GetFieldLowLimit()
{
    return ScanRange[0];
};
LocationData ScanField::GetFieldHighLimit()
{
    return ScanRange[1];
};
LocationData ScanField::GetWindowLowLimit()
{
    return ScanWindow[0];
};
LocationData ScanField::GetWindowHighLimit()
{
    return ScanWindow[1];
};

void ScanField::SetScanTrajectoryType(char NewScanTrajectoryType)
{
    ScanTrajectoryType = NewScanTrajectoryType;
}

char ScanField::GetScanTrajectoryType()
{
    return ScanTrajectoryType;
}

void ScanField::SetScanTrajectory ()
{
    Trajectory.clear();
setDefaultScanTrajectory:
    switch (ScanTrajectoryType)
    {
    case TX:
    {
        CreateTrajectoryX();
    };
        break;
    case TY:
    {
        CreateTrajectoryY();
    };
    case TZ:
    {
        CreateTrajectoryZ();
    };
        break;
    case TN:
    {
        CreateTrajectoryN();
    };
        break;
    default:
        ScanTrajectoryType='Z';
        goto setDefaultScanTrajectory;
        break;
    };

};

void ScanField::CreateTrajectoryX()
{
    Trajectory.clear();
    for (long i_y = ScanWindow[0].Position[1]; i_y <= ScanWindow[1].Position[1]; i_y++)
    {
        for (long i_x = ScanWindow[0].Position[0]; i_x <= ScanWindow[1].Position[0]; i_x++)
        {
            LocationData Point;
            Point.Position[X] = i_x;
            Point.Position[Y] = i_y;
            Trajectory.push_back(Point);
        };
    };
};

void ScanField::CreateTrajectoryY()
{
    Trajectory.clear();
    for (long i_x = ScanWindow[0].Position[0]; i_x <= ScanWindow[1].Position[0]; i_x++)
    {
        for (long i_y = ScanWindow[0].Position[1]; i_y <= ScanWindow[1].Position[1]; i_y++)
        {
            LocationData Point;
            Point.Position[X] = i_x;
            Point.Position[Y] = i_y;
            Trajectory.push_back(Point);
        };
    };
};

void ScanField::CreateTrajectoryZ()
{
    Trajectory.clear();
    bool Direction = true;
    for (long i_y = ScanWindow[0].Position[1]; i_y <= ScanWindow[1].Position[1]; i_y++)
    {
        if (Direction)
        {
            for (long i_x = ScanWindow[0].Position[0]; i_x <= ScanWindow[1].Position[0]; i_x++)
            {
                LocationData Point;
                Point.Position[X] = i_x;
                Point.Position[Y] = i_y;
                Trajectory.push_back(Point);
            };
            Direction = false;
        }
        else
        {
            for (long i_x = ScanWindow[1].Position[0]; i_x >= ScanWindow[0].Position[0]; i_x--)
            {
                LocationData Point;
                Point.Position[X] = i_x;
                Point.Position[Y] = i_y;
                Trajectory.push_back(Point);
            };
            Direction = true;
        };
    };
};

void ScanField::CreateTrajectoryN()
{
    Trajectory.clear();
    bool Direction = true;
            for (long i_x = ScanWindow[0].Position[0]; i_x <= ScanWindow[1].Position[0]; i_x++)
    {
        if (Direction)
        {
    for (long i_y = ScanWindow[0].Position[1]; i_y <= ScanWindow[1].Position[1]; i_y++)
            {
                LocationData Point;
                Point.Position[X] = i_x;
                Point.Position[Y] = i_y;
                Trajectory.push_back(Point);
            };
            Direction = false;
        }
        else
        {
    for (long i_y = ScanWindow[1].Position[1]; i_y >= ScanWindow[0].Position[1]; i_y--)
            {
                LocationData Point;
                Point.Position[X] = i_x;
                Point.Position[Y] = i_y;
                Trajectory.push_back(Point);
            };
            Direction = true;
        };
    };
};

unsigned long ScanField::GetScanTrajectoryLength()
{
    return Trajectory.size();
};

LocationData ScanField::GetScanTrajectoryPoint (unsigned long Point)
{
    return Trajectory.at(Point);
};
