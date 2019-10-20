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
    void getBooksInfo(int byShelfNumber);
    QTcpSocket *tcpClient;
private:
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
