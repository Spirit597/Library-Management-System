#ifndef RETURNBOOKWINDOW_H
#define RETURNBOOKWINDOW_H

#include <QWidget>
#include <QScrollArea>
#include <QVector>

#include "returnbookwidget.h"

class ReturnBookWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ReturnBookWindow(QWidget *parent = 0);

    void getBorrowedBookInfo(QString ID, QTcpSocket *tcpClient);
    void refreshReturnBookWin(int index);
private:
    QScrollArea *scrollArea;
    QWidget *borrowedBookListWidget;

    int bookNum = 0;
    QLabel *bookNumLabel;
    QVector <ReturnBookWidget *> returnBookWidgets;

signals:

public slots:
};

#endif // RETURNBOOKWINDOW_H
