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
    shelfListWidget = new QWidget(this);
    shelfListWidget->resize(520,300);
    shelfListWidget->setMaximumWidth(500);
    shelfListWidget->setStyle(QStyleFactory::create("fusion"));
    shelfListWidget->show();

    ScrollArea = new QScrollArea(this);
    ScrollArea->setWidget(shelfListWidget);
    ScrollArea->setFixedSize(520,300);
    ScrollArea->move(1,200);

}

void LibraryInfoWindow::returnMainWindow()
{
    emit returnSignal();
}

void LibraryInfoWindow::setTcpClient(QTcpSocket *tcpClient)
{
    this->tcpClient = tcpClient;
}

void LibraryInfoWindow::creteShelfDetailWin(int ShelfNumber)
{
    BookShelfDetailWidget *shelfDetailWidget = new BookShelfDetailWidget;
    shelfDetailWidget->setTcpClient(this->tcpClient);
    shelfDetailWidget->setWindowTitle(QString::number(ShelfNumber)+"号书架");
    shelfDetailWidget->getBooksInfo(ShelfNumber);

}


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


void LibraryInfoWindow::getShelfInfo()
{

    for(int i = 0; i<shelfButtonLists.count();i++ )
    {
        if(shelfButtonLists[i])
        {
            delete shelfButtonLists[i];
        }
    }
    shelfButtonLists.clear();
    shelfLists.clear();
    shelfListWidget->resize(shelfListWidget->width(),0);
    QJsonObject getShelfsInfoPackage;
    getShelfsInfoPackage.insert("type","get shelfs information");

    tcpClient = this->tcpClient;
    QByteArray byte_array = QJsonDocument(getShelfsInfoPackage).toJson();
    tcpClient->write(byte_array);
    if(tcpClient->waitForReadyRead(1000))//阻塞式连接
    {
        QByteArray result = tcpClient->readAll();
        if(!result.isEmpty())
        {
            QJsonObject resultInfo = QJsonDocument::fromJson(result).object();
            for(int i = 1; i <=resultInfo.count() ; i++)
            {
                QJsonObject temp = resultInfo.value("shelfs" + QString::number(i)).toObject();
                ShelfButton *singleShelfButton = new ShelfButton(shelfListWidget);
                singleShelfButton->show();
                shelfButtonLists.append(singleShelfButton);
                /*
                 * 注意发送信号的过程：
                 * 单个书架按钮被点击->
                 * 按钮发出一个带书架编号参数的信号->
                 * 书架列表窗口接收信号，创建该书架的详细信息窗口
                 */
                connect(singleShelfButton, &ShelfButton::clicked, singleShelfButton, &ShelfButton::sendCreatNewWin);
                connect(singleShelfButton, &ShelfButton::ShelfNumbersignal, this,&LibraryInfoWindow::creteShelfDetailWin);

                int ShelfNumber = temp.value("ShelfNumber").toInt();
                QString ShelfType = temp.value("ShelfType").toString();
                int ShelfCapacity = temp.value("ShelfCapacity").toInt();

                singleShelfButton->setShelfNumber(ShelfNumber);
                QString strshelfnumber = QString::number(ShelfNumber);
                QString strshelfcapacity = QString::number(ShelfCapacity);
                singleShelfButton->setText("书架号："+strshelfnumber+"\n存放书类："+ShelfType+"\n容量："+strshelfcapacity);
                int row = (((i-1) / 3) + 1);
                int column = ((i - 1) % 3) + 1;
                singleShelfButton->move((column-1)*(singleShelfButton->width()+20),(row-1) * (singleShelfButton->height()+20));
                if (i%3 == 1)
                {
                    shelfListWidget->resize(shelfListWidget->width(),shelfListWidget->height()+row*(singleShelfButton->height()+20));
                }

                BookShelf *singleShelf = new BookShelf(ShelfNumber,ShelfType,ShelfCapacity);
                shelfLists.append(singleShelf);
            }

        }
        this->shelfListWidget->show();
    }


}
