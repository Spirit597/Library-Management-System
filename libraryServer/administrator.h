#ifndef administrator_H
#define administrator_H

#include <QString>

class administrator
{
public:
    administrator(QString ID);
    void setID(QString ID);
    void setPassword(QString password);
    void setName(QString name);
private:
    QString ID;
    QString password;
    QString name;
};

#endif // administrator_H
