#ifndef BOOKSHELFDETAILWIDGET_H
#define BOOKSHELFDETAILWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QHeaderView>
#include "QStyleFactory"
#include <QComboBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>

#include <book.h>
#include <bookbutton.h>
#include <libraryinfowindow.h>

class BookShelfDetailWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BookShelfDetailWidget(QWidget *parent = 0);
    void dealBackSignal();
    void getBooksInfo(int byShelfNumber);//根据书架号获取书架上的书藉
    void setTcpClient(QTcpSocket *tcpClient);//设定此窗口的连接client,实际上是变相的传参数
    void getShelfInfo(int shelfNumber);
    void dealClassify();
    void dealDelete();
    void dealSaveClassify();

private:

    QFrame *basicInfo;

    QLineEdit *shelfNumberLineEdit;
    QComboBox *shelfTypeComboBox;
    QLineEdit *shelfCapacityLineEdit;

    QWidget *bookListWidget;
    QScrollArea *scrollArea;
    QPushButton *classifyShelf;
    QPushButton *deleteShelf;
    QPushButton *saveClassify;

    QVector <BookButton *> bookButtonLists;
    QVector <Book*> bookLists;
    QTcpSocket *tcpClient;

    QStringList shelfTypeList;
    QMap <QString,QString> shelfTypeMapping;//书架类别的映射容器,对应表


signals:
    void goBackSignal();

public slots:
};

#endif // BOOKSHELFDETAILWIDGET_H
