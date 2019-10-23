#include "bookshelfdetailwidget.h"

BookShelfDetailWidget::BookShelfDetailWidget(QWidget *parent) : QWidget(parent)
{

    this->resize(800,600);

    basicInfo = new QFrame(this);
    basicInfo->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    basicInfo->setLineWidth(2);
    basicInfo->move(20, 20);
    basicInfo->resize(760, 150);

    QFormLayout *newShelfLayout = new QFormLayout();

    shelfNumberLineEdit = new QLineEdit();
    shelfNumberLineEdit->setReadOnly(true);

    shelfTypeComboBox = new QComboBox();
    shelfTypeList<<"A"
                 <<"B"
                 <<"C"
                 <<"D"
                 <<"E"
                 <<"F"
                 <<"J";


    shelfTypeMapping.insert("马克思主义、列宁主义、毛泽东思想、邓小平理论","A");
    shelfTypeMapping.insert("哲学、宗教","B");
    shelfTypeMapping.insert("社会科学总论","C");
    shelfTypeMapping.insert("政治、法律","D");
    shelfTypeMapping.insert("文学","E");
    shelfTypeMapping.insert("艺术","F");
    shelfTypeMapping.insert("历史、地理","J");

    shelfCapacityLineEdit = new QLineEdit();
    shelfCapacityLineEdit->setReadOnly(true);

    newShelfLayout->addRow(QStringLiteral("书架编号："), shelfNumberLineEdit);
    newShelfLayout->addRow(QStringLiteral("书架类别："), shelfTypeComboBox);
    newShelfLayout->addRow(QStringLiteral("书架容量："), shelfCapacityLineEdit);
    newShelfLayout->setSpacing(10);
    newShelfLayout->setMargin(20);
    basicInfo->setLayout(newShelfLayout);

    classifyShelf = new QPushButton(basicInfo);
    classifyShelf->setText("重新划分");
    classifyShelf->resize(80,30);
    classifyShelf->move(200,115);
    classifyShelf->show();
    connect(classifyShelf, &QPushButton::clicked, this, &BookShelfDetailWidget::dealClassify);

    deleteShelf = new QPushButton(basicInfo);
    deleteShelf->setText("移除书架");
    deleteShelf->resize(80,30);
    deleteShelf->move(480,115);
    deleteShelf->show();
    connect(deleteShelf, &QPushButton::clicked, this, &BookShelfDetailWidget::dealDelete);

    saveClassify = new QPushButton(basicInfo);
    saveClassify->setText("保存");
    saveClassify->resize(80,30);
    saveClassify->move(200,115);
    saveClassify->hide();
    connect(saveClassify, &QPushButton::clicked, this, &BookShelfDetailWidget::dealSaveClassify);


    bookListWidget = new QWidget;
    bookListWidget->setMinimumWidth(780);
    bookListWidget->resize(780,0);
    bookListWidget->setStyle(QStyleFactory::create("fusion"));
    bookListWidget->show();


    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(bookListWidget);
    scrollArea->move(20,180);
    scrollArea->setFixedSize(770,350);


}


void BookShelfDetailWidget::dealBackSignal()
{
    //广播的形式发送信号
    emit goBackSignal();
}

void BookShelfDetailWidget::setTcpClient(QTcpSocket *tcpClient)
{
    this->tcpClient = tcpClient;
}

void BookShelfDetailWidget::dealClassify()
{
    if(shelfCapacityLineEdit->text()!= "30")
    {
        QMessageBox::StandardButton info = QMessageBox::information(NULL, "Error", "该书架不为空，无法重新划分！");

    }
    else
    {

        for(int i = 0; i < shelfTypeList.count(); i++)
        {
            if (shelfTypeList[i]!= shelfTypeComboBox->currentText())
            {
                shelfTypeComboBox->addItem(shelfTypeList[i]);
            }
        }
        classifyShelf->hide();
        saveClassify->show();

    }
}

//还有点小bug
void  BookShelfDetailWidget::dealSaveClassify()
{
    QString newShelfType = shelfTypeComboBox->currentText();

    QJsonObject getShelfsInfoPackage;
    getShelfsInfoPackage.insert("type","update shelftype by shelfnumber");
    getShelfsInfoPackage.insert("ShelfNumber",shelfNumberLineEdit->text());
    getShelfsInfoPackage.insert("ShelfType", newShelfType);

    tcpClient = this->tcpClient;
    QByteArray byte_array = QJsonDocument(getShelfsInfoPackage).toJson();
    tcpClient->write(byte_array);
    if(tcpClient->waitForReadyRead(1000))//阻塞式连接
    {
        QByteArray result = tcpClient->readAll();
        if(!result.isEmpty())
        {
            QJsonObject temp = QJsonDocument::fromJson(result).object();

            if( temp.value("result").toString()== "succeed")
            {
                shelfTypeComboBox->clear();
                shelfTypeComboBox->addItem(newShelfType);
                QMessageBox::StandardButton info = QMessageBox::information(NULL, "OK", "重新划分成功");

            }
            else
            {
                QMessageBox::StandardButton info = QMessageBox::information(NULL, "Error", "重新划分失败");

            }
            classifyShelf->hide();
            saveClassify->show();

       }

    }
}

