#ifndef BOOKSHELF_H
#define BOOKSHELF_H

#include <QString>

class BookShelf
{
public:
    BookShelf(int ShelfNumber, QString ShelfType, int ShelfCapicity );
    int getShelfNumber();
    QString getShelfType();
    int getShelfCapicity();
private:
    int ShelfNumber;
    QString ShelfType;
    int ShelfCapicity;
};

#endif // BOOKSHELF_H
