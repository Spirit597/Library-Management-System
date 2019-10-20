#ifndef BORROWEDBOOK_H
#define BORROWEDBOOK_H

#include <QString>

class BorrowedBook
{
public:
    BorrowedBook(QString ISBN);
private:
    QString ISBN;
    float price;
    int days;
    float debt;
};

#endif // BORROWEDBOOK_H
