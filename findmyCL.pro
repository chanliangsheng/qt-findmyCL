QT       += core gui
QT += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    base64.cpp \
    database.cpp \
    databaserecord.cpp \
    main.cpp \
    mainwindow.cpp \
    ms1.cpp \
    ms1match.cpp \
    ms2.cpp \
    mzml.cpp \
    singlems1match.cpp

HEADERS += \
    base64.h \
    database.h \
    databaserecord.h \
    mainwindow.h \
    ms1.h \
    ms1match.h \
    ms2.h \
    mzml.h \
    singlems1match.h

FORMS += \
    mainwindow.ui

# 这是为了导入头文件路径和动态库的路径还有动态库的名字
INCLUDEPATH += 3rdparty/MyLibary/include
LIBS += -L $$PWD/3rdparty/MyLibary/lib -l tinyxml2
LIBS += -lz

# 定义一个变量
MY_PWD = $$PWD
OUTPUT_PWD = $$OUT_PWD
# 将变量MY_PWD中的正斜杠替换为反斜杠
MY_PWD_WIN = $$replace(MY_PWD, /, \\)
OUTPUT_PWD_WIN = $$replace(OUTPUT_PWD, /, \\)

QMAKE_PRE_LINK = xcopy $$MY_PWD_WIN\3rdparty\MyLibary\lib\tinyxml2.dll $$OUTPUT_PWD_WIN\debug /y

# 复制db文件夹到编译后项目中
MY_DB_PWD = $$MY_PWD_WIN\db
OUT_DB_PWD = $$OUTPUT_PWD_WIN\db

copy_db = $$system("echo d| xcopy $${MY_DB_PWD} $${OUT_DB_PWD}")

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
