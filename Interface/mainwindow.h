#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_spinBox_Xnew_valueChanged(int arg1);

    void on_spinBox_Xnew_editingFinished();

    void on_spinBox_Ynew_valueChanged(int arg1);

    void on_spinBox_Znew_valueChanged(int arg1);

    void on_verticalScrollBar_Znew_valueChanged(int value);

    void on_verticalScrollBar_Xnew_sliderMoved(int position);

    void on_verticalScrollBar_Ynew_sliderMoved(int position);

    void on_verticalScrollBar_Znew_sliderMoved(int position);

    void on_pushButton_SendToPort_clicked();

    void on_verticalScrollBar_Xnew_valueChanged(int value);

    void on_spinBox_Xstart_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
