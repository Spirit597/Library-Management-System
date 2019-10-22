#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    this->resize(600,370);

    localIP = new QLabel(this);
    localIP->setMinimumWidth(300);
    localIP->move(10,10);
    localIP->setText("本机IP地址: " + QNetworkInterface().allAddresses().at(1).toString());



    clientList = new QComboBox;
    clientList->setParent(this);
    clientList->setMinimumWidth(200);
    clientList->move(10,70);
    clientList->addItem("- 全部连接 -");

    userList = new QComboBox;
    userList->setParent(this);
    userList->setMinimumWidth(350);
    userList->move(220,70);
    userList->addItem("- 已登录用户 -");

    promptMessage = new QTextEdit;
    promptMessage->setReadOnly(true);
    promptMessage->setParent(this);
    promptMessage->setMinimumWidth(560);
    promptMessage->move(10,100);

    localPort = new QLabel(this);
    localPort->setMidLineWidth(300);
    localPort->move(10,40);
    localPort->setText("本机端口号: 8000");

    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("libraryDB.db");
    }
    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }

    tcpServer = new QTcpServer(this);
    bool ok = tcpServer->listen(QHostAddress::Any, 8000);
    connect(tcpServer, &QTcpServer::newConnection, this, &MainWidget::dealNewConnection);
    currentClient = NULL;
}

void MainWidget::dealNewConnection()
{
    currentClient = tcpServer->nextPendingConnection();
    connect(currentClient, &QTcpSocket::readyRead, this, &MainWidget::readDataAndRespond);
    connect(currentClient, &QTcpSocket::disconnected, this, &MainWidget::refreshClientList);
    tcpClients.append(currentClient);
    clientList->addItem(tr("%1:%2").arg(currentClient->peerAddress().toString().split("::ffff:")[1])\
            .arg(currentClient->peerPort()));
    promptMessage->append(tr("%1:%2").arg(currentClient->peerAddress().toString().split("::ffff:")[1])\
            .arg(currentClient->peerPort()) + " connect!");
}

