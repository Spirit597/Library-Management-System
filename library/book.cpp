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

QString Book::getISBN()
{
    return this->ISBN;
}

QString Book::getName()
{
    return this->name;
}

QString Book::getWriter()
{
    return this->writer;
}

QString Book::getType()
{
    return this->type;
}

QString Book::getPress()
{
    return this->press;
}

QString Book::getPublicationDate()
{
    return this->publicationDate;
}

float Book::getPrice()
{
    return this->price;
}

void Book::setProperty(QString name, QString wri, QString type, QString press, QString pubDate, float price)
{
    this->name = name;
    this->writer = wri;
    this->type = type;
    this->press = press;
    this->publicationDate = pubDate;
    this->price = price;
}
