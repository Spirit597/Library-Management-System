#include "reader.h"

Reader::Reader(QString ID)
{
    this->ID = ID;
}

void Reader::setID(QString ID)
{
    this->ID = ID;
}

void Reader::setPassword(QString password)
{
    this->password = password;
}

void Reader::setName(QString name)
{
    this->name = name;
}

void Reader::setMaxBookNumber(int maxBookNumber)
{
    this->maxBookNumber = maxBookNumber;
}

void Reader::setMaxDays(int maxDays)
{
    this->maxDays = maxDays;
}


