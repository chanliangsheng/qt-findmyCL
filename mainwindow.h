#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <mzml.h>
#include <QTime>
#include <database.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public:
    Mzml m_mzml;//mzml文件类
    Database m_database;//数据库类
};
#endif // MAINWINDOW_H
