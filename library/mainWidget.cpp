#include "mainWidget.h"


MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    tcpClient = new QTcpSocket(this);
    //qDebug() << "tcpClient1:" << tcpClient;
    mainWin = new MainWindow;
    //mainWin->show();
    mainWin->setTcpClient(tcpClient);
    connect(mainWin, &MainWindow::logoutSignal, this, &MainWidget::dealLogout);

    loginWindow = new LoginWidget;
    loginWindow->show();

    connect(loginWindow, &LoginWidget::loginClicked, this, &MainWidget::loginJudge);

    connect(this, &MainWidget::currentUserInfo, mainWin, &MainWindow::upDateCurrentUser);

}

void MainWidget::loginJudge(QString ID, QString password, QString role)
{
    tryTcpConnect();
    QJsonObject loginPackage;
    loginPackage.insert("type", "login");
    loginPackage.insert("role", role);

    loginPackage.insert("id",ID);
    loginPackage.insert("password",password);
    QByteArray byte_array = QJsonDocument(loginPackage).toJson();
    tcpClient->write(byte_array);
    if(tcpClient->waitForReadyRead(1000))//阻塞式连接
    {
        QByteArray loginResult = tcpClient->readAll();
        if(!loginResult.isEmpty())
        {
            QJsonObject loginResultInfo = QJsonDocument::fromJson(loginResult).object();
            if(loginResultInfo.value("result").toInt() == 1)
            {
                emit currentUserInfo(ID, role);
                loginWindow->hide();
                mainWin->changeButton(role);
                mainWin->getBooksInfo();
                mainWin->show();

            }
            else if(loginResultInfo.value("result").toInt() == 2)
            {
                QMessageBox::information(NULL,QString("登陆失败"),QString("您输入的密码错误"),QMessageBox::Ok);
            }
            else if(loginResultInfo.value("result").toInt() == 3)
            {
                QMessageBox::information(NULL,QString("登陆失败"),QString("您输入的账号不存在"),QMessageBox::Ok);
            }
            else if(loginResultInfo.value("result").toInt() == 4)
            {
                QMessageBox::information(NULL,QString("登陆失败"),QString("该账号已在别处登录"),QMessageBox::Ok);
            }
        }
        else
        {
            qDebug() << "loginResult is empty";
        }
    }
    else
    {
        qDebug() << "等待服务器的登录响应超时";
    }

}

void MainWidget::tryTcpConnect()
{
    tcpClient->abort();
    //tcpClient->connectToHost("10.30.129.96", 8000);
    tcpClient->connectToHost("127.0.0.1",8000);
    if(!tcpClient->waitForConnected(1000))
    {
        QMessageBox::StandardButton reminder = QMessageBox::question(NULL, "Network Error", "与服务器连接失败，您想要尝试重新连接吗",
                                                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (reminder == QMessageBox::Yes)
        {
            tryTcpConnect();
        }
    }
}

void MainWidget::dealLogout()
{
    mainWin->hide();
    loginWindow->clearPasswordLineEdit();
    loginWindow->show();
}


