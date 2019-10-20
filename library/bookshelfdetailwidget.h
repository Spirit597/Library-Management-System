#ifndef BOOKSHELFDETAILWIDGET_H
#define BOOKSHELFDETAILWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>

#include <book.h>
#include <bookbutton.h>

class BookShelfDetailWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BookShelfDetailWidget(QWidget *parent = 0);
    void dealBackSignal();
    void getBooksInfo(int byShelfNumber);//根据书架号获取书架上的书藉
    void setTcpClient(QTcpSocket *tcpClient);//设定此窗口的连接client,实际上是变相的传参数
private:
    QWidget *bookListWidget;
    QScrollArea *scrollArea;
    QPushButton closeShelfDetail;

    QLabel *shelfCapacity;
    QVector <BookButton *> bookButtonLists;
    QVector <Book*> bookLists;
    QTcpSocket *tcpClient;

signals:
    void goBackSignal();

public slots:
};

#endif // BOOKSHELFDETAILWIDGET_H
