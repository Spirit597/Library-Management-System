#ifndef SHELFLISTWIDGET_H
#define SHELFLISTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QPushButton>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>

#include <QTcpSocket>
#include <QVector>
#include <shelfbutton.h>
#include <bookshelf.h>
#include <bookshelfdetailwidget.h>
class ShelfListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShelfListWidget(QWidget *parent = 0);
    void returnMainWindow();
    void getShelfInfo();//获取所有书架列表的函数
    void creteShelfDetailWin(int ShelfNumber);//获取一特定书架的详细信息的触发函数
    void dealGetNewShelf();
    void setTcpClient(QTcpSocket *tcpClient);//设定此窗口的连接client,实际上是变相的传参数

    void enterShelfDetail();//进入书架信息的处理函数
    void backToShelfList();//返回书架列表的处理函数
private:
    float shelfPrice;//其实应该放在bookshelf的属性中，后续优化

    QFrame *basicInfo;
    QLineEdit *shelfNumberLineEdit;
    QComboBox *shelfTypeComboBox;
    QLineEdit *shelfTypeLineEdit;
    QLineEdit *shelfCapacityLineEdit;

    QPushButton *getNewShelf;


    QWidget *shelfListWidget;
    QScrollArea *ScrollArea;
    QVector <BookShelf *> shelfLists;
    QVector <ShelfButton *> shelfButtonLists;
    QStringList shelfTypeList;
    QMap <QString,QString> shelfTypeMapping;//书架类别的映射容器,对应表

    QTcpSocket *tcpClient;

signals:
    void returnSignal();
//    void shelfNumeberSignal(int shelfNumber);

};

#endif // SHELFLISTWIDGET_H
