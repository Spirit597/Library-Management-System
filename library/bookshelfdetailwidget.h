#ifndef BOOKSHELFDETAILWIDGET_H
#define BOOKSHELFDETAILWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <book.h>
#include <bookbutton.h>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
class BookShelfDetailWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BookShelfDetailWidget(QWidget *parent = 0);
    void dealBackSignal();
    void creteShelfDetailWin(int ShelfNumber,QTcpSocket *tcpSocket);
    void getBooksInfo(int ShelfNumber,QTcpSocket *tcpSocket);

    QWidget *bookListWidget;
    QScrollArea *scrollArea;

    QPushButton closeShelfDetail;

    QLabel *shelfCapacity;
    QVector <BookButton *> bookButtonLists;
    QVector <Book*> bookLists;
signals:
    void goBackSignal();

public slots:
};

#endif // BOOKSHELFDETAILWIDGET_H
