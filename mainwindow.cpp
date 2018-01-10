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
    connect(this->ui->button_cookie, SIGNAL( clicked() ), this, SLOT(cookieAdd()));
    connect(this->ui->button_cursor, SIGNAL( clicked() ), this, SLOT(getSomeItems()));
    connect(this->ui->button_grandma, SIGNAL( clicked() ), this, SLOT(getSomeItems()));
    connect(this->ui->button_farm, SIGNAL( clicked() ), this, SLOT(getSomeItems()));
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(getAutoCookies()));
    timer->start(1000); //time specified in ms

}

MainWindow::~MainWindow()
{
    delete timer;
    delete ui;
}

void MainWindow::getSomeItems() {
    QString name;
    int *item;
    int *item_price;
    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    if (buttonSender == this->ui->button_cursor) {
        name = "Cursor";
        item = &cursors;
        item_price = &cursor_price;
    } else if (buttonSender == this->ui->button_grandma) {
        name = "Grandma";
        item = &grandmas;
        item_price = &grangma_prise;
    } else if (buttonSender == this->ui->button_farm) {
        name = "Farm";
        item = &farms;
        item_price = &grangma_prise;
    }
    int repeat = 1;
    if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
        repeat = 10;
    } else if (QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier)) {
        repeat = 100;
    }
    for (int i = 0; i < repeat; i++) {
        itemAdd(buttonSender, name, item, item_price);
    }
}

void MainWindow::cookieAdd() {
    cookies++;
}

void MainWindow::itemAdd(QPushButton*button_sender, QString name, int* item, int* item_price){
    if (cookies >= *item_price) {
        *item = *item + 1;
        cookies -= *item_price;
        *item_price += ceil(*item_price * 0.05);
        button_sender->setText("Buy " + name + ": " + QString::fromStdString(std::to_string(*item_price)));
    }
}

void MainWindow::getAutoCookies(){
    //printf("Some cookies; timer=%p\n", timer); fflush(stdout);
    int cookies_per_second = cursors + 5*grandmas + 10*farms;
    cookies = cookies + cookies_per_second;
    ui->label->setText("Cookies: " +
                        QString::fromStdString(std::to_string(cookies)) +
                       "\nCursors: " +
                       QString::fromStdString(std::to_string(cursors)) +
                       "\nGrandmas: " +
                       QString::fromStdString(std::to_string(grandmas)) +
                       "\nFarms: " +
                       QString::fromStdString(std::to_string(farms)) +
                       "\nCookies per second: " +
                       QString::fromStdString(std::to_string(cookies_per_second)));
    timer->start(1000); //time specified in ms
}
