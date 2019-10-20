#include <QApplication>
#include "mainWidget.h"
#include "QStyleFactory"
int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    MainWidget bottom;
    return app.exec();
}
