#include "book.h"

Book::Book(QString ISBN, QString name, QString writer, QString type, QString press, QString publicationDate, float price)
{
    this->ISBN = ISBN;
    this->name = name;
    this->writer = writer;
    this->type = type;
    this->press = press;
    this->publicationDate = publicationDate;
    this->price = price;
}
