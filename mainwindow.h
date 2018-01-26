#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QPushButton>
#include <QMouseEvent>
#include <QShortcut>
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
    //void mousePressEvent(QMouseEvent *event);
    //void mouseReleaseEvent(QMouseEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
    void timerTick();
    void setGameStatus();

    void onItemButtonClick(QPushButton *buttonSender = nullptr, bool rightclick = false);

    bool getItem(QPushButton*button_sender, QString name, int *item, int *item_price);
    bool sellItem(QPushButton*button_sender, QString name, int *item, int *item_price);
    void addCookie();
    void getAutoCookies();

    void callGoldenCookie();
    void withdrawGoldenCookie();
    void catchGoldenCookie();

    void cheatActivation();
    void goldenCookie();
    void callGoldenCookie();
    void withdrawGoldenCookie();


private:
    bool rightclick = false;
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

    int seconds = 0;
    int cps = 0;
    int golden_cookies = 0;

    int golden_cookie_time_called = 0;
    int golden_cookie_lifetime = 0;

    QPushButton* golden_cookie = nullptr;
    QShortcut* cheatShorcut;
    QTimer* timer = nullptr;
    QPushButton* button_goldencookie = nullptr;
    int timechecker = 0;
    int gclifetime;
};

#endif // MAINWINDOW_H
