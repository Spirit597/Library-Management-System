#include "bookshelfdetailwidget.h"

BookShelfDetailWidget::BookShelfDetailWidget(QWidget *parent) : QWidget(parent)
{

//    this->setWindowTitle("书架详细信息");
    this->resize(800,600);


    //既然已经不在书架列表窗口的类中声明了，也就没必要再设置这个按钮了
    //即使这个窗口关闭，书架列表也依旧在
//    closeShelfDetail.setParent(this);
//    closeShelfDetail.setText("返回书架列表");
//    closeShelfDetail.resize(100,50);
//    closeShelfDetail.move(650,500);
//    closeShelfDetail.show();
//    //返回书架列表窗口，给书架列表窗口发送一个信号
//    connect(&closeShelfDetail, &QPushButton::clicked, this, &BookShelfDetailWidget::dealBackSignal);


    bookListWidget = new QWidget;
    bookListWidget->setMinimumWidth(780);
    bookListWidget->resize(780,0);
    bookListWidget->show();

    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(bookListWidget);
    scrollArea->move(0,150);
    scrollArea->setFixedSize(800,350);


}


void BookShelfDetailWidget::dealBackSignal()
{
    //广播的形式发送信号
    emit goBackSignal();
}

void BookShelfDetailWidget::setTcpClient(QTcpSocket *tcpClient)
{
    this->tcpClient = tcpClient;
}

//根据书架号搜索书架上的所有书籍
//这样传参可以，但是每次都要传这个tcpClient很麻烦
//是不是可以直接用widget声明时传到子类部件里？
void BookShelfDetailWidget::getBooksInfo(int byShelfNumber)
{
//    BookShelfDetailWidget *shelfDetailWidget = new BookShelfDetailWidget();
//    connect(shelfDetailWidget, &BookShelfDetailWidget::shelfNumber);
//    connect 留待按钮绑定

    for(int i = 0; i<bookButtonLists.count();i++)
    {
        if(bookButtonLists[i])
        {
            delete bookButtonLists[i];
        }

    }
    bookButtonLists.clear();
    bookLists.clear();
    bookListWidget->resize(bookListWidget->width(),0);

    QJsonObject getBooksInfoPackage;
    getBooksInfoPackage.insert("type", "get books by shelfnumber");
    getBooksInfoPackage.insert("shelfnumber",QString::number(byShelfNumber,10));

    QByteArray byte_array = QJsonDocument(getBooksInfoPackage).toJson();
    tcpClient->write(byte_array);
    if(tcpClient->waitForReadyRead(1000))//阻塞式连接
    {
        QByteArray result = tcpClient->readAll();
        if(!result.isEmpty())
        {
            QJsonObject resultInfo = QJsonDocument::fromJson(result).object();

            for(int i=1; i<=resultInfo.count(); i++)
            {
                QJsonObject temp = resultInfo.value("book" + QString::number(i)).toObject();
                BookButton *oneBookButton = new BookButton(bookListWidget);
                oneBookButton->show();
                bookButtonLists.append(oneBookButton);
                //这里的书籍按钮就不点开了，仅供浏览？
//                connect(oneBookButton, &BookButton::clicked, oneBookButton, &BookButton::sendSignal);
//                connect(oneBookButton, &BookButton::ISBNsignal, this, &MainWindow::createBookDetailWin);

                QString ISBN = temp.value("ISBN").toString();
                QString name = temp.value("name").toString();
                QString writer = temp.value("writer").toString();
                QString type = temp.value("type").toString();
                QString press = temp.value("press").toString();
                QString publicationDate = temp.value("publicationDate").toString();
                float price = temp.value("price").toDouble();
                int bookShelf = temp.value("bookShelf").toInt();

                oneBookButton->setISBN(ISBN);
                oneBookButton->setText("ISBN：" + ISBN + "\n书名：" + name + "\n作者：" + writer + "\n类别：" + type + "\n出版社：" + press);
                oneBookButton->move(0, (i-1) * oneBookButton->height());
                bookListWidget->resize(bookListWidget->width(), bookListWidget->height() + oneBookButton->height());

                Book *oneBook = new Book(ISBN, name, writer, type, press, publicationDate, price, bookShelf);
                bookLists.append(oneBook);
            }
            this->bookListWidget->show();
            this->show();
        }

    }
}
