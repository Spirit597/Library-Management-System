#include <QApplication>
#include "mainWidget.h"
#include "QStyleFactory"
#include <QFile>
int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QFile qssFile(":/qss/silvery.css");
    qssFile.open(QIODevice::ReadOnly);
    app.setStyleSheet(qssFile.readAll());
    MainWidget bottom;
    return app.exec();
}
