#ifndef LIBRARYINFOWINDOW_H
#define LIBRARYINFOWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QPushButton>
#include <QJsonObject>
#include <QJsonDocument>

#include <QTcpSocket>
#include <QVector>
#include <shelfbutton.h>
#include <bookshelf.h>
#include <bookshelfdetailwidget.h>

class LibraryInfoWindow : public QWidget
{
    Q_OBJECT
public:
    explicit LibraryInfoWindow(QWidget *parent = 0);
    void setFund(float fund);
    void getLibraryInfo(QTcpSocket *tcpClient);
//    void returnMainWindow();
//    void getShelfInfo();//获取所有书架列表的函数
//    void creteShelfDetailWin(int ShelfNumber);//获取一特定书架的详细信息的触发函数
//    void setTcpClient(QTcpSocket *tcpClient);//设定此窗口的连接client,实际上是变相的传参数

//    void enterShelfDetail();//进入书架信息的处理函数
//    void backToShelfList();//返回书架列表的处理函数
private:
    QLabel *fund;
//    QPushButton closeShelfWidget;
//    //既然这里不需要hide了，也可以不用在这里定义这个widget;先预留着
////    BookShelfDetailWidget *shelfDetailWidget;
//    QWidget *shelfListWidget;
//    QScrollArea *ScrollArea;
//    QVector <BookShelf *> shelfLists;
//    QVector <ShelfButton *> shelfButtonLists;
//    QTcpSocket *tcpClient;

signals:
    void returnSignal();
//    void shelfNumeberSignal(int shelfNumber);

public slots:
};

#endif // LIBRARYINFOWINDOW_H
