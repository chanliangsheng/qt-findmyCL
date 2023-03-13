#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <database.h>
#include <ms1match.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTime timedebuge;//声明一个时钟对象
    timedebuge.start();//开始计时

    Ms1Match ms1match;

    this->m_database.LoadAllTable();

    this->m_mzml.ReadMzml("D:/mzml/NEG_ID_01.mzML");


    ms1match.MatchMs1WithAllTables(this->m_mzml.m_ms1_vector , this->m_database);

    qDebug()<<"第一段程序耗时："<<timedebuge.elapsed()/1000.0<<"s";//输出计时





}

MainWindow::~MainWindow()
{
    delete ui;
}

