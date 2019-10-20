#ifndef BOOK_H
#define BOOK_H

#include <QString>

class Book
{
public:
    Book(QString ISBN, QString name, QString writer, QString type, QString press, QString publicationDate, float price);
private:
    QString ISBN;
    QString name;
    QString writer;
    QString type;
    QString press;
    QString publicationDate;
    float price;

    bool isBorrowed = false;

};

#endif // BOOK_H
