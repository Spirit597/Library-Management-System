#ifndef MainWidget_H
#define MainWidget_H

#include <QWidget>
#include <QMessageBox>

#include <QJsonObject>
#include <QJsonDocument>

#include <QTcpSocket>
#include <QHostAddress>

#include "loginwidget.h"
#include "mainWindow.h"

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = 0);

    void loginJudge(QString ID, QString password, QString role);
    void tryTcpConnect();

    void dealLogout();

private:
    LoginWidget *loginWindow;

    MainWindow *mainWin;

    QTcpSocket *tcpClient;



signals:
    void currentUserInfo(QString ID, QString role);

public slots:
};

#endif // MainWidget_H
