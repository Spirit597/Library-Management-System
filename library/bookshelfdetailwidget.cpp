#include "bookshelfdetailwidget.h"

BookShelfDetailWidget::BookShelfDetailWidget(QWidget *parent) : QWidget(parent)
{

    this->setWindowTitle("书架详细信息");
    this->resize(800,600);

    closeShelfDetail.setParent(this);
    closeShelfDetail.setText("返回书架列表");
    closeShelfDetail.resize(100,50);
    closeShelfDetail.move(650,500);
    closeShelfDetail.show();
    //返回书架列表窗口，给书架列表窗口发送一个信号
    connect(&closeShelfDetail, &QPushButton::clicked, this, &BookShelfDetailWidget::dealBackSignal);


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



void BookShelfDetailWidget::getBooksInfo(int ShelfNumber, QTcpSocket *tcpSocket)
{
////    BookShelfDetailWidget *shelfDetailWidget = new BookShelfDetailWidget();
////    connect(shelfDetailWidget, &BookShelfDetailWidget::shelfNumber);
////    connect 留待按钮绑定
////    connect
//    for(int i = 0; i<bookButtonLists.count();i++)
//    {
//        if(bookButtonLists[i])
//        {
//            delete bookButtonLists[i];
//        }

//    }
//    bookButtonLists.clear();
//    bookLists.clear();
//    bookListWidget->resize(bookListWidget->width(),0);

//    QJsonObject getBooksInfoPackage;
//    getBooksInfoPackage.insert("type", "get books");

//    QByteArray byte_array = QJsonDocument(getBooksInfoPackage).toJson();
//    tcpClient->write(byte_array);


//    for(int i = 1; i < 10; i++)
//    {
//        BookButton *oneBookButton = new BookButton(bookListWidget);
//        oneBookButton->show();
//        bookButtonLists.append(oneBookButton);

//        oneBookButton->setText("ISBN：\n书名：\n作者：\n类别：\n出版社：" );
////        留待数据库文件参与后使用
////        oneBookButton->setText("ISBN：" + + "\n书名：" + name + "\n作者：" + writer + "\n类别：" + type + "\n出版社：" + press);
//        oneBookButton->move(0, (i-1) * oneBookButton->height());
//        bookListWidget->resize(bookListWidget->width(), bookListWidget->height() + oneBookButton->height());

////        留待数据库文件参与后使用
////        Book *oneBook = new Book(ISBN, name, writer, type, press, publicationDate, price);
//        Book *oneBook = new Book("ISBN"," name", "writer", "type", "press", "publicationDate", 1.1);
//        bookLists.append(oneBook);
//    }

//    this->bookListWidget->show();
}
