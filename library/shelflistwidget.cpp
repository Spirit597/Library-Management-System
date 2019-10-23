#include "shelflistwidget.h"

ShelfListWidget::ShelfListWidget(QWidget *parent):QWidget(parent)
{
//    this->resize(540, 580);
    this->setWindowTitle("书架列表");

    basicInfo = new QFrame(this);
    basicInfo->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    basicInfo->setLineWidth(2);
    basicInfo->move(20, 20);
    basicInfo->resize(500, 160);

    QFormLayout *newShelfLayout = new QFormLayout();

    shelfNumberLineEdit = new QLineEdit();
    shelfNumberLineEdit->setText("默认分配，无需填写");
    shelfNumberLineEdit->setReadOnly(true);

    shelfTypeComboBox = new QComboBox();
    shelfTypeList<<"马克思主义、列宁主义、毛泽东思想、邓小平理论"
                 <<"哲学、宗教"
                 <<"社会科学总论"
                 <<"政治、法律"
                 <<"文学"
                 <<"艺术"
                 <<"历史、地理";
    shelfTypeComboBox->addItems(shelfTypeList);

    shelfTypeMapping.insert("马克思主义、列宁主义、毛泽东思想、邓小平理论","A");
    shelfTypeMapping.insert("哲学、宗教","B");
    shelfTypeMapping.insert("社会科学总论","C");
    shelfTypeMapping.insert("政治、法律","D");
    shelfTypeMapping.insert("文学","E");
    shelfTypeMapping.insert("艺术","F");
    shelfTypeMapping.insert("历史、地理","F");

    shelfCapacityLineEdit = new QLineEdit();
    shelfCapacityLineEdit->setText("默认初始为30");
    shelfCapacityLineEdit->setReadOnly(true);

    newShelfLayout->addRow(QStringLiteral("书架编号："), shelfNumberLineEdit);
    newShelfLayout->addRow(QStringLiteral("书架类别："), shelfTypeComboBox);
    newShelfLayout->addRow(QStringLiteral("书架容量："), shelfCapacityLineEdit);
    newShelfLayout->setSpacing(10);
    newShelfLayout->setMargin(10);
    basicInfo->setLayout(newShelfLayout);

    //购置书架
    getNewShelf = new QPushButton(basicInfo);
    getNewShelf->setText("购置书架");
    getNewShelf->resize(80,30);
    getNewShelf->move(220, 120);
    getNewShelf->show();
    connect(getNewShelf, &QPushButton::clicked, this, &ShelfListWidget::dealGetNewShelf);

    shelfPrice = 0.1;

    //滑动区域显示书架信息
    shelfListWidget = new QWidget(this);
    shelfListWidget->resize(520,300);
    shelfListWidget->setMaximumWidth(500);
    shelfListWidget->setStyle(QStyleFactory::create("fusion"));
    shelfListWidget->show();

    ScrollArea = new QScrollArea(this);
    ScrollArea->setWidget(shelfListWidget);
    ScrollArea->setFixedSize(510,300);
    ScrollArea->move(20,200);

}


void ShelfListWidget::returnMainWindow()
{
    emit returnSignal();
}

void ShelfListWidget::setTcpClient(QTcpSocket *tcpClient)
{
    this->tcpClient = tcpClient;
}

void ShelfListWidget::creteShelfDetailWin(int ShelfNumber)
{
    BookShelfDetailWidget *shelfDetailWidget = new BookShelfDetailWidget;
    shelfDetailWidget->setTcpClient(this->tcpClient);
    shelfDetailWidget->setWindowTitle(QString::number(ShelfNumber)+"号书架");
    shelfDetailWidget->getBooksInfo(ShelfNumber);

}

