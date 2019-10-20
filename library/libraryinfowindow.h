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
    void returnMainWindow();
    void getShelfInfo();
    void creteShelfDetailWin(int ShelfNumber);
    void enterShelfDetail();//进入书架信息的处理函数
    void backToShelfList();//返回书架列表的处理函数
    QTcpSocket *tcpClient;
private:
    QLabel *fund;
    QPushButton closeShelfWidget;
    BookShelfDetailWidget *shelfDetailWidget;
    QWidget *shelfListWidget;
    QScrollArea *ScrollArea;
    QVector <BookShelf *> shelfLists;
    QVector <ShelfButton *> shelfButtonLists;




signals:
    void returnSignal();
    void shelfNumeberSignal(QString shelfNumber);

public slots:
};

#endif // LIBRARYINFOWINDOW_H
