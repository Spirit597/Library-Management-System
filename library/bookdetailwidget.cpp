#include "bookdetailwidget.h"


BookDetailWidget::BookDetailWidget(QWidget *parent) : QWidget(parent)
{

    basicInfo = new QFrame(this);
    basicInfo->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    basicInfo->setLineWidth(2);
    basicInfo->move(20, 20);
    basicInfo->resize(400, 310);

    this->setMinimumWidth(440);

    shelfTable = new QTableWidget(this);
    shelfTable->resize(400,170);
    shelfTable->move(20,340);
    shelfTable->setWindowTitle("书架列表");
    shelfTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    shelfTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    QHeaderView *verticalheader = shelfTable->verticalHeader();
    verticalheader->setVisible(false);
    QHeaderView *horizontalHeader = shelfTable->horizontalHeader();
    horizontalHeader->setDefaultSectionSize(133);
    shelfTable->show();


    ISBNLabel = new QLabel("ISBN：", basicInfo);
    ISBNLabel->move(20, 24);

    ISBNLineEdit = new QLabel(basicInfo);
    ISBNLineEdit->resize(280, 20);
    ISBNLineEdit->move(100, 20);
    //ISBNLineEdit->setReadOnly(true);

    nameLabel = new QLabel("书名：", basicInfo);
    nameLabel->move(20, 54);

    nameLineEdit = new QLineEdit(basicInfo);
    nameLineEdit->resize(280, 20);
    nameLineEdit->move(100, 50);
    nameLineEdit->setReadOnly(true);


    writerLabel = new QLabel("作者：", basicInfo);
    writerLabel->move(20, 84);

    writerLineEdit = new QLineEdit(basicInfo);
    writerLineEdit->resize(280, 20);
    writerLineEdit->move(100, 80);
    writerLineEdit->setReadOnly(true);

    typeLabel = new QLabel("类型：", basicInfo);
    typeLabel->move(20, 114);

    typeComboBox = new QComboBox(basicInfo);
    typeComboBox->resize(280,20);
    typeComboBox->move(100,110);
    shelfTypeList<<"马克思主义、列宁主义、毛泽东思想、邓小平理论"
                 <<"哲学、宗教"
                 <<"社会科学总论"
                 <<"政治、法律"
                 <<"文学"
                 <<"艺术"
                 <<"历史、地理";
    connect(typeComboBox, &QComboBox::currentTextChanged, this, &BookDetailWidget::showShelfListByType);

    shelfTypeMapping.insert("马克思主义、列宁主义、毛泽东思想、邓小平理论","A");
    shelfTypeMapping.insert("哲学、宗教","B");
    shelfTypeMapping.insert("社会科学总论","C");
    shelfTypeMapping.insert("政治、法律","D");
    shelfTypeMapping.insert("文学","I");
    shelfTypeMapping.insert("艺术","J");
    shelfTypeMapping.insert("历史、地理","K");



    pressLabel = new QLabel("出版社：", basicInfo);
    pressLabel->move(20, 144);

    pressLineEdit = new QLineEdit(basicInfo);
    pressLineEdit->resize(280, 20);
    pressLineEdit->move(100, 140);
    pressLineEdit->setReadOnly(true);

    publicationDateLabel = new QLabel("出版日期：", basicInfo);
    publicationDateLabel->move(20, 174);

    publicationDateLineEdit = new QLineEdit(basicInfo);
    publicationDateLineEdit->resize(280, 20);
    publicationDateLineEdit->move(100, 170);
    publicationDateLineEdit->setReadOnly(true);

    priceLabel = new QLabel("价格（元）：", basicInfo);
    priceLabel->move(20, 204);

    priceLineEdit = new QLineEdit(basicInfo);
    priceLineEdit->resize(280, 20);
    priceLineEdit->move(100, 200);
    priceLineEdit->setReadOnly(true);

    bookShelfLabel = new QLabel("书架编号", basicInfo);
    bookShelfLabel->move(20, 234);

    bookShelfLineEdit = new QLineEdit(basicInfo);
    bookShelfLineEdit->resize(280, 20);
    bookShelfLineEdit->move(100, 230);
    bookShelfLineEdit->setReadOnly(true);


    borrowButton = new QPushButton("借书", this);
    borrowButton->resize(80, 30);
    borrowButton->move(340, 300);
    connect(borrowButton, &QPushButton::clicked, this, &BookDetailWidget::dealBorrow);

    editButton = new QPushButton("编辑", this);
    editButton->resize(80, 30);
    editButton->move(100, 280);
    connect(editButton, &QPushButton::clicked, this, &BookDetailWidget::dealEdit);

    saveEditButton = new QPushButton("保存", this);
    saveEditButton->resize(80, 30);
    saveEditButton->move(160, 280);
    saveEditButton->hide();
    connect(saveEditButton, &QPushButton::clicked, this, &BookDetailWidget::dealSaveEdit);



    deleteButton = new QPushButton("删除", this);
    deleteButton->resize(80, 30);
    deleteButton->move(300, 280);
    connect(deleteButton, &QPushButton::clicked, this, &BookDetailWidget::dealDelete);



}

