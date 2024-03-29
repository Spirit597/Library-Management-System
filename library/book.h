#ifndef BOOK_H
#define BOOK_H

#include <QString>

class Book
{
public:
    Book(QString ISBN, QString name, QString writer, QString type, QString press, QString publicationDate, float price,int bookShelf);
    QString getISBN();
    QString getName();
    QString getWriter();
    QString getType();
    QString getPress();
    QString getPublicationDate();
    int getShelfNumber();
    float getPrice();

    void setProperty(QString name, QString wri, QString type, QString press, QString pubDate, float price);
private:
    QString ISBN;
    QString name;
    QString writer;
    QString type;
    QString press;
    QString publicationDate;
    float price;
    int bookShelf;

};

#endif // BOOK_H
