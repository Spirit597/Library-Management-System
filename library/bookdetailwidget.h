#ifndef BOOKDETAILWIDGET_H
#define BOOKDETAILWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

#include <QJsonObject>
#include <QJsonDocument>

#include <QTcpSocket>

class BookDetailWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BookDetailWidget(QWidget *parent = 0);
    void setISBN(QString ISBN);
    void setName(QString name);
    void setWriter(QString writer);
    void setType(QString type);
    void setPress(QString press);
    void setPublicationDate(QString publicationDate);
    void setPrice(float price);
    void setParameter(QString ID, QString role, QTcpSocket *tcpClient);


    void dealBorrow();
    void dealEdit();
    void dealSaveEdit();
    void dealDelete();
    void dealBuyBook();

    void buyBookMode();//采购新书的界面与该界面类似，故重复利用

private:
    QLabel *ISBNLabel;
    QLabel *nameLabel;
    QLabel *writerLabel;
    QLabel *typeLabel;
    QLabel *pressLabel;
    QLabel *publicationDateLabel;
    QLabel *priceLabel;

    QLabel *ISBNLineEdit;//这个变量的类型是之后修改成QLabel的，务必注意不要弄错
    QLineEdit *nameLineEdit;
    QLineEdit *writerLineEdit;
    QLineEdit *typeLineEdit;
    QLineEdit *pressLineEdit;
    QLineEdit *publicationDateLineEdit;
    QLineEdit *priceLineEdit;
    QLineEdit *buyBookISBNLineEdit;


    QFrame *basicInfo;

    QPushButton *borrowButton;
    QPushButton *editButton;
    QPushButton *saveEditButton;
    QPushButton *deleteButton;
    QPushButton *confirmBuyButton;

    QString currentUserID;
    QString currentUserRole;
    QTcpSocket *tcpClient;

signals:
    void ISBNsignal(QString ISBN);
    void editBookSucSignal(QString ISBN, QString name, QString wri, QString type, QString press, QString pubDate, float price);
    void buySucSignal();


public slots:
};

#endif // BOOKDETAILWIDGET_H