void BookDetailWidget::setISBN(QString ISBN)
{
    this->ISBNLineEdit->setText(ISBN);
}

void BookDetailWidget::setName(QString name)
{

    this->nameLineEdit->setText(name);
}

void BookDetailWidget::setWriter(QString writer)
{

    this->writerLineEdit->setText(writer);
}

void BookDetailWidget::setType(QString type)
{

    this->typeComboBox->addItem(type);
}

void BookDetailWidget::setPress(QString press)
{

    this->pressLineEdit->setText(press);
}

void BookDetailWidget::setPublicationDate(QString publicationDate)
{

    this->publicationDateLineEdit->setText(publicationDate);
}

void BookDetailWidget::setPrice(float price)
{

    this->priceLineEdit->setText(QString::number(price, 'f', 2));
}

void BookDetailWidget::setBookShelf(int shelfnumber)
{
    this->bookShelfLineEdit->setText(QString::number(shelfnumber));
}


void BookDetailWidget::setParameter(QString ID, QString role, QTcpSocket *tcpClient)
{
    this->currentUserID = ID;
    this->currentUserRole = role;
    this->tcpClient = tcpClient;


    if(role == "reader")
    {
        editButton->hide();
        deleteButton->hide();
    }
    else if(role == "admin")
    {
        borrowButton->hide();
    }
}

