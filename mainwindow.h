#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "info.h"
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    info infowidget;
    QTimer *time;
    ~MainWindow();

private slots:
    void on_ButtonSet_clicked();
    void showhome();
    void showVedio();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
