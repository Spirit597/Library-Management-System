#include "bookbutton.h"

BookButton::BookButton(QWidget *parent) : QPushButton(parent)
{

    this->setStyleSheet("text-align: left;");
    this->resize(780, 80);
}

void BookButton::setISBN(QString ISBN)
{
    this->ISBN = ISBN;
}



QString BookButton::getISBN()
{
    return this->ISBN;
}



void BookButton::sendSignal()
{
    emit ISBNsignal(this->ISBN);
}