//  处理购置书架的函数
void ShelfListWidget::dealGetNewShelf()
{
    /* 查询当前剩余金额
     * 若足够：执行insert，返回购置成功；
     * 若不足：返回购置失败
     */
    QJsonObject getLibraryInfoPackage;
    QString newShelfType = shelfTypeComboBox->currentText();
    QString mappingtype = shelfTypeMapping.find(newShelfType).value();//  映射关系转换："马克思主义、列宁主义、毛泽东思想、邓小平理论","A"
    getLibraryInfoPackage.insert("type", "buy new shelf");
    getLibraryInfoPackage.insert("shelfType", mappingtype);
    getLibraryInfoPackage.insert("shelfPrice", QString::number(shelfPrice));

    QByteArray byte_array = QJsonDocument(getLibraryInfoPackage).toJson();
    tcpClient->write(byte_array);
    if(tcpClient->waitForReadyRead(1000))//阻塞式连接
    {
        QByteArray result = tcpClient->readAll();
        if(!result.isEmpty())
        {
            QJsonObject resultInfo = QJsonDocument::fromJson(result).object();
            if (resultInfo.value("result").toString() == "failed")
            {
                QMessageBox::StandardButton info = QMessageBox::information(NULL, "", "购置失败");

            }
            else
            {
                QMessageBox::StandardButton info = QMessageBox::information(NULL, "", "购置成功");
                this->getShelfInfo();

            }

        }
    }
}



void ShelfListWidget::getShelfInfo()
{

    for(int i = 0; i<shelfButtonLists.count();i++ )
    {
        if(shelfButtonLists[i])
        {
            delete shelfButtonLists[i];
        }
    }
    shelfButtonLists.clear();
    shelfLists.clear();
    shelfListWidget->resize(shelfListWidget->width(),0);
    QJsonObject getShelfsInfoPackage;
    getShelfsInfoPackage.insert("type","get shelfs information");

    tcpClient = this->tcpClient;
    QByteArray byte_array = QJsonDocument(getShelfsInfoPackage).toJson();
    tcpClient->write(byte_array);
    if(tcpClient->waitForReadyRead(1000))//阻塞式连接
    {
        QByteArray result = tcpClient->readAll();
        if(!result.isEmpty())
        {
            QJsonObject resultInfo = QJsonDocument::fromJson(result).object();
            for(int i = 1; i <=resultInfo.count() ; i++)
            {
                QJsonObject temp = resultInfo.value("shelfs" + QString::number(i)).toObject();
                ShelfButton *singleShelfButton = new ShelfButton(shelfListWidget);
                singleShelfButton->show();
                shelfButtonLists.append(singleShelfButton);
                /*
                 * 注意发送信号的过程：
                 * 单个书架按钮被点击->
                 * 按钮发出一个带书架编号参数的信号->
                 * 书架列表窗口接收信号，创建该书架的详细信息窗口
                 */
                connect(singleShelfButton, &ShelfButton::clicked, singleShelfButton, &ShelfButton::sendCreatNewWin);
                connect(singleShelfButton, &ShelfButton::ShelfNumbersignal, this,&ShelfListWidget::creteShelfDetailWin);

                int ShelfNumber = temp.value("ShelfNumber").toInt();
                QString ShelfType = temp.value("ShelfType").toString();
                int ShelfCapacity = temp.value("ShelfCapacity").toInt();

                singleShelfButton->setShelfNumber(ShelfNumber);
                QString strshelfnumber = QString::number(ShelfNumber);
                QString strshelfcapacity = QString::number(ShelfCapacity);
                singleShelfButton->setText("书架号："+strshelfnumber+"\n存放书类："+ShelfType+"\n容量："+strshelfcapacity);
                int row = (((i-1) / 3) + 1);
                int column = ((i - 1) % 3) + 1;
                singleShelfButton->move((column-1)*(singleShelfButton->width()+20),(row-1) * (singleShelfButton->height()+20));
                if (i % 3 == 1)
                {
                    shelfListWidget->resize(shelfListWidget->width(),shelfListWidget->height()+row*(singleShelfButton->height()+20));
                }

                BookShelf *singleShelf = new BookShelf(ShelfNumber,ShelfType,ShelfCapacity);
                shelfLists.append(singleShelf);
            }

        }
        this->shelfListWidget->show();
    }


}

