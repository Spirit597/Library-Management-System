#ifndef RETURNBOOKWIDGET_H
#define RETURNBOOKWIDGET_H

#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

#include <QJsonObject>
#include <QJsonDocument>

#include <QTcpSocket>

class ReturnBookWidget : public QFrame
{
    Q_OBJECT
public:
    explicit ReturnBookWidget(QWidget *parent = 0);
    void setISBN(QString ISBN);
    void setName(QString name);
    void setWriter(QString writer);
    void setType(QString type);
    void setDays(int days);
    void setExtraDays(int extraDays);
    void setDebt(float debt);
    void setIndex(int index);
    void setTcpClient(QTcpSocket *tcpClient);
    void indexMinusOne();

    QString getISBN();

    void dealReturn();
    void dealRepayDebt();
private:
    QLabel *ISBN;
    QLabel *name;
    QLabel *writer;
    QLabel *type;
    QLabel *days;
    QLabel *extraDays;
    QLabel *debt;

    QPushButton *returnBookButton;
    QPushButton *repayDebtButton;

    QTcpSocket *tcpClient = NULL;

    int index; //决定此widget的几何坐标

signals:
    void indexSignal(int index);

public slots:
};

#endif // RETURNBOOKWIDGET_H
