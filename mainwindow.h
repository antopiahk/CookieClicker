#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QPushButton>
//#include "timer.h"

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
    void getNewItems();
    void addCookie();
    void addItem(QPushButton*button_sender, QString name, int *item, int *item_price);
    void getAutoCookies();
    void setGameStatus();

private:
    Ui::MainWindow *ui;
    int cookies = 0;
    int cursors = 0;
    int grandmas = 0;
    int farms = 0;
    int mines = 0;
    int cursor_price = 10;
    int grandma_price = 25;
    int farm_price = 100;
    int mine_price = 1000;
    QTimer* timer = nullptr;
};

#endif // MAINWINDOW_H
