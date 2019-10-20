#ifndef READER_H
#define READER_H

#include <QVector>

#include "borrowedbook.h"

class Reader
{
public:
    Reader(QString ID);
    void setID(QString ID);
    void setPassword(QString password);
    void setName(QString name);
    void setMaxBookNumber(int maxBookNumber);
    void setMaxDays(int maxDays);
private:
    QString ID;
    QString password;
    QString name;
    int maxBookNumber;
    int maxDays;

    QVector <BorrowedBook *> borrowedBookList;

};

#endif // READER_H
