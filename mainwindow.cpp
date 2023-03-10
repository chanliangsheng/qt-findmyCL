#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <database.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTime timedebuge;//声明一个时钟对象
    timedebuge.start();//开始计时


    this->m_database.LoadAllTable();


    qDebug()<<"第一段程序耗时："<<timedebuge.elapsed()/1000.0<<"s";//输出计时





}

MainWindow::~MainWindow()
{
    delete ui;
}

