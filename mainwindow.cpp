#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
#include <QFile>
#include <QTime>
#include <QTimer>
#include <cstdio>
#include <cmath>
//#include "timer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this->ui->button_cookie, SIGNAL( clicked() ), this, SLOT(addCookie()));
    /*connect(this->ui->button_cursor, SIGNAL( clicked() ), this, SLOT(mousePressEvent(QMouseEvent*)));
    connect(this->ui->button_grandma, SIGNAL( clicked() ), this, SLOT(mousePressEvent(QMouseEvent*)));
    connect(this->ui->button_farm, SIGNAL( clicked() ), this, SLOT(mousePressEvent(QMouseEvent*)));
    connect(this->ui->button_mine, SIGNAL( clicked() ), this, SLOT(mousePressEvent(QMouseEvent*)));*/
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(getAutoCookies()));
    setGameStatus();
    timer->start(1000); //time specified in ms

}

MainWindow::~MainWindow()
{
    delete timer;
    delete ui;
}

void QPushButton::mousePressEvent(QMouseEvent *event) {
    this->onItemButtonClick(event);
}

void MainWindow::onItemButtonClick(QMouseEvent *event) {
    //mousePressEvent(event);
    bool rightclick;
    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    if (event->button() == Qt::RightButton) {
        rightclick = true;
    }
    QString name;
    int* item_num;
    int *item_price;
    if (buttonSender == this->ui->button_cursor) {
        name = "Cursor";
        item_num = &cursors;
        item_price = &cursor_price;
    } else if (buttonSender == this->ui->button_grandma) {
        name = "Grandma";
        item_num = &grandmas;
        item_price = &grandma_price;
    } else if (buttonSender == this->ui->button_farm) {
        name = "Farm";
        item_num = &farms;
        item_price = &farm_price;
    } else if (buttonSender == this->ui->button_mine) {
        name = "Mine";
        item_num = &mines;
        item_price = &mine_price;
    }
    int repeat = 1;
    if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
        repeat = 10;
    } else if (QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier)) {
        repeat = 100;
    }
    bool transaction_finished = true;
    if (rightclick){
        for (int i = 0; i < repeat && transaction_finished; i++) {
            transaction_finished = sellItem(buttonSender, name, item_num, item_price);
        };
    } else {
        for (int i = 0; i < repeat && transaction_finished; i++) {
            transaction_finished = getItem(buttonSender, name, item_num, item_price);
        }
    }
}

void MainWindow::addCookie() {
    cookies++;
}

bool MainWindow::getItem(QPushButton*button_sender, QString name, int* item_num, int* item_price){
    if (cookies >= *item_price) {
        *item_num = *item_num + 1;
        cookies -= *item_price;
        *item_price += ceil(*item_price * 0.05);
        button_sender->setText("Buy " + name + ": " + QString::fromStdString(std::to_string(*item_price)));
        cps = cursors + 5*grandmas + 10*farms + 50*mines;
        setGameStatus();
        return true;
    }
    return false;
}

bool MainWindow::sellItem(QPushButton*button_sender, QString name, int* item_num, int* item_price){
    if (*item_num>0){
        *item_num = *item_num - 1;
        cookies += ceil(*item_price/2.0);
        *item_price = ceil(*item_price * 0.952380952381); // 1/1.05
        button_sender->setText("Buy " + name + ": " + QString::fromStdString(std::to_string(*item_price)));
        cps = cursors + 5*grandmas + 10*farms + 50*mines;
        setGameStatus();
        return true;
    }
    return false;

}

void MainWindow::getAutoCookies(){
    //printf("Some cookies; timer=%p\n", timer); fflush(stdout);
    cookies = cookies + cps;
    setGameStatus();
    timer->start(1000); //time specified in ms
}

void MainWindow::setGameStatus(){
    ui->label->setText("Cookies: " +
                        QString::fromStdString(std::to_string(cookies)) +
                       "\nCursors: " +
                       QString::fromStdString(std::to_string(cursors)) +
                       "\nGrandmas: " +
                       QString::fromStdString(std::to_string(grandmas)) +
                       "\nFarms: " +
                       QString::fromStdString(std::to_string(farms)) +
                       "\nMines: " +
                       QString::fromStdString(std::to_string(mines)) +
                       "\nCookies per second: " +
                       QString::fromStdString(std::to_string(cps)));
}