void MainWidget::readDataAndRespond()
{
    QTcpSocket *theClient = currentClient;//之后做多用户时，theClient的值要在tcpClients列表中查找
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("libraryDB.db");
    }
    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }
    else
    {
        QSqlQuery query;
        QByteArray clientInfo = theClient->readAll();
        QJsonObject clientMessage;
        if(!clientInfo.isEmpty())
        {
            clientMessage = QJsonDocument::fromJson(clientInfo).object();
            if(clientMessage.value("type").toString() == "login")
            {
                if(clientMessage.value("role").toString() == "reader")
                {
                    QString sqlSentence = "select * from Reader";
                    if(!query.exec(sqlSentence))
                    {
                        qDebug()<<query.lastError()<< endl;
                    }
                    else
                    {
                        for(int i=0; i<userIDList.size(); i++)
                        {
                            if(clientMessage.value("id").toString() == userIDList[i])
                            {
                                QJsonObject loginResponsePackage;
                                loginResponsePackage.insert("result",4);//该账号已在别处登录
                                QByteArray byte_array = QJsonDocument(loginResponsePackage).toJson();
                                theClient->write(byte_array);
                                return;
                            }
                        }

                        int readerNumber = 0;//存储数据库中一共有多少读者
                        while(query.next())
                        {
                            readerNumber++;
                        }
                        int readerNumber1 = 0;//用来保存数据库中已经检索的读者数量
                        if(!query.exec(sqlSentence))
                        {
                            qDebug()<<query.lastError()<< endl;
                        }
                        while(query.next())
                        {
                            readerNumber1++;
                            QString id1 = query.value("ID").toString();
                            QString password1= query.value("password").toString();

                            if(id1 == clientMessage.value("id").toString())
                            {
                                if(password1 == clientMessage.value("password").toString())
                                {
                                    promptMessage->append("A reader whose id is " + clientMessage.value("id").toString() + " login!");
                                    userIDList.append(id1);
                                    QString userMessage = "id:" + id1 + "  role:";

                                    userMessage += "读者";

                                    userList->addItem(userMessage);
                                    QJsonObject loginResponsePackage;
                                    loginResponsePackage.insert("result", 1);//账号密码都正确
                                    QByteArray byte_array = QJsonDocument(loginResponsePackage).toJson();
                                    theClient->write(byte_array);

                                }
                                else
                                {

                                    QJsonObject loginResponsePackage;
                                    loginResponsePackage.insert("result",2);//账号存在，密码错误
                                    QByteArray byte_array = QJsonDocument(loginResponsePackage).toJson();
                                    theClient->write(byte_array);

                                }
                                break;
                            }
                            if(readerNumber1 == readerNumber)
                            {
                                QJsonObject userPackage;
                                userPackage.insert("result",3);//账号不存在
                                QByteArray byte_array = QJsonDocument(userPackage).toJson();
                                theClient->write(byte_array);
                            }
                        }
                    }
                }
                else if(clientMessage.value("role").toString() == "admin")
                {
                    QString sqlSentence = "select * from Administrator";
                    if(!query.exec(sqlSentence))
                    {
                        qDebug()<<query.lastError()<< endl;
                    }
                    else
                    {
                        for(int i=0; i<userIDList.size(); i++)
                        {
                            if(clientMessage.value("id").toString() == userIDList[i])
                            {
                                QJsonObject loginResponsePackage;
                                loginResponsePackage.insert("result",4);//该账号已在别处登录
                                QByteArray byte_array = QJsonDocument(loginResponsePackage).toJson();
                                theClient->write(byte_array);
                                return;
                            }
                        }

                        int readerNumber = 0;//存储数据库中一共有多少管理员
                        while(query.next())
                        {
                            readerNumber++;
                        }
                        int readerNumber1 = 0;//用来保存数据库中已经检索的管理员数量
                        if(!query.exec(sqlSentence))
                        {
                            qDebug()<<query.lastError()<< endl;
                        }
                        while(query.next())
                        {
                            readerNumber1++;
                            QString id1 = query.value("ID").toString();
                            QString password1= query.value("password").toString();

                            if(id1 == clientMessage.value("id").toString())
                            {
                                if(password1 == clientMessage.value("password").toString())
                                {
                                    promptMessage->append("An admin whose id is " + clientMessage.value("id").toString() + " login!");
                                    userIDList.append(id1);
                                    QString userMessage = "id:" + id1 + "  role:";

                                    userMessage += "管理员";

                                    userList->addItem(userMessage);
                                    QJsonObject loginResponsePackage;
                                    loginResponsePackage.insert("result", 1);//账号密码都正确
                                    QByteArray byte_array = QJsonDocument(loginResponsePackage).toJson();
                                    theClient->write(byte_array);

                                }
                                else
                                {

                                    QJsonObject loginResponsePackage;
                                    loginResponsePackage.insert("result",2);//账号存在，密码错误
                                    QByteArray byte_array = QJsonDocument(loginResponsePackage).toJson();
                                    theClient->write(byte_array);

                                }
                                break;
                            }
                            if(readerNumber1 == readerNumber)
                            {
                                QJsonObject userPackage;
                                userPackage.insert("result",3);//账号不存在
                                QByteArray byte_array = QJsonDocument(userPackage).toJson();
                                theClient->write(byte_array);
                            }
                        }
                    }
                }
            }
            else if(clientMessage.value("type").toString() == "get books")
            {
                QString sqlSentence = "select * from Book";
                if(!query.exec(sqlSentence))
                {
                    qDebug()<<query.lastError()<< endl;
                }
                QJsonObject booksPackage;
                int number = 0;
                while(query.next())
                {
                    number ++;
                    QJsonObject bookPackage;
                    bookPackage.insert("ISBN", query.value("ISBN").toString());
                    bookPackage.insert("name", query.value("name").toString());
                    bookPackage.insert("writer", query.value("writer").toString());
                    bookPackage.insert("type", query.value("type").toString());
                    bookPackage.insert("press", query.value("press").toString());
                    bookPackage.insert("publicationDate", query.value("publicationDate").toString());
                    bookPackage.insert("price", query.value("price").toFloat());
                    bookPackage.insert("bookShelf", query.value("bookShelf").toInt());

                    booksPackage.insert("book" + QString::number(number), bookPackage);

                }
                QByteArray byte_array = QJsonDocument(booksPackage).toJson();
                theClient->write(byte_array);
            }
            //怎么传入shelfnumber?询问包里装入即可已修改
            else if(clientMessage.value("type").toString() == "get books by shelfnumber")
            {
                qDebug()<<clientMessage.value("shelfnumber").toInt();
                QString sqlSentence = "select * from Book where bookShelf = ";
                sqlSentence = sqlSentence+clientMessage.value("shelfnumber").toString();
                if(!query.exec(sqlSentence))
                {
                    qDebug()<<query.lastError()<< endl;
                }
                QJsonObject booksPackage;
                int number = 0;
                while(query.next())
                {
                    number ++;
                    QJsonObject bookPackage;
                    bookPackage.insert("ISBN", query.value("ISBN").toString());
                    bookPackage.insert("name", query.value("name").toString());
                    bookPackage.insert("writer", query.value("writer").toString());
                    bookPackage.insert("type", query.value("type").toString());
                    bookPackage.insert("press", query.value("press").toString());
                    bookPackage.insert("publicationDate", query.value("publicationDate").toString());
                    bookPackage.insert("price", query.value("price").toFloat());
                    bookPackage.insert("bookShelf", query.value("bookShelf").toInt());


                    booksPackage.insert("book" + QString::number(number), bookPackage);

                }
                QByteArray byte_array = QJsonDocument(booksPackage).toJson();
                theClient->write(byte_array);
            }

            else if(clientMessage.value("type").toString() == "borrow")
            {
                QString sqlSentence = "select * from BorrowedBook";
                if(!query.exec(sqlSentence))
                {
                    qDebug()<<query.lastError()<< endl;
                }
                //bool inDebt = false;//该读者是否有超时未还的书
                //bool othersBorrow = false;//该书是否被其他人借走
                //bool repBorrow = false;//该书是否被本人借走，但未到期
                int bookNumber = 0;//该读者已经借阅的书的数量
                while(query.next())
                {
                    QString ISBN = query.value("ISBN").toString();
                    QString borrowerID = query.value("borrowerID").toString();
                    int maxDays = query.value("maxDays").toInt();
                    int days = query.value("days").toInt();
                    if(borrowerID == clientMessage.value("id").toString())
                    {
                        bookNumber++;
                        if(days > maxDays)//至少有一本书超时未还
                        {
                            QJsonObject borrowResponsePackage;
                            borrowResponsePackage.insert("result", "in debt");
                            QByteArray byte_array = QJsonDocument(borrowResponsePackage).toJson();
                            theClient->write(byte_array);
                            return;
                        }
                        if(ISBN == clientMessage.value("ISBN").toString())//该书被本人借走，但未到期
                        {
                            //repBorrow = true;
                            QJsonObject borrowResponsePackage;
                            borrowResponsePackage.insert("result", "repetitively borrow");
                            QByteArray byte_array = QJsonDocument(borrowResponsePackage).toJson();
                            theClient->write(byte_array);
                            return;
                        }
                    }
                    else if(ISBN == clientMessage.value("ISBN").toString())//该书被其他人借走
                    {
                        //othersBorrow = true;
                        QJsonObject borrowResponsePackage;
                        borrowResponsePackage.insert("result", "others borrow");
                        QByteArray byte_array = QJsonDocument(borrowResponsePackage).toJson();
                        theClient->write(byte_array);
                        return;
                    }
                }
                sqlSentence = "select * from Reader";
                if(!query.exec(sqlSentence))
                {
                    qDebug()<<query.lastError()<< endl;
                }
                int maxBookNumber = 0;
                int maxDays = 0;
                while(query.next())
                {
                    if(query.value("ID").toString() == clientMessage.value("id").toString())
                    {
                        maxBookNumber = query.value("maxBookNumber").toInt();
                        maxDays = query.value("maxDays").toInt();
                        break;
                    }
                }
                if(maxBookNumber > bookNumber)//所有条件都满足，可以借书
                {
                    sqlSentence = "insert into BorrowedBook (ISBN, borrowerID, maxDays) values ('";
                    sqlSentence = sqlSentence + clientMessage.value("ISBN").toString() + "','" + clientMessage.value("id").toString() + "'," + QString::number(maxDays) + ")";
                    //qDebug() << sqlSentence;
                    if(!query.exec(sqlSentence))
                    {
                        qDebug()<<query.lastError()<< endl;
                    }
                    QJsonObject borrowResponsePackage;
                    borrowResponsePackage.insert("result", "suc");
                    QByteArray byte_array = QJsonDocument(borrowResponsePackage).toJson();
                    theClient->write(byte_array);
                }
                else
                {
                    QJsonObject borrowResponsePackage;
                    borrowResponsePackage.insert("result", "reach maxBookNumber");
                    QByteArray byte_array = QJsonDocument(borrowResponsePackage).toJson();
                    theClient->write(byte_array);
                }

            }
            else if(clientMessage.value("type").toString() == "reborrow")
            {
                QString sqlSentence = "select * from BorrowedBook";
                if(!query.exec(sqlSentence))
                {
                    qDebug()<<query.lastError()<< endl;
                }
                while(query.next())
                {
                    QString ISBN = query.value("ISBN").toString();
                    //QString borrowerID = query.value("borrowerID").toString();
                    if(ISBN == clientMessage.value("ISBN").toString())
                    {
                        sqlSentence = "update BorrowedBook set days = 0 where ISBN = '";
                        sqlSentence = sqlSentence + ISBN + "'";
                        if(!query.exec(sqlSentence))
                        {
                            qDebug()<<query.lastError()<< endl;
                            QJsonObject reborrowResponsePackage;
                            reborrowResponsePackage.insert("result", "fail");
                            QByteArray byte_array = QJsonDocument(reborrowResponsePackage).toJson();
                            theClient->write(byte_array);
                        }
                        else
                        {
                            QJsonObject reborrowResponsePackage;
                            reborrowResponsePackage.insert("result", "suc");
                            QByteArray byte_array = QJsonDocument(reborrowResponsePackage).toJson();
                            theClient->write(byte_array);
                        }
                    }
                }
            }
            else if(clientMessage.value("type").toString() == "get borrowed")//请求返回某个用户的借的所有书的信息
            {
                QString sqlSentence = "select * from (BorrowedBook left outer join Book on BorrowedBook.ISBN = Book.ISBN)";
                if(!query.exec(sqlSentence))
                {
                    qDebug()<<query.lastError()<< endl;
                }
                QString id = clientMessage.value("id").toString();
                QJsonObject borrowedBooksPackage;
                int number = 0;
                while(query.next())
                {
                    if(id == query.value("borrowerID").toString())
                    {
                        number ++;
                        QJsonObject borrowedBookPackage;
                        borrowedBookPackage.insert("ISBN", query.value("ISBN").toString());
                        borrowedBookPackage.insert("maxDays", query.value("maxDays").toInt());
                        borrowedBookPackage.insert("days", query.value("days").toInt());
                        borrowedBookPackage.insert("debt", query.value("debt").toFloat());
                        borrowedBookPackage.insert("name", query.value("name").toString());
                        //qDebug() << "name" << query.value("name").toString();
                        borrowedBookPackage.insert("writer", query.value("writer").toString());
                        borrowedBookPackage.insert("type", query.value("type").toString());

                        borrowedBooksPackage.insert("book" + QString::number(number), borrowedBookPackage);
                    }
                }
                QByteArray byte_array = QJsonDocument(borrowedBooksPackage).toJson();
                theClient->write(byte_array);
            }
            else if(clientMessage.value("type").toString() == "return book")
            {
                //qDebug() << "ISBN" << clientMessage.value("ISBN");
                QString sqlSentence = "select debt from BorrowedBook where ISBN ='";
                sqlSentence = sqlSentence + clientMessage.value("ISBN").toString() + "'";
                if(!query.exec(sqlSentence))
                {
                    qDebug()<<query.lastError()<< endl;

                    QJsonObject responsePackage;
                    responsePackage.insert("result", "fail");
                    QByteArray byte_array = QJsonDocument(responsePackage).toJson();
                    theClient->write(byte_array);
                }
                else
                {
                    while(query.next())
                    {
                        if(query.value("debt").toFloat() > 0)//罚款未还清
                        {
                            QJsonObject responsePackage;
                            responsePackage.insert("result", "in debt");
                            QByteArray byte_array = QJsonDocument(responsePackage).toJson();
                            theClient->write(byte_array);
                        }
                        else
                        {
                            sqlSentence = "delete from BorrowedBook where ISBN = '";
                            sqlSentence = sqlSentence + clientMessage.value("ISBN").toString() + "'";
                            if(!query.exec(sqlSentence))
                            {
                                qDebug()<<query.lastError()<< endl;

                                QJsonObject responsePackage;
                                responsePackage.insert("result", "fail");
                                QByteArray byte_array = QJsonDocument(responsePackage).toJson();
                                theClient->write(byte_array);
                            }
                            else
                            {
                                QJsonObject responsePackage;
                                responsePackage.insert("result", "suc");
                                QByteArray byte_array = QJsonDocument(responsePackage).toJson();
                                theClient->write(byte_array);
                            }
                        }
                    }
                }




            }
            else if(clientMessage.value("type").toString() == "repay debt")
            {
                QString sqlSentence = "select fund from LibraryInfo";
                float fund;
                if(!query.exec(sqlSentence))
                {
                    qDebug()<<query.lastError()<< endl;
                }
                while(query.next())
                {
                    fund = query.value("fund").toFloat();
                }
                sqlSentence = "select debt from BorrowedBook where ISBN = '";
                sqlSentence = sqlSentence + clientMessage.value("ISBN").toString() + "'";
                if(!query.exec(sqlSentence))
                {
                    qDebug()<<query.lastError()<< endl;
                }

                while(query.next())
                {
                    fund = fund + query.value("debt").toFloat();
                }
                sqlSentence = "update LibraryInfo set fund = ";
                sqlSentence = sqlSentence + QString::number(fund, 'f', 2);
                if(!query.exec(sqlSentence))
                {
                    qDebug()<<query.lastError()<< endl;

                    QJsonObject responsePackage;
                    responsePackage.insert("result", "fail");
                    QByteArray byte_array = QJsonDocument(responsePackage).toJson();
                    theClient->write(byte_array);
                }


                sqlSentence = "update BorrowedBook set debt = 0 where ISBN ='";
                sqlSentence = sqlSentence + clientMessage.value("ISBN").toString() + "'";
                if(!query.exec(sqlSentence))
                {
                    qDebug()<<query.lastError()<< endl;

                    QJsonObject responsePackage;
                    responsePackage.insert("result", "fail");
                    QByteArray byte_array = QJsonDocument(responsePackage).toJson();
                    theClient->write(byte_array);
                }
                else
                {
                    QJsonObject responsePackage;
                    responsePackage.insert("result", "suc");
                    QByteArray byte_array = QJsonDocument(responsePackage).toJson();
                    theClient->write(byte_array);
                }
            }
            else if(clientMessage.value("type").toString() == "get library information")
            {
                QString sqlSentence = "select fund from LibraryInfo";
                float fund;
                if(!query.exec(sqlSentence))
                {
                    qDebug()<<query.lastError()<< endl;
                }
                while(query.next())
                {
                    fund = query.value("fund").toFloat();
                }

                QJsonObject loginResponsePackage;
                loginResponsePackage.insert("fund", fund);
                QByteArray byte_array = QJsonDocument(loginResponsePackage).toJson();
                theClient->write(byte_array);
            }
            else if(clientMessage.value("type").toString() == "get shelfs information")
            {
                QString sqlSentence = "select * from BookShelf";
                if(!query.exec(sqlSentence))
                {
                    qDebug()<<query.lastError()<< endl;
                }
                QJsonObject shelfsPackage;
                int number = 0;
                while(query.next())
                {
                    number++;
                    QJsonObject shelfPackage;
                    shelfPackage.insert("ShelfNumber",query.value("ShelfNumber").toInt());
                    shelfPackage.insert("ShelfType",query.value("ShelfType").toString());
                    shelfPackage.insert("ShelfCapacity",query.value("ShelfCapacity").toInt());

                    shelfsPackage.insert("shelfs"+QString::number(number),shelfPackage);

                }
                QByteArray byte_array = QJsonDocument(shelfsPackage).toJson();
                theClient->write(byte_array);
            }

            else if(clientMessage.value("type").toString() == "get user information")
            {
                if(clientMessage.value("role") == "admin")
                {
                    QString sqlSentence = "select * from Administrator where ID = '";
                    sqlSentence = sqlSentence + clientMessage.value("id").toString() + "'";
                    if(!query.exec(sqlSentence))
                    {
                        qDebug()<<query.lastError()<< endl;
                    }
                    while(query.next())
                    {
                        QJsonObject responsePackage;
                        responsePackage.insert("name", query.value("name").toString());
                        QByteArray byte_array = QJsonDocument(responsePackage).toJson();
                        theClient->write(byte_array);
                    }
                }
                else if(clientMessage.value("role") == "reader")
                {
                    QString sqlSentence = "select * from Reader where ID = '";
                    sqlSentence = sqlSentence + clientMessage.value("id").toString() + "'";
                    if(!query.exec(sqlSentence))
                    {
                        qDebug()<<query.lastError()<< endl;
                    }
                    while(query.next())
                    {
                        QJsonObject responsePackage;
                        responsePackage.insert("name", query.value("name").toString());
                        responsePackage.insert("maxBookNumber", query.value("maxBookNumber").toInt());
                        responsePackage.insert("maxDays", query.value("maxDays").toInt());
                        QByteArray byte_array = QJsonDocument(responsePackage).toJson();
                        theClient->write(byte_array);
                    }
                }
            }
            else if(clientMessage.value("type").toString() == "edit user information")
            {
                if(clientMessage.value("role").toString() == "admin")
                {
                    if(clientMessage.value("isPasswordEdited").toBool())
                    {
                        QString sqlSentence = "select password from Administrator where ID = '";
                        sqlSentence = sqlSentence + clientMessage.value("id").toString() + "'";
                        if(!query.exec(sqlSentence))
                        {
                            qDebug()<<query.lastError()<< endl;
                        }
                        while(query.next())
                        {
                            if(query.value("password").toString() != clientMessage.value("priPassword").toString())
                            {
                                QJsonObject responsePackage;
                                responsePackage.insert("result", "primary password wrong");
                                QByteArray byte_array = QJsonDocument(responsePackage).toJson();
                                theClient->write(byte_array);
                                return;
                            }
                            else
                            {
                                sqlSentence = "update Administrator set password = '" + clientMessage.value("newPassword").toString() + "' ";
                                sqlSentence = sqlSentence + " where ID = '" + clientMessage.value("id").toString() + "'";
                                if(!query.exec(sqlSentence))
                                {
                                    qDebug()<<query.lastError()<< endl;
                                    QJsonObject responsePackage;
                                    responsePackage.insert("result", "fail");
                                    QByteArray byte_array = QJsonDocument(responsePackage).toJson();
                                    theClient->write(byte_array);
                                    return;
                                }
                            }
                        }
                    }
                    QString sqlSentence = "update Administrator set name = '";
                    sqlSentence = sqlSentence + clientMessage.value("name").toString() + "' where ID ='";
                    sqlSentence = sqlSentence + clientMessage.value("id").toString() + "'";
                    if(!query.exec(sqlSentence))
                    {
                        qDebug()<<query.lastError()<< endl;
                        QJsonObject responsePackage;
                        responsePackage.insert("result", "fail");
                        QByteArray byte_array = QJsonDocument(responsePackage).toJson();
                        theClient->write(byte_array);
                    }
                    else
                    {
                        QJsonObject responsePackage;
                        responsePackage.insert("result", "suc");
                        QByteArray byte_array = QJsonDocument(responsePackage).toJson();
                        theClient->write(byte_array);
                    }
                }
                else if(clientMessage.value("role").toString() == "reader")
                {
                    if(clientMessage.value("isPasswordEdited").toBool())
                    {
                        QString sqlSentence = "select password from Reader where ID = '";
                        sqlSentence = sqlSentence + clientMessage.value("id").toString() + "'";
                        if(!query.exec(sqlSentence))
                        {
                            qDebug()<<query.lastError()<< endl;
                        }
                        while(query.next())
                        {
                            if(query.value("password").toString() != clientMessage.value("priPassword").toString())
                            {
                                QJsonObject responsePackage;
                                responsePackage.insert("result", "primary password wrong");
                                QByteArray byte_array = QJsonDocument(responsePackage).toJson();
                                theClient->write(byte_array);
                                return;
                            }
                            else
                            {
                                sqlSentence = "update Reader set password = '" + clientMessage.value("newPassword").toString() + "'";
                                sqlSentence = sqlSentence + " where ID = '" + clientMessage.value("id").toString() + "'";
                                if(!query.exec(sqlSentence))
                                {
                                    qDebug()<<query.lastError()<< endl;
                                    QJsonObject responsePackage;
                                    responsePackage.insert("result", "fail");
                                    QByteArray byte_array = QJsonDocument(responsePackage).toJson();
                                    theClient->write(byte_array);
                                    return;
                                }
                            }
                        }
                    }
                    QString sqlSentence = "update Reader set name = '" + clientMessage.value("name").toString() + "'";
                    if(clientMessage.value("isNumberAndDaysEdited").toBool())
                    {
                        sqlSentence = sqlSentence + ", maxBookNumber = " + QString::number(clientMessage.value("maxBookNumber").toInt());
                        sqlSentence = sqlSentence + ", maxDays = " +QString::number(clientMessage.value("maxDays").toInt());
                    }
                    sqlSentence = sqlSentence + " where ID ='" + clientMessage.value("id").toString() + "'";
                    if(!query.exec(sqlSentence))
                    {
                        qDebug()<<query.lastError()<< endl;
                        QJsonObject responsePackage;
                        responsePackage.insert("result", "fail");
                        QByteArray byte_array = QJsonDocument(responsePackage).toJson();
                        theClient->write(byte_array);
                    }
                    else
                    {
                        QJsonObject responsePackage;
                        responsePackage.insert("result", "suc");
                        QByteArray byte_array = QJsonDocument(responsePackage).toJson();
                        theClient->write(byte_array);
                    }
                }
            }
            else if(clientMessage.value("type").toString() == "delete book")
            {
                QString sqlSentence = "select * from BorrowedBook where ISBN = '";
                sqlSentence = sqlSentence + clientMessage.value("ISBN").toString() + "'";
                if(!query.exec(sqlSentence))
                {
                    qDebug()<<query.lastError()<< endl;
                }
                while(query.next())
                {
                    QJsonObject responsePackage;
                    responsePackage.insert("result", "not returned");
                    QByteArray byte_array = QJsonDocument(responsePackage).toJson();
                    theClient->write(byte_array);
                    return;
                }
                sqlSentence = "delete from Book where ISBN = '";
                sqlSentence = sqlSentence + clientMessage.value("ISBN").toString() + "'";
                if(!query.exec(sqlSentence))
                {
                    qDebug()<<query.lastError()<< endl;
                }
                else
                {
                    QJsonObject responsePackage;
                    responsePackage.insert("result", "suc");
                    QByteArray byte_array = QJsonDocument(responsePackage).toJson();
                    theClient->write(byte_array);
                }
            }
            else if(clientMessage.value("type").toString() == "edit book")
            {
                QString sqlSentence = "select * from BorrowedBook where ISBN = '";
                sqlSentence = sqlSentence + clientMessage.value("ISBN").toString() + "'";

                if(!query.exec(sqlSentence))
                {
                    qDebug()<<query.lastError()<< endl;

                }
                while(query.next())
                {
                    QJsonObject responsePackage;
                    responsePackage.insert("result", "not returned");
                    QByteArray byte_array = QJsonDocument(responsePackage).toJson();
                    theClient->write(byte_array);
                    return;
                }

                sqlSentence = "update Book set name = '";
                sqlSentence = sqlSentence + clientMessage.value("name").toString() + "', writer = '";
                sqlSentence = sqlSentence + clientMessage.value("writer").toString() + "', type = '";
                sqlSentence = sqlSentence + clientMessage.value("bookType").toString() + "', press = '";
                sqlSentence = sqlSentence + clientMessage.value("press").toString() + "', publicationDate ='";
                sqlSentence = sqlSentence + clientMessage.value("publicationDate").toString() + "', price ='";
                sqlSentence = sqlSentence + QString::number(clientMessage.value("price").toDouble(), 'f', 2)+"',bookShelf=";
                sqlSentence = sqlSentence + QString::number(clientMessage.value("bookShelf").toInt());
                sqlSentence = sqlSentence + " where ISBN = '";
                sqlSentence = sqlSentence + clientMessage.value("ISBN").toString() + "'";
                qDebug() << sqlSentence;
                if(!query.exec(sqlSentence))
                {
                    qDebug()<<query.lastError()<< endl;
                    QJsonObject responsePackage;
                    responsePackage.insert("result", "fail");
                    QByteArray byte_array = QJsonDocument(responsePackage).toJson();
                    theClient->write(byte_array);
                }
                else
                {
                    QJsonObject responsePackage;
                    responsePackage.insert("result", "suc");
                    QByteArray byte_array = QJsonDocument(responsePackage).toJson();
                    theClient->write(byte_array);
                }
            }
            else if(clientMessage.value("type").toString() == "buy book")
            {
                QString sqlSentence = "select * from Book where ISBN = '";
                sqlSentence = sqlSentence + clientMessage.value("ISBN").toString() + "'";

                if(!query.exec(sqlSentence))
                {
                    qDebug()<<query.lastError()<< endl;

                }
                while(query.next())
                {
                    QJsonObject responsePackage;
                    responsePackage.insert("result", "repetitive ISBN");
                    QByteArray byte_array = QJsonDocument(responsePackage).toJson();
                    theClient->write(byte_array);
                    return;
                }

                sqlSentence = "select fund from LibraryInfo";
                if(!query.exec(sqlSentence))
                {
                    qDebug()<<query.lastError()<< endl;

                }
                float fund = 0;
                while(query.next())
                {
                    fund = query.value("fund").toFloat();
                }
                if(fund >= clientMessage.value("price").toDouble())
                {
                    fund = fund - clientMessage.value("price").toDouble();
                    sqlSentence = "update LibraryInfo set fund = " + QString::number(fund);
                    if(!query.exec(sqlSentence))
                    {
                        qDebug()<<query.lastError()<< endl;
                        QJsonObject responsePackage;
                        responsePackage.insert("result", "fail");
                        QByteArray byte_array = QJsonDocument(responsePackage).toJson();
                        theClient->write(byte_array);
                        return;
                    }
                    sqlSentence = "insert into Book (ISBN,name,writer,type,press,publicationDate,price) values (";
                    sqlSentence = sqlSentence + "'" + clientMessage.value("ISBN").toString() + "',";
                    sqlSentence = sqlSentence + "'" + clientMessage.value("name").toString() + "',";
                    sqlSentence = sqlSentence + "'" + clientMessage.value("writer").toString() + "',";
                    sqlSentence = sqlSentence + "'" + clientMessage.value("bookType").toString() + "',";
                    sqlSentence = sqlSentence + "'" + clientMessage.value("press").toString() + "',";
                    sqlSentence = sqlSentence + "'" + clientMessage.value("publicationDate").toString() + "',";
                    sqlSentence = sqlSentence + QString::number(clientMessage.value("price").toDouble(), 'f', 2) + ")";

                    if(!query.exec(sqlSentence))
                    {
                        qDebug()<<query.lastError()<< endl;

                        QJsonObject responsePackage;
                        responsePackage.insert("result", "fail");
                        QByteArray byte_array = QJsonDocument(responsePackage).toJson();
                        theClient->write(byte_array);
                        return;
                    }
                    else
                    {
                        QJsonObject responsePackage;
                        responsePackage.insert("result", "suc");
                        QByteArray byte_array = QJsonDocument(responsePackage).toJson();
                        theClient->write(byte_array);
                        return;
                    }
                }
                else
                {
                    QJsonObject responsePackage;
                    responsePackage.insert("result", "not afford");
                    QByteArray byte_array = QJsonDocument(responsePackage).toJson();
                    theClient->write(byte_array);
                }
            }
            /*QJsonObject responsePackage;
            responsePackage.insert("result", 1);
            QByteArray byte_array = QJsonDocument(responsePackage).toJson();
            theClient->write(byte_array);*/
        }
    }
}