void BookDetailWidget::dealBorrow()
{

    if(currentUserRole == "admin")
    {
         QMessageBox::StandardButton info = QMessageBox::information(NULL, "Error", "您不是读者，无法借书");
         return;
    }
    QJsonObject borrowPackage;

    borrowPackage.insert("type", "borrow");
    borrowPackage.insert("id", currentUserID);
    borrowPackage.insert("ISBN", ISBNLineEdit->text());

    QByteArray byte_array = QJsonDocument(borrowPackage).toJson();
    tcpClient->write(byte_array);
    if(tcpClient->waitForReadyRead(1000))//阻塞式连接
    {
        QByteArray borrowResult = tcpClient->readAll();
        if(!borrowResult.isEmpty())
        {
            QJsonObject borrowResultInfo = QJsonDocument::fromJson(borrowResult).object();
            if(borrowResultInfo.value("result") == "suc")
            {
                QMessageBox::StandardButton info = QMessageBox::information(NULL, "", "您本次申请借书的操作成功，请按时归还");
            }
            else if(borrowResultInfo.value("result") == "in debt")
            {
                QMessageBox::StandardButton info = QMessageBox::information(NULL, "", "您尚有已超出最大借阅天数的书籍未归还，请您先归还这些书籍并缴清欠款再办理新的借阅");
            }
            else if(borrowResultInfo.value("result") == "others borrow")
            {
                QMessageBox::StandardButton info = QMessageBox::information(NULL, "", "抱歉，本书已被其他读者借走");
            }
            else if(borrowResultInfo.value("result") == "repetitively borrow")
            {
                QMessageBox::StandardButton reminder = QMessageBox::question(NULL, "", "本书已被您借阅，且尚未到期，请问您是否要续借", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                if (reminder == QMessageBox::Yes)
                {
                    QJsonObject reborrowPackage;
                    reborrowPackage.insert("type", "reborrow");
                    //reborrowPackage.insert("id", ID);
                    reborrowPackage.insert("ISBN", ISBNLineEdit->text());
                    QByteArray byte_array = QJsonDocument(reborrowPackage).toJson();
                    tcpClient->write(byte_array);
                    if(tcpClient->waitForReadyRead(1000))//阻塞式连接
                    {
                        QByteArray reborrowResult = tcpClient->readAll();
                        if(!reborrowResult.isEmpty())
                        {
                            QJsonObject reborrowResultInfo = QJsonDocument::fromJson(reborrowResult).object();
                            if(reborrowResultInfo.value("result") == "suc")
                            {
                                QMessageBox::StandardButton info = QMessageBox::information(NULL, "", "您本次申请续借的操作成功，请按时归还");
                            }
                            else
                            {
                                QMessageBox::StandardButton info = QMessageBox::information(NULL, "", "抱歉，您本次申请续借的操作失败");
                            }
                        }
                    }
                }
            }
            else if(borrowResultInfo.value("result") == "reach maxBookNumber")
            {
                QMessageBox::StandardButton info = QMessageBox::information(NULL, "", "您借书的数量已达上限，若要借阅本书，请先归还已借阅的书籍");
            }
        }
    }
}

void BookDetailWidget::dealEdit()
{
    if(currentUserRole == "reader")
    {
         QMessageBox::StandardButton info = QMessageBox::information(NULL, "Error", "您不是管理员，无法编辑");
         return;
    }
    editButton->hide();
    deleteButton->hide();
    saveEditButton->show();

    nameLineEdit->setReadOnly(false);
    writerLineEdit->setReadOnly(false);
    for(int i = 0; i<shelfTypeList.size(); ++i)
        if(shelfTypeList[i]!= typeComboBox->currentText())
        {
            typeComboBox->addItem(shelfTypeList[i]);
        }

    pressLineEdit->setReadOnly(false);
    publicationDateLineEdit->setReadOnly(false);
    priceLineEdit->setReadOnly(false);
    bookShelfLineEdit->setReadOnly(false);


}

void BookDetailWidget::dealSaveEdit()
{

    QJsonObject editBookPackage;

    editBookPackage.insert("type", "edit book");
    editBookPackage.insert("ISBN", ISBNLineEdit->text());
    editBookPackage.insert("name", nameLineEdit->text());
    editBookPackage.insert("writer", writerLineEdit->text());
    editBookPackage.insert("bookType", typeComboBox->currentText());
    editBookPackage.insert("press", pressLineEdit->text());
    editBookPackage.insert("publicationDate", publicationDateLineEdit->text());
    editBookPackage.insert("price", priceLineEdit->text().toFloat());
    editBookPackage.insert("bookShelf", bookShelfLineEdit->text().toInt());

    QByteArray byte_array = QJsonDocument(editBookPackage).toJson();
    tcpClient->write(byte_array);
    if(tcpClient->waitForReadyRead(1000))//阻塞式连接
    {

        QByteArray result = tcpClient->readAll();
        if(!result.isEmpty())
        {
            QJsonObject resultInfo = QJsonDocument::fromJson(result).object();
            if(resultInfo.value("result").toString() == "suc")
            {
                QMessageBox::StandardButton info = QMessageBox::information(NULL, "", "本次修改成功");
                emit editBookSucSignal(ISBNLineEdit->text(), nameLineEdit->text(), writerLineEdit->text(), typeComboBox->currentText(), pressLineEdit->text(), \
                                       publicationDateLineEdit->text(), priceLineEdit->text().toFloat());
                this->close();
            }
            else if(resultInfo.value("result").toString() == "not returned")
            {
                QMessageBox::StandardButton info = QMessageBox::information(NULL, "", "该书尚未归还，不能修改资料");
            }
            else
            {
                QMessageBox::StandardButton info = QMessageBox::information(NULL, "Error", "本次修改失败");
            }
        }
    }
}



void BookDetailWidget::dealDelete()
{
    if(currentUserRole == "reader")
    {
         QMessageBox::StandardButton info = QMessageBox::information(NULL, "Error", "您不是管理员，无法删除");
         return;
    }
    QJsonObject deleteBookPackage;

    deleteBookPackage.insert("type", "delete book");
    deleteBookPackage.insert("ISBN", ISBNLineEdit->text());

    QByteArray byte_array = QJsonDocument(deleteBookPackage).toJson();
    tcpClient->write(byte_array);
    if(tcpClient->waitForReadyRead(1000))//阻塞式连接
    {
        QByteArray result = tcpClient->readAll();
        if(!result.isEmpty())
        {
            QJsonObject resultInfo = QJsonDocument::fromJson(result).object();
            if(resultInfo.value("result").toString() == "suc")
            {
                QMessageBox::StandardButton info = QMessageBox::information(NULL, "", "您已成功删除该书");
                emit ISBNsignal(ISBNLineEdit->text());
                this->close();
            }
            else if(resultInfo.value("result").toString() == "not returned")
            {
                QMessageBox::StandardButton info = QMessageBox::information(NULL, "Error", "该书尚未归还，无法删除");
            }
        }
    }
}

void BookDetailWidget::dealBuyBook()
{
    if(buyBookISBNLineEdit->text() == "")
    {
        QMessageBox::StandardButton info = QMessageBox::information(NULL, "Error", "ISBN的输入不能为空");
        return;
    }
    else if(nameLineEdit->text() == "")
    {
        QMessageBox::StandardButton info = QMessageBox::information(NULL, "Error", "书名的输入不能为空");
        return;
    }
    else if(writerLineEdit->text() == "")
    {
        QMessageBox::StandardButton info = QMessageBox::information(NULL, "Error", "作者的输入不能为空");
        return;
    }
//    else if(typeLineEdit->text() == "")
//    {
//        QMessageBox::StandardButton info = QMessageBox::information(NULL, "Error", "类型的输入不能为空");
//        return;
//    }
    else if(pressLineEdit->text() == "")
    {
        QMessageBox::StandardButton info = QMessageBox::information(NULL, "Error", "出版社的输入不能为空");
        return;
    }
    else if(publicationDateLineEdit->text() == "")
    {
        QMessageBox::StandardButton info = QMessageBox::information(NULL, "Error", "出版日期的输入不能为空");
        return;
    }
    else if(priceLineEdit->text() == "")
    {
        QMessageBox::StandardButton info = QMessageBox::information(NULL, "Error", "价格的输入不能为空");
        return;
    }
    else if(bookShelfLineEdit->text() == "")
    {
        QMessageBox::StandardButton info = QMessageBox::information(NULL, "Error", "书架编号的输入不能为空");
        return;
    }



    QJsonObject buyBookPackage;

    buyBookPackage.insert("type", "buy book");
    buyBookPackage.insert("ISBN", buyBookISBNLineEdit->text());
    buyBookPackage.insert("name", nameLineEdit->text());
    buyBookPackage.insert("writer", writerLineEdit->text());
    buyBookPackage.insert("bookType", typeComboBox->currentText());
    buyBookPackage.insert("press", pressLineEdit->text());
    buyBookPackage.insert("publicationDate", publicationDateLineEdit->text());
    buyBookPackage.insert("price", priceLineEdit->text().toFloat());
    buyBookPackage.insert("bookShelf", bookShelfLineEdit->text().toInt());


    QByteArray byte_array = QJsonDocument(buyBookPackage).toJson();
    tcpClient->write(byte_array);
    if(tcpClient->waitForReadyRead(1000))//阻塞式连接
    {
        QByteArray result = tcpClient->readAll();
        if(!result.isEmpty())
        {
            QJsonObject resultInfo = QJsonDocument::fromJson(result).object();
            if(resultInfo.value("result").toString() == "suc")
            {
                QMessageBox::StandardButton info = QMessageBox::information(NULL, "", "购买新书成功");
                emit buySucSignal();
                this->close();
            }
            else if(resultInfo.value("result").toString() == "repetitive ISBN")
            {
                QMessageBox::StandardButton info = QMessageBox::information(NULL, "", "该书已存在，本次购买失败");
            }
            else if(resultInfo.value("result").toString() == "not afford")
            {
                QMessageBox::StandardButton info = QMessageBox::information(NULL, "", "由于经费不足，本次购买失败");
            }
        }
    }
}

void BookDetailWidget::buyBookMode()
{
    this->editButton->hide();
    this->deleteButton->hide();
    this->ISBNLineEdit->hide();

    nameLineEdit->setReadOnly(false);
    writerLineEdit->setReadOnly(false);
//    typeLineEdit->setReadOnly(false);
    typeComboBox->addItems(shelfTypeList);
    pressLineEdit->setReadOnly(false);
    publicationDateLineEdit->setReadOnly(false);
    priceLineEdit->setReadOnly(false);
    bookShelfLineEdit->setReadOnly(false);


    confirmBuyButton = new QPushButton("确定", this);
    confirmBuyButton->resize(80, 30);
    confirmBuyButton->move(340, 300);
    connect(confirmBuyButton, &QPushButton::clicked, this, &BookDetailWidget::dealBuyBook);

    buyBookISBNLineEdit = new QLineEdit(basicInfo);
    buyBookISBNLineEdit->resize(280, 20);
    buyBookISBNLineEdit->move(100, 20);

}

//这个似乎用不到了
void BookDetailWidget::showShelfList()
{
    shelfTable->clearContents();

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
            QStringList header;
            shelfTable->setRowCount(resultInfo.count());
            shelfTable->setColumnCount(3);
            header<<"ShelfNumber"<<"ShelfType"<<"ShelfCapacity";//表头
            shelfTable->setHorizontalHeaderLabels(header);
//            shelfTable->setResizeMode(2, QHeaderView::Stretch);
            for(int i = 1; i <=resultInfo.count() ; i++)
            {
                QJsonObject temp = resultInfo.value("shelfs" + QString::number(i)).toObject();

                int ShelfNumber = temp.value("ShelfNumber").toInt();
                QString ShelfType = temp.value("ShelfType").toString();
                int ShelfCapacity = temp.value("ShelfCapacity").toInt();

                shelfTable->setItem(i-1,0,new QTableWidgetItem(QString::number(ShelfNumber)));
                shelfTable->setItem(i-1,1,new QTableWidgetItem(ShelfType));
                shelfTable->setItem(i-1,2,new QTableWidgetItem(QString::number(ShelfCapacity)));

            }
            shelfTable->resizeRowsToContents();
            shelfTable->show();

        }

    }
}

