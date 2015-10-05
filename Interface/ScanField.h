#include <iostream>
#include <stdlib.h>
#include <vector>
#include "./../Coordinates.h"

enum ScanTrajectoryTypes {TZ = 'Z', TN = 'N', TX = 'X', TY = 'Y'};

class ScanField  { // Данные о поле сканирования
private:
public:
    DecartCoordinates ScanRange[2]; // Размер поля сканирования
    DecartCoordinates ScanWindow[2]; // Границы текущего поля сканирования
    char ScanTrajectoryType; // Вид траектории
    std::vector<DecartCoordinates> Trajectory; // Траектория перемещения
    unsigned long ScanPosition; // Текущее положение на траектории
public:
    ScanField();
    void SetScanFieldRange(); // Задать максимальный размер поля сканирования
    DecartCoordinates GetFieldLowLimit(); // Получить нижний предел поля сканирования
    DecartCoordinates GetFieldHighLimit(); // Получить верхний предел поля сканирования
    void SetScanWindowRange(); // Задать положение текущего поля сканирования
    void SetScanWindowRange(Coordinate Xmin, Coordinate Xmax, Coordinate Ymin, Coordinate Ymax); // Задать положение текущего поля сканирования
    DecartCoordinates GetWindowLowLimit(); // Получить нижний предел окна сканирования
    DecartCoordinates GetWindowHighLimit(); // Получить верхний предел окна сканирования

    void SetScanTrajectoryType(char NewScanTrajectoryType); // Задать тип траектории сканирования
    char GetScanTrajectoryType(); // Получить тип траектории сканирования
    void SetScanTrajectory(); // Задать траекторию сканирования

    void CreateTrajectoryX();
    void CreateTrajectoryY();
    void CreateTrajectoryZ();
    void CreateTrajectoryN();

    unsigned long GetScanTrajectoryLength(); // Получить длину траектории
    DecartCoordinates GetScanTrajectoryPoint(unsigned long Point); // Получить координату траектории по точке
};

ScanField::ScanField()
{
    char ScanTrajectoryType = TZ;
    Trajectory.clear();
    ScanPosition = 0;
};

void ScanField::SetScanFieldRange()
{
    for (int i = 0; i < ScanRange[0].PositionsNum(); i++)
    {
again:
        ScanRange[0].Position[i] = rand () %10;
        ScanRange[1].Position[i] = rand () %100;
        if (ScanRange[0].Position[i] == ScanRange[1].Position[i])
        {
            goto again;
        };
        if (ScanRange[0].Position[i] > ScanRange[1].Position[i])
        {
            Coordinate temp = ScanRange[0].Position[i];
            ScanRange[0].Position[i] = ScanRange[1].Position[i];
            ScanRange[1].Position[i] = temp;
        }
    }
};
void ScanField::SetScanWindowRange()
{
    //    SetScanFieldRange();
    for (int i = 0; i < ScanWindow[0].PositionsNum(); i++)
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


DecartCoordinates ScanField::GetFieldLowLimit()
{
    return ScanRange[0];
};
DecartCoordinates ScanField::GetFieldHighLimit()
{
    return ScanRange[1];
};
DecartCoordinates ScanField::GetWindowLowLimit()
{
    return ScanWindow[0];
};
DecartCoordinates ScanField::GetWindowHighLimit()
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
        break;
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
    for (long i_y = ScanWindow[0].Position[Y]; i_y <= ScanWindow[1].Position[Y]; i_y++)
    {
        for (long i_x = ScanWindow[0].Position[X]; i_x <= ScanWindow[1].Position[X]; i_x++)
        {
            DecartCoordinates Point;
            Point.Position[X] = i_x;
            Point.Position[Y] = i_y;
            Trajectory.push_back(Point);
        };
    };
};

void ScanField::CreateTrajectoryY()
{
    Trajectory.clear();
    for (long i_x = ScanWindow[0].Position[X]; i_x <= ScanWindow[1].Position[X]; i_x++)
    {
        for (long i_y = ScanWindow[0].Position[Y]; i_y <= ScanWindow[1].Position[Y]; i_y++)
        {
            DecartCoordinates Point;
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
    for (long i_y = ScanWindow[0].Position[Y]; i_y <= ScanWindow[1].Position[Y]; i_y++)
    {
        if (Direction)
        {
            for (long i_x = ScanWindow[0].Position[X]; i_x <= ScanWindow[1].Position[X]; i_x++)
            {
                DecartCoordinates Point;
                Point.Position[X] = i_x;
                Point.Position[Y] = i_y;
                Trajectory.push_back(Point);
            };
            Direction = false;
        }
        else
        {
            for (long i_x = ScanWindow[1].Position[X]; i_x >= ScanWindow[0].Position[X]; i_x--)
            {
                DecartCoordinates Point;
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
    for (long i_x = ScanWindow[0].Position[X]; i_x <= ScanWindow[1].Position[X]; i_x++)
    {
        if (Direction)
        {
            for (long i_y = ScanWindow[0].Position[Y]; i_y <= ScanWindow[1].Position[Y]; i_y++)
            {
                DecartCoordinates Point;
                Point.Position[X] = i_x;
                Point.Position[Y] = i_y;
                Trajectory.push_back(Point);
            };
            Direction = false;
        }
        else
        {
            for (long i_y = ScanWindow[1].Position[Y]; i_y >= ScanWindow[0].Position[Y]; i_y--)
            {
                DecartCoordinates Point;
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

DecartCoordinates ScanField::GetScanTrajectoryPoint (unsigned long Point)
{
    return Trajectory.at(Point);
};
