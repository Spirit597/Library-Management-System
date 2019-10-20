#ifndef BOOKSHELFBUTTON_H
#define BOOKSHELFBUTTON_H
#include <QString>
#include <QPushButton>
#include <QTcpSocket>

class ShelfButton : public QPushButton
{
    Q_OBJECT
public:
    ShelfButton(QWidget *parent = 0);

    void setShelfNumber(int ShelfNumber,QTcpSocket *tcpSocket);
    void sendCreatNewWin();
private:
    int ShelfNumber;
    QTcpSocket *tcpSocket;

signals:
    void ShelfNumbersignal(int ShelfNumber, QTcpSocket *tcpSocket);
};

#endif // BOOKSHELFBUTTON_H
