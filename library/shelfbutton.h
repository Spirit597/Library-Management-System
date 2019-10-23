#ifndef BOOKSHELFBUTTON_H
#define BOOKSHELFBUTTON_H
#include <QString>
#include <QPushButton>
#include <QTcpSocket>
#include <QFile>

class ShelfButton : public QPushButton
{
    Q_OBJECT
public:
    ShelfButton(QWidget *parent = 0);

    void setShelfNumber(int ShelfNumber);
    void sendCreatNewWin();

private:
    int ShelfNumber;


signals:
    void ShelfNumbersignal(int ShelfNumber);
};

#endif // BOOKSHELFBUTTON_H
