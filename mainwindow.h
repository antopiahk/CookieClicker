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
    void autoPlay();

    void callGoldenCookie();
    void withdrawGoldenCookie(int gc_num);
    void catchGoldenCookie(int gc_num);

    void evilCheatActivation();
    void goldenCheatActivation();
    void autoPlayCheatActivation();


private:
    bool autoplay = false;
    Ui::MainWindow *ui;
    static const int max_gc_number = 100;

    int cookies = 0;

    int cursors = 0;
    int grandmas = 0;
    int farms = 0;
    int mines = 0;

    int cursor_price = 10;
    int grandma_price = 25;
    int farm_price = 100;
    int mine_price = 1000;

    int cps = 0;
    int created_golden_cookies = 0;
    int caught_golden_cookies = 0;

    int gc_timecheck[max_gc_number];
    int gc_lifetime[max_gc_number];

    QPushButton* golden_cookie;
    QPushButton* golden_cookies[max_gc_number];
    QShortcut* evilCheatShortcut;
    QShortcut* goldenCheatShortcut;
    QShortcut* autoPlayCheatShortcut;
    QTimer* timer = nullptr;
};

#endif // MAINWINDOW_H
