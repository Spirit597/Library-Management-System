#ifndef BOOKBUTTON_H
#define BOOKBUTTON_H

#include <QPushButton>
#include <QFile>

class BookButton : public QPushButton
{
     Q_OBJECT
public:
    BookButton(QWidget *parent = 0);

    void setISBN(QString ISBN);
    void loadCss();


    QString getISBN();


    void sendSignal();
private:
    QString ISBN;

signals:
    void ISBNsignal(QString ISBN);

};

#endif // BOOKBUTTON_H
