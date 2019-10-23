#include "libraryinfowindow.h"
#include "QStyleFactory"
LibraryInfoWindow::LibraryInfoWindow(QWidget *parent) : QWidget(parent)
{
    this->resize(520, 600);
    this->setWindowTitle("图书馆信息");

    fund = new QLabel(this);
    fund->move(20, 20);
//    //好像不需要返回按钮？先不处理了
//    closeShelfWidget.setParent(this);
//    closeShelfWidget.setText("返回主界面");
//    closeShelfWidget.resize(100,50);
//    closeShelfWidget.move(350,140);
//    closeShelfWidget.show();
//    需要返回时，书架信息系统给主界面发送一个信号
//    connect(&closeShelfWidget, &QPushButton::clicked, this, &LibraryInfoWindow::returnMainWindow);
//    connect(&shelfDetailWidget, &BookShelfDetailWidget::goBackSignal, this, &LibraryInfoWindow::backToShelfList);


    //滑动区域显示书架网格信息
//    shelfListWidget = new QWidget(this);
//    shelfListWidget->resize(520,300);
//    shelfListWidget->setMaximumWidth(500);
//    shelfListWidget->setStyle(QStyleFactory::create("fusion"));
//    shelfListWidget->show();

//    ScrollArea = new QScrollArea(this);
//    ScrollArea->setWidget(shelfListWidget);
//    ScrollArea->setFixedSize(520,300);
//    ScrollArea->move(1,200);


}

//void LibraryInfoWindow::returnMainWindow()
//{
//    emit returnSignal();
//}

//void LibraryInfoWindow::setTcpClient(QTcpSocket *tcpClient)
//{
//    this->tcpClient = tcpClient;
//}

//void LibraryInfoWindow::creteShelfDetailWin(int ShelfNumber)
//{
//    BookShelfDetailWidget *shelfDetailWidget = new BookShelfDetailWidget;
//    shelfDetailWidget->setTcpClient(this->tcpClient);
//    shelfDetailWidget->setWindowTitle(QString::number(ShelfNumber)+"号书架");
//    shelfDetailWidget->getBooksInfo(ShelfNumber);

//}


//上下级窗口切换信号，进入书架详细信息的子菜单，
//其实是我实现方式略显繁琐了，有空再优化
//不过其实也有关系，点开详细信息还能返回书架列表
//void LibraryInfoWindow::enterShelfDetail()
//{
//    this->hide();
//    shelfDetailWidget->show();
//}

//void LibraryInfoWindow::backToShelfList()
//{
//    this->show();
//    shelfDetailWidget->hide();
//}

void LibraryInfoWindow::setFund(float fund)
{
    QString temp = "图书馆经费余额：";
    temp = temp + QString::number(fund, 'f', 2) + "元";
    this->fund->setText(temp);
}

void LibraryInfoWindow::getLibraryInfo(QTcpSocket *tcpClient)
{
    QJsonObject getLibraryInfoPackage;

    getLibraryInfoPackage.insert("type", "get library information");


    QByteArray byte_array = QJsonDocument(getLibraryInfoPackage).toJson();
    tcpClient->write(byte_array);
    if(tcpClient->waitForReadyRead(1000))//阻塞式连接
    {
        QByteArray result = tcpClient->readAll();
        if(!result.isEmpty())
        {
            QJsonObject resultInfo = QJsonDocument::fromJson(result).object();

            this->setFund(resultInfo.value("fund").toDouble());

        }
    }
}

