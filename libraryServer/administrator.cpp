#include "administrator.h"

administrator::administrator(QString ID)
{
    this->ID = ID;
}

void administrator::setID(QString ID)
{
    this->ID = ID;
}

void administrator::setName(QString name)
{
    this->name = name;
}

void administrator::setPassword(QString password)
{
    this->password = password;
}
