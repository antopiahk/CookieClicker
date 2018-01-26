#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QMessageBox>
#include <QShortcut>
#include <QTextStream>
#include <QTime>
#include <QTimer>
#include <QShortcut>
#include <cmath>
#include <ctime>
#include <cstdio>
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    srand(time(NULL));

    srand(time(NULL));


    cheatShorcut = new QShortcut(QKeySequence(tr("Ctrl+Shift+Alt+T")), this);

    connect(this->cheatShorcut, SIGNAL(activated()), this, SLOT(cheatActivation()));
    connect(this->ui->button_cookie, SIGNAL( clicked() ), this, SLOT(addCookie()));
    this->ui->button_cursor->installEventFilter(this);
    this->ui->button_grandma->installEventFilter(this);
    this->ui->button_farm->installEventFilter(this);
    this->ui->button_mine->installEventFilter(this);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTick()));
    setGameStatus();
    timer->start(1000); //time specified in ms

}

MainWindow::~MainWindow()
{
    delete timer;
    delete ui;
}

void MainWindow::cheatActivation() {
    if (grandmas != 0) {
        cookies +=grandmas*1000000;
        grandmas = 0;
        grandma_price = 25;
        this->ui->button_grandma->setText("Buy Grandma : " + QString::fromStdString(std::to_string(grandma_price)));
        this->ui->label->setStyleSheet("QLabel { background-color : red; color : white; }");
        cps = cursors + 5*grandmas + 10*farms + 50*mines;
        setGameStatus();
    }

}

void MainWindow::timerTick() {
    golden_cookie_timecheck ++;
    getAutoCookies();
    int random = rand() % 50;
    //printf("%d", random);
    if (random == 0 && golden_cookie == nullptr) {
        callGoldenCookie();
    }
    if (golden_cookie_timecheck >= golden_cookie_lifetime &&
            golden_cookie != nullptr) {
        withdrawGoldenCookie();
    }
    timer->start(1000); //time specified in ms
}

bool MainWindow::eventFilter(QObject* target, QEvent* event) {
    auto mouse_event = (QMouseEvent*)event;
    if (event->type() == QEvent::MouseButtonPress &&
            mouse_event->button() & Qt::LeftButton &&
            target == golden_cookie) {
        catchGoldenCookie();
        return true;
    }
    if (event->type() == QEvent::MouseButtonPress) {
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

void MainWindow::callGoldenCookie() {
    const int gc_width = 80;
    const int gc_height = 40;
    QSize sz = size();

    golden_cookie_timecheck = 0;
    golden_cookie_lifetime = 3 + rand() % 8;

    int x = rand()%(sz.width() - gc_width);
    int y = rand()%(sz.height() - gc_height);


    golden_cookie = new QPushButton(ui->centralWidget);
    golden_cookie->setObjectName(QStringLiteral("golden_cookie"));
    golden_cookie->setText(QStringLiteral("Eat me!"));
    golden_cookie->setGeometry(QRect(x, y, gc_width, gc_height));
    golden_cookie->show();
    golden_cookie->installEventFilter(this);
}

void MainWindow::withdrawGoldenCookie() {
    delete golden_cookie;
    golden_cookie = nullptr;
}

void MainWindow::catchGoldenCookie(){
    golden_cookies++;
    int random = rand() % 50 + 1;
    int prize = ceil(cookies * 0.01*random + 1);
    if (prize == 1) {
        QMessageBox::information(
            this,
            tr("Golden cookie"),
            tr("You got 1 cookie!") );
    } else {
        QMessageBox::information(
            this,
            tr("Golden cookie"),
            QString("You got %1 cookies!").arg(prize) );
    }
    cookies = cookies + prize;
    withdrawGoldenCookie();
}

void MainWindow::getAutoCookies(){
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
                       "\nCpS: " +
                       QString::fromStdString(std::to_string(cps)) +
                       "\nGolden cookies caught: " +
                       QString::fromStdString(std::to_string(golden_cookies)));
}
