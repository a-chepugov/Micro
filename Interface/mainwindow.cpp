#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "IOUSB.h"
#include "ScanData.h"

int usbport = -1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    DecartCoordinates Point;
    Point.Position[X] = ui->verticalScrollBar_Xnew->value();
    Point.Position[Y] = ui->verticalScrollBar_Ynew->value();
    Data.Fields[0].SetScanWindowRange(ui->verticalScrollBar_Xnew->value(),ui->verticalScrollBar_Xnew->value(),ui->verticalScrollBar_Ynew->value(),ui->verticalScrollBar_Ynew->value());
    Data.Fields[0].SetScanTrajectory();
    ui->lineEdit->setText(Data.CreateScanCommand(0));
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->textEdit->insertPlainText("Openning port\n");
    usbport = openport (ui->PortName->text().toStdString().c_str());
    if (usbport == -1)
    {  // Возвращает файловый дескриптор при успехе или -1 при ошибке.
        ui->textEdit->insertPlainText("error port\n");
        ui->textEdit->insertPlainText("open_port: Unable to open ");
        ui->textEdit->insertPlainText(ui->PortName->text());
        ui->textEdit->insertPlainText("\n");
    }
    ui->textEdit->insertPlainText( (char *) &usbport );
    ui->textEdit->insertPlainText( "\n" );
}

void MainWindow::on_pushButton_3_clicked()
{
    unsigned long Length = Data.Fields[0].GetScanTrajectoryLength();
    for (unsigned long i = 0; i < Length; i++)
    {
        writetoport (usbport, Data.CreateScanCommand(i).toStdString().c_str() );
     };

}

void MainWindow::on_pushButton_4_clicked()
{
    //in.setCodec("ISO 8859-1");
    char buf[512];
    if (usbport != -1)
    {
        readport (usbport,buf);
        ui->textEdit->insertPlainText(buf);
        ui->textEdit->insertPlainText( "\n" );
    }
    else
    {
        ui->textEdit->insertPlainText("Port is not open\n");
    }

}

void MainWindow::on_pushButton_5_clicked()
{
    Data.Fields[0].SetScanFieldRange();

    //Data.Fields[0].ScanRange[0] = DecartPosition(1,2,3);
    //Data.Fields[0].ScanRange[0] = DecartPosition(4,5,6);

    DecartCoordinates HeadFieldLowLimit = Data.Fields[0].GetFieldLowLimit();
    DecartCoordinates HeadFieldHighLimit = Data.Fields[0].GetFieldHighLimit();
    ui->spinBox_Xmin->setValue(HeadFieldLowLimit.Position[0]);
    ui->spinBox_Ymin->setValue(HeadFieldLowLimit.Position[1]);
    ui->spinBox_Zmin->setValue(HeadFieldLowLimit.Position[2]);
    ui->spinBox_Xmax->setValue(HeadFieldHighLimit.Position[0]);
    ui->spinBox_Ymax->setValue(HeadFieldHighLimit.Position[1]);
    ui->spinBox_Zmax->setValue(HeadFieldHighLimit.Position[2]);
}

void MainWindow::on_pushButton_6_clicked()
{
    if (ui->radioButton_X->isChecked())
    {
        Data.Fields[0].SetScanTrajectoryType('X');
    };
    if (ui->radioButton_Y->isChecked())
    {
        Data.Fields[0].SetScanTrajectoryType('Y');
    };
    if (ui->radioButton_Z->isChecked())
    {
        Data.Fields[0].SetScanTrajectoryType('Z');
    };
    if (ui->radioButton_N->isChecked())
    {
        Data.Fields[0].SetScanTrajectoryType('N');
    };


    Data.Fields[0].SetScanTrajectory();

    ui->textEdit_trajectory->clear();

    ui->textEdit_trajectory->insertPlainText("Trajectory - ");
    ui->textEdit_trajectory->insertPlainText(QString(Data.Fields[0].GetScanTrajectoryType()));
    ui->textEdit_trajectory->insertPlainText("\n");

    unsigned long Length = Data.Fields[0].GetScanTrajectoryLength();
    for (unsigned long i = 0; i < Length; i++)
    {
        ui->textEdit_trajectory->insertPlainText(QString::number(Data.Fields[0].GetScanTrajectoryPoint(i).Position[0]));
        ui->textEdit_trajectory->insertPlainText(" - ");
        ui->textEdit_trajectory->insertPlainText(QString::number(Data.Fields[0].GetScanTrajectoryPoint(i).Position[1]));
        ui->textEdit_trajectory->insertPlainText(" : ");
        ui->textEdit_trajectory->insertPlainText(Data.CreateScanCommand(i));
        ui->textEdit_trajectory->insertPlainText("\n");
    };
}


void MainWindow::on_spinBox_Xnew_valueChanged(int arg1)
{
    ui->verticalScrollBar_Xnew->setValue(arg1);
}

void MainWindow::on_spinBox_Ynew_valueChanged(int arg1)
{
    ui->verticalScrollBar_Ynew->setValue(arg1);
}

void MainWindow::on_spinBox_Znew_valueChanged(int arg1)
{
    ui->verticalScrollBar_Znew->setValue(arg1);
}

void MainWindow::on_verticalScrollBar_Xnew_sliderMoved(int position)
{
    ui->spinBox_Xnew->setValue(position);
}

void MainWindow::on_verticalScrollBar_Ynew_sliderMoved(int position)
{
    ui->spinBox_Ynew->setValue(position);
}

void MainWindow::on_verticalScrollBar_Znew_sliderMoved(int position)
{
    ui->spinBox_Znew->setValue(position);
}

void MainWindow::on_pushButton_7_clicked()
{
    Data.Fields[0].SetScanWindowRange();
    DecartCoordinates HeadWindowLowLimit = Data.Fields[0].GetWindowLowLimit();
    DecartCoordinates HeadWindowHighLimit = Data.Fields[0].GetWindowHighLimit();
    ui->spinBox_Xstart->setValue(HeadWindowLowLimit.Position[0]);
    ui->spinBox_Ystart->setValue(HeadWindowLowLimit.Position[1]);
    ui->spinBox_Zstart->setValue(HeadWindowLowLimit.Position[2]);
    ui->spinBox_Xend->setValue(HeadWindowHighLimit.Position[0]);
    ui->spinBox_Yend->setValue(HeadWindowHighLimit.Position[1]);
    ui->spinBox_Zend->setValue(HeadWindowHighLimit.Position[2]);
}

void MainWindow::on_verticalScrollBar_Znew_valueChanged(int value)
{

}

void MainWindow::on_spinBox_Xnew_editingFinished()
{

}

void MainWindow::on_pushButton_SendToPort_clicked()
{
    ui->textEdit->insertPlainText( ui->lineEdit->displayText().toStdString().c_str() ) ;
    const char * b;
    b = ui->lineEdit->displayText().toStdString().c_str();

    writetoport (usbport, b );
}

void MainWindow::on_verticalScrollBar_Xnew_valueChanged(int value)
{

}

void MainWindow::on_spinBox_Xstart_valueChanged(int arg1)
{
//    Data.Fields[0].SetScanWindowRange(arg1,0,0,0);
}
