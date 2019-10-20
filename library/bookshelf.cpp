#include "bookshelf.h"

BookShelf::BookShelf(int ShelfNumber, QString ShelfType, int ShelfCapicity)
{
    this->ShelfNumber = ShelfNumber;
    this->ShelfType = ShelfType;
    this->ShelfCapicity = ShelfCapicity;

}

int BookShelf::getShelfNumber()
{
    return this->ShelfNumber;
}

QString BookShelf::getShelfType()
{
    return this->ShelfType;
}


int BookShelf::getShelfCapicity()
{
    return this->ShelfCapicity;
}
