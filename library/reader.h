#ifndef READER_H
#define READER_H

#include <QVector>

class Reader
{
public:
    Reader();
    void setID(QString ID);
    void setName(QString name);
    void setMaxBookNumber(int maxBookNumber);
    void setMaxDays(int maxDays);
private:
    QString ID;
    QString name;
    int maxBookNumber;
    int maxDays;


};

#endif // READER_H