void MainWidget::refreshClientList()
{
    for(int i=0; i<tcpClients.length(); i++)
    {
        if(tcpClients[i]->state() == QAbstractSocket::UnconnectedState)
        {
            tcpClients[i]->destroyed();
            tcpClients.removeAt(i);
            promptMessage->append(clientList->itemText(i+1) + " disconnent!");
            if(clientList->count() == userList->count())
            {
                userList->removeItem(i+1);
                if(userIDList[i].startsWith("R"))
                {
                    promptMessage->append("A reader whose id is " + userIDList[i] + " logout!");
                }
                else
                {
                    promptMessage->append("An admin whose id is " + userIDList[i] + " logout!");
                }

                userIDList.removeAt(i);
            }

            break;
        }
    }

    clientList->clear();
    clientList->addItem("全部连接");
    for(int i=0; i<tcpClients.length(); i++)
    {
        clientList->addItem(tr("%1:%2").arg(tcpClients[i]->peerAddress().toString().split("::ffff:")[1])\
                .arg(tcpClients[i]->peerPort()));
    }
}

void MainWidget::closeEvent(QCloseEvent *event)
{
    for(int i=0; i<tcpClients.size(); i++)
    {
        if(tcpClients[i])
        {
            disconnect(tcpClients[i], 0, this, 0);
        }
    }

}
