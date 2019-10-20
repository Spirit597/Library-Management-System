#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QMessageBox>
#include <QComboBox>

#include <QCloseEvent>

#include <QJsonObject>
#include <QJsonDocument>

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QNetworkInterface>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = 0);

    void dealNewConnection();
    void readDataAndRespond();
    void refreshClientList();

    void closeEvent(QCloseEvent *event);
private:
    QSqlDatabase database;

    QTcpServer *tcpServer;
    QTcpSocket *currentClient;

    QLabel *localIP;
    QLabel *localPort;
    QTextEdit *promptMessage;
    QComboBox *clientList;
    QComboBox *userList;

    QList<QTcpSocket*> tcpClients;
    QList<QString> userIDList;

signals:

public slots:
};

#endif // MAINWIDGET_H
