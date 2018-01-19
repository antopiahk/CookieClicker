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
    //connect(this->ui->button_cursor, SIGNAL( clicked() ), this, SLOT(onItemButtonClick()));
    //connect(this->ui->button_grandma, SIGNAL( clicked() ), this, SLOT(onItemButtonClick()));
    //connect(this->ui->button_farm, SIGNAL( clicked() ), this, SLOT(onItemButtonClick()));
    //connect(this->ui->button_mine, SIGNAL( clicked() ), this, SLOT(onItemButtonClick()));
    this->ui->button_cursor->installEventFilter(this);
    this->ui->button_grandma->installEventFilter(this);
    this->ui->button_farm->installEventFilter(this);
    this->ui->button_mine->installEventFilter(this);

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

bool MainWindow::eventFilter(QObject* target, QEvent* event) {
    if (event->type() == QEvent::MouseButtonPress) {
        auto mouse_event = (QMouseEvent*)event;
        QPushButton* buttonSender;
        if (mouse_event->button() & Qt::RightButton) {
            QPushButton* buttonSender;
            printf("right\n");
            if (target == this->ui->button_cursor) {
                buttonSender = this->ui->button_cursor;
            } else if (target == this->ui->button_grandma) {
                buttonSender = this->ui->button_grandma;
            } else if (target == this->ui->button_farm) {
                buttonSender = this->ui->button_farm;
            } else {
                buttonSender = this->ui->button_mine;
            }
            this->onItemButtonClick(buttonSender, true);
            return true;
        } else if (mouse_event->button() & Qt::LeftButton) {
            if (target == this->ui->button_cursor) {
                buttonSender = this->ui->button_cursor;
            } else if (target == this->ui->button_grandma) {
                buttonSender = this->ui->button_grandma;
            } else if (target == this->ui->button_farm) {
                buttonSender = this->ui->button_farm;
            } else {
                buttonSender = this->ui->button_mine;
            }
            this->onItemButtonClick(buttonSender, false);
            return true;
        }
    }
    return false;
}


void MainWindow::onItemButtonClick(QPushButton* buttonSender, bool rightclick) {
    QString name;
    int* item_num;
    int *item_price;
    if (buttonSender == nullptr) {
        buttonSender = qobject_cast<QPushButton*>(sender());
    }
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
