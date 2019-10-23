#include "shelfbutton.h"
#include "QStyleFactory"
ShelfButton::ShelfButton(QWidget *parent):QPushButton(parent)
{
    this->setStyleSheet("text-align: left;");
    this->resize(150,100);
    this->setMaximumHeight(60);
    this->setMinimumHeight(60);

}

void ShelfButton::sendCreatNewWin()
{
    emit ShelfNumbersignal(this->ShelfNumber);
}

void ShelfButton::setShelfNumber(int ShelfNumber)
{
    this->ShelfNumber = ShelfNumber;

}