void BookDetailWidget::showShelfListByType( QString shelfType)
{

    if( currentUserRole == "reader")
    {
        shelfTable->hide();

    }
    else
    {
        shelfTable->clearContents();

        QJsonObject getShelfsInfoPackage;
        getShelfsInfoPackage.insert("type","get shelfs information by shelfType");
        QString mappingtype = shelfTypeMapping.find(shelfType).value();//映射关系转换："马克思主义、列宁主义、毛泽东思想、邓小平理论","A"
        getShelfsInfoPackage.insert("shelftype",mappingtype);

        tcpClient = this->tcpClient;
        QByteArray byte_array = QJsonDocument(getShelfsInfoPackage).toJson();
        tcpClient->write(byte_array);
        if(tcpClient->waitForReadyRead(1000))//阻塞式连接
        {
            QByteArray result = tcpClient->readAll();
            if(!result.isEmpty())
            {
                QJsonObject resultInfo = QJsonDocument::fromJson(result).object();
                QStringList header;
                shelfTable->setRowCount(resultInfo.count());
                shelfTable->setColumnCount(3);
                header<<"ShelfNumber"<<"ShelfType"<<"ShelfCapacity";//表头
                shelfTable->setHorizontalHeaderLabels(header);
                for(int i = 1; i <=resultInfo.count() ; i++)
                {
                    QJsonObject temp = resultInfo.value("shelfs" + QString::number(i)).toObject();

                    int ShelfNumber = temp.value("ShelfNumber").toInt();
                    QString ShelfType = temp.value("ShelfType").toString();
                    int ShelfCapacity = temp.value("ShelfCapacity").toInt();

                    shelfTable->setItem(i-1,0,new QTableWidgetItem(QString::number(ShelfNumber)));
                    shelfTable->setItem(i-1,1,new QTableWidgetItem(ShelfType));
                    shelfTable->setItem(i-1,2,new QTableWidgetItem(QString::number(ShelfCapacity)));

                }
                shelfTable->resizeRowsToContents();
                shelfTable->show();

            }

        }


    }

}