void BookShelfDetailWidget::dealDelete()
{

}

void BookShelfDetailWidget::getBooksInfo(int byShelfNumber)
{


    for(int i = 0; i<bookButtonLists.count();i++)
    {
        if(bookButtonLists[i])
        {
            delete bookButtonLists[i];
        }

    }
    bookButtonLists.clear();
    bookLists.clear();
    bookListWidget->resize(bookListWidget->width(),0);

    QJsonObject getBooksInfoPackage;
    getBooksInfoPackage.insert("type", "get books by shelfnumber");
    getBooksInfoPackage.insert("shelfnumber",QString::number(byShelfNumber,10));

    QByteArray byte_array = QJsonDocument(getBooksInfoPackage).toJson();
    tcpClient->write(byte_array);
    if(tcpClient->waitForReadyRead(1000))//阻塞式连接
    {
        QByteArray result = tcpClient->readAll();
        if(!result.isEmpty())
        {
            QJsonObject resultInfo = QJsonDocument::fromJson(result).object();

            for(int i=1; i<=resultInfo.count(); i++)
            {
                QJsonObject temp = resultInfo.value("book" + QString::number(i)).toObject();
                BookButton *oneBookButton = new BookButton(bookListWidget);
                oneBookButton->show();
                bookButtonLists.append(oneBookButton);
                //这里的书籍按钮就不点开了，仅供浏览？
//                connect(oneBookButton, &BookButton::clicked, oneBookButton, &BookButton::sendSignal);
//                connect(oneBookButton, &BookButton::ISBNsignal, this, &MainWindow::createBookDetailWin);

                QString ISBN = temp.value("ISBN").toString();
                QString name = temp.value("name").toString();
                QString writer = temp.value("writer").toString();
                QString type = temp.value("type").toString();
                QString press = temp.value("press").toString();
                QString publicationDate = temp.value("publicationDate").toString();
                float price = temp.value("price").toDouble();
                int bookShelf = temp.value("bookShelf").toInt();

                oneBookButton->setISBN(ISBN);
                oneBookButton->setText("ISBN：" + ISBN + "\n书名：" + name + "\n作者：" + writer + "\n类别：" + type + "\n出版社：" + press);
                oneBookButton->move(0, (i-1) * oneBookButton->height());
                bookListWidget->resize(bookListWidget->width(), bookListWidget->height() + oneBookButton->height());

                Book *oneBook = new Book(ISBN, name, writer, type, press, publicationDate, price, bookShelf);
                bookLists.append(oneBook);
            }
            this->bookListWidget->show();
            this->show();
        }

    }
}

void BookShelfDetailWidget::getShelfInfo(int shelfNumber)
{
    QJsonObject getShelfsInfoPackage;
    getShelfsInfoPackage.insert("type","get shelf information by shelfnumber");
    getShelfsInfoPackage.insert("ShelfNumber",QString::number(shelfNumber));

    tcpClient = this->tcpClient;
    QByteArray byte_array = QJsonDocument(getShelfsInfoPackage).toJson();
    tcpClient->write(byte_array);
    if(tcpClient->waitForReadyRead(1000))//阻塞式连接
    {
        QByteArray result = tcpClient->readAll();
        if(!result.isEmpty())
        {
            QJsonObject temp = QJsonDocument::fromJson(result).object();

            int ShelfNumber = temp.value("ShelfNumber").toInt();
            QString ShelfType = temp.value("ShelfType").toString();
            int ShelfCapacity = temp.value("ShelfCapacity").toInt();

//            QString theshelftype;
//            QMap<QString, QString>::const_iterator i;
//            for (i = shelfTypeMapping.constBegin(); i != shelfTypeMapping.constEnd(); ++i)
//            {
//                if(i.value() == ShelfType)
//                {
//                    theshelftype = i.key();
//                }
//            }
            shelfNumberLineEdit->setText(QString::number(ShelfNumber));
//            shelfTypeComboBox->addItem(theshelftype);
            shelfTypeComboBox->addItem(ShelfType);
            shelfCapacityLineEdit->setText(QString::number(ShelfCapacity));

            }

        }


}
