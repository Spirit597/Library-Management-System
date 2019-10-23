#include "mainWindow.h"
#include "QStyleFactory"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle("图书馆信息管理系统");


    topBar = new QFrame;
    topBar->setParent(this);
    topBar->setFixedSize(780, 30);
    topBar->setFrameShape(QFrame::StyledPanel);

    viewUserInfoButton = new QPushButton("查看个人信息", topBar);
    viewUserInfoButton->resize(100, 20);
    connect(viewUserInfoButton, &QPushButton::clicked, this, &MainWindow::createUserInfoWin);

    viewLibraryInfoButton = new QPushButton("查看图书馆信息", topBar);
    viewLibraryInfoButton->resize(120, 20);
    viewLibraryInfoButton->move(120, 0);
    connect(viewLibraryInfoButton, &QPushButton::clicked, this, &MainWindow::createViewLibraryInfoWin);

    viewShelfInfoButton = new QPushButton("查看书架信息", topBar);
    viewShelfInfoButton->resize(120, 20);
    viewShelfInfoButton->move(260, 0);
    connect(viewShelfInfoButton, &QPushButton::clicked, this, &MainWindow::createViewShelfInfoWin);


    logOutButton = new QPushButton("注销", topBar);
    logOutButton->resize(50, 20);
    logOutButton->move(730, 0);
    connect(logOutButton, &QPushButton::clicked, this, &MainWindow::sendLogoutSignal);

    returnBooksButton = new QPushButton("还书", topBar);
    returnBooksButton->resize(50, 20);
    returnBooksButton->move(120, 0);
    connect(returnBooksButton, &QPushButton::clicked, this, &MainWindow::createReturnBookWin);

    refreshBooksButton = new QPushButton("刷新书籍列表", this);
    refreshBooksButton->resize(100, 20);
    refreshBooksButton->move(0, 100);
    connect(refreshBooksButton, &QPushButton::clicked, this, &MainWindow::getBooksInfo);

    sortingMethodList = new QComboBox(this);
    sortingMethodList->resize(130, 20);
    sortingMethodList->move(180, 100);
    sortingMethodList->addItem("- 请选择排序方式 -");
    sortingMethodList->addItem("按价格升序排序");
    sortingMethodList->addItem("按价格降序排序");
    connect(sortingMethodList, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::dealSortingMethodChoose);

    searchingMethodList = new QComboBox(this);
    searchingMethodList->resize(160, 20);
    searchingMethodList->move(330, 100);
    searchingMethodList->addItem("- 请选择搜索/筛选方式 -");
    searchingMethodList->addItem("按国际标准书号ISBN搜索");
    searchingMethodList->addItem("按书籍名搜索");
    searchingMethodList->addItem("按作者名搜索");
    searchingMethodList->addItem("按类别筛选");
    connect(searchingMethodList, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::dealSearchingMethodChoose);

    searchingInput = new QLineEdit(this);
    searchingInput->resize(220, 20);
    searchingInput->move(500, 100);

    searchButton = new QPushButton("搜索", this);
    searchButton->resize(50, 20);
    searchButton->move(730, 100);
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::dealSearch);



    typeList = new MultiChooseComboBox;
    typeList->setParent(this);
    typeList->resize(220, 20);
    typeList->move(500, 100);
    typeList->hide();

    booksListWidget = new QWidget;
    booksListWidget->setMinimumWidth(780);
    booksListWidget->resize(780, 0);
    booksListWidget->setStyle(QStyleFactory::create("fusion"));

    booksListWidget->show();


    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(booksListWidget);
    scrollArea->move(0,130);
    scrollArea->setFixedSize(800,350);

    createBookButton = new QPushButton("购买新书", this);
    createBookButton->resize(100, 30);
    createBookButton->move(680, 490);
    connect(createBookButton, &QPushButton::clicked, this, &MainWindow::createNewBook);

    this->setMinimumSize(800, 530);






}

void MainWindow::dealSortingMethodChoose(int index)
{

    if(sortingMethodList->currentText() == "按价格升序排序")
    {
        for(int i=0; i<matchedBooksButtonsList.count(); i++)
        {
            for(int j=0; j<(matchedBooksButtonsList.count() - i - 1); j++)
            {

                if(getBookPrice(matchedBooksButtonsList[j]) > getBookPrice(matchedBooksButtonsList[j+1]))
                {

                    BookButton *temp = matchedBooksButtonsList[j];
                    matchedBooksButtonsList[j] = matchedBooksButtonsList[j+1];
                    matchedBooksButtonsList[j+1] = temp;
                    temp = NULL;

                }
            }
        }

    }
    else if(sortingMethodList->currentText() == "按价格降序排序")
    {
        for(int i=0; i<matchedBooksButtonsList.count(); i++)
        {
            for(int j=0; j<(matchedBooksButtonsList.count() - i - 1); j++)
            {

                if(getBookPrice(matchedBooksButtonsList[j]) < getBookPrice(matchedBooksButtonsList[j+1]))
                {

                    BookButton *temp = matchedBooksButtonsList[j];
                    matchedBooksButtonsList[j] = matchedBooksButtonsList[j+1];
                    matchedBooksButtonsList[j+1] = temp;
                    temp = NULL;

                }
            }
        }
    }
    for(int i=0; i<matchedBooksButtonsList.count(); i++)
    {
        matchedBooksButtonsList[i]->move(0, i * matchedBooksButtonsList[i]->height());
    }

}

void MainWindow::dealSearchingMethodChoose(int index)
{
    if(searchingMethodList->currentText() == "按类别筛选")
    {
        searchingInput->hide();
        typeList->show();
        searchButton->setText("筛选");
    }
    else
    {
        typeList->hide();
        searchingInput->show();
        searchButton->setText("搜索");

    }
}

void MainWindow::dealSearch()
{
    matchedBooksButtonsList.clear();
    for(int i=0; i<booksButtonsList.count(); i++)
    {
        matchedBooksButtonsList.append(booksButtonsList[i]);//先还原
        booksButtonsList[i]->show();
    }
    QVector <int> notMatchedIndexList;
    if(searchingMethodList->currentText() == "按国际标准书号ISBN搜索")
    {
        QString ISBN = searchingInput->text();
        for(int i=0; i<matchedBooksButtonsList.count(); i++)
        {
            if(!matchedBooksButtonsList[i]->getISBN().contains(ISBN))
            {
                matchedBooksButtonsList[i]->hide();
                notMatchedIndexList.append(i);
            }
        }
    }
    else if(searchingMethodList->currentText() == "按书籍名搜索")
    {
        QString name = searchingInput->text();
        for(int i=0; i<matchedBooksButtonsList.count(); i++)
        {
            if(!getBookProperty(matchedBooksButtonsList[i], "name").contains(name))
            {
                matchedBooksButtonsList[i]->hide();
                notMatchedIndexList.append(i);
            }
        }
    }
    else if(searchingMethodList->currentText() == "按作者名搜索")
    {
        QString writer = searchingInput->text();
        for(int i=0; i<matchedBooksButtonsList.count(); i++)
        {
            if(!getBookProperty(matchedBooksButtonsList[i], "writer").contains(writer))
            {
                matchedBooksButtonsList[i]->hide();
                notMatchedIndexList.append(i);
            }
        }
    }
    else if(searchingMethodList->currentText() == "按类别筛选")
    {
        QStringList chosenTypesList = typeList->currentText().split(';');

        for(int i=0; i<matchedBooksButtonsList.count(); i++)
        {
            for(int j=0; j<chosenTypesList.count(); j++)
            {
                QString type = chosenTypesList[j];
                if(!type.contains(getBookProperty(matchedBooksButtonsList[i], "type")))
                {
                    if(j == (chosenTypesList.count() - 1))//说明不符合任何一个被选中的类别
                    {
                        matchedBooksButtonsList[i]->hide();
                        notMatchedIndexList.append(i);
                    }
                }
                else//说明符合一个被选中的类别
                {
                    break;
                }
            }
        }



    }

    for(int i=notMatchedIndexList.count()-1; i>=0; i--)//必须从后往前遍历，否则remove后下标会变化
    {
        matchedBooksButtonsList.remove(notMatchedIndexList[i]);
    }
    booksListWidget->resize(booksListWidget->width(), 0);
    for(int i=0; i<matchedBooksButtonsList.count(); i++)
    {
        matchedBooksButtonsList[i]->move(0, i * matchedBooksButtonsList[i]->height());
        booksListWidget->resize(booksListWidget->width(), booksListWidget->height() + matchedBooksButtonsList[i]->height());
    }
    sortingMethodList->setCurrentIndex(0);
}

void MainWindow::dealDeleteBook(QString ISBN)
{
    for(int i=0; i<booksButtonsList.count(); i++)
    {
        if(booksButtonsList[i]->getISBN() == ISBN)
        {
            booksButtonsList[i]->setText("已删除");
            disconnect(booksButtonsList[i], 0, 0, 0);
        }
    }
}

void MainWindow::dealEditBookSuc(QString ISBN, QString name, QString wri, QString type, QString press, QString pubDate, float price)
{
//    for(int i=0; i<booksList.count(); i++)
//    {
//        if(booksList[i]->getISBN() == ISBN)
//        {
//            booksList[i]->setProperty(name, wri, type, press, pubDate, price);
//            booksButtonsList[i]->setText("ISBN：" + ISBN + "\n书名：" + name + "\n作者：" + wri + "\n类别：" + type + "\n出版社：" + press);
//            break;
//        }
//    }
    this->getBooksInfo();
}

void MainWindow::upDateCurrentUser(QString ID, QString role)
{
    this->currentUserID = ID;
    this->currentUserRole = role;
}



void MainWindow::setTcpClient(QTcpSocket *tcpClient)
{
    this->tcpClient = tcpClient;
}

void MainWindow::createReturnBookWin()
{
    if(currentUserRole == "admin")
    {
        QMessageBox::information(NULL,"Error",QString("您不是读者，不需要还书"),QMessageBox::Ok);
        return;
    }
    ReturnBookWindow *returnBookWin = new ReturnBookWindow;
    returnBookWin->getBorrowedBookInfo(currentUserID, tcpClient);
    returnBookWin->show();
}

void MainWindow::createViewLibraryInfoWin()
{
    if(currentUserRole == "reader")
    {
        QMessageBox::information(NULL,"Error",QString("您不是管理员，无权限查看图书馆信息"),QMessageBox::Ok);
        return;
    }
    LibraryInfoWindow *libraryInfoWin = new LibraryInfoWindow;
    libraryInfoWin->getLibraryInfo(tcpClient);
//    //(已解决）继承主界面的tcpClient,这样比较简便,但是这样不规范也不安全
//    libraryInfoWin->setTcpClient(this->tcpClient);
//    libraryInfoWin->getShelfInfo();
    libraryInfoWin->show();

}

void MainWindow::createViewShelfInfoWin()
{
    ShelfListWidget *shelfListInfoWin = new ShelfListWidget;
    shelfListInfoWin->setTcpClient(this->tcpClient);
    shelfListInfoWin->getShelfInfo();
    shelfListInfoWin->show();
}

void MainWindow::createUserInfoWin()
{
    UserInfoWindow *userInfoWin = new UserInfoWindow;
    userInfoWin->setRole(currentUserRole, currentUserRole);
    userInfoWin->chooseRole();
    userInfoWin->getUserInfo(tcpClient, currentUserID);
    userInfoWin->show();
}

void MainWindow::createBookDetailWin(QString ISBN)
{
    BookDetailWidget *bookDetailWin = new BookDetailWidget;
    connect(bookDetailWin, &BookDetailWidget::ISBNsignal, this, &MainWindow::dealDeleteBook);
    connect(bookDetailWin, &BookDetailWidget::editBookSucSignal, this, &MainWindow::dealEditBookSuc);
    bookDetailWin->setParameter(currentUserID, currentUserRole, tcpClient);
    for(int i=0; i<booksList.count(); i++)
    {
        if(booksList[i]->getISBN() == ISBN)
        {
            bookDetailWin->setISBN(ISBN);
            bookDetailWin->setName(booksList[i]->getName());

            bookDetailWin->setWindowTitle(booksList[i]->getName());

            bookDetailWin->setWriter(booksList[i]->getWriter());
            bookDetailWin->setType(booksList[i]->getType());
            bookDetailWin->setPress(booksList[i]->getPress());
            bookDetailWin->setPublicationDate(booksList[i]->getPublicationDate());
            bookDetailWin->setPrice(booksList[i]->getPrice());
            bookDetailWin->setBookShelf(booksList[i]->getShelfNumber());
            break;
        }
    }

    bookDetailWin->show();

}

void MainWindow::createNewBook()
{
    if(currentUserRole == "reader")
    {
        QMessageBox::StandardButton info = QMessageBox::information(NULL, "Error", "您不是管理员，无此权限");
        return;
    }
    BookDetailWidget *newBook = new BookDetailWidget;
    connect(newBook, &BookDetailWidget::buySucSignal, this, &MainWindow::getBooksInfo);
    newBook->setWindowTitle("购买新书");
    newBook->setParameter(currentUserID, currentUserRole, tcpClient);
    newBook->buyBookMode();

    newBook->show();
}

void MainWindow::sendLogoutSignal()
{
    emit logoutSignal();
}

void MainWindow::changeButton(QString role)
{
    if(role == "admin")
    {
        this->returnBooksButton->hide();
        this->viewLibraryInfoButton->show();
        this->createBookButton->show();
    }
    else if(role == "reader")
    {
        this->viewLibraryInfoButton->hide();
        this->createBookButton->hide();
        this->returnBooksButton->show();
    }
}

void MainWindow::getBooksInfo()
{
    for(int i=0; i<booksButtonsList.count(); i++)
    {
        if(booksButtonsList[i])
        {
            delete booksButtonsList[i];
        }
    }

    booksButtonsList.clear();
    matchedBooksButtonsList.clear();
    booksList.clear();
    booksListWidget->resize(booksListWidget->width(), 0);
    QJsonObject getBooksInfoPackage;
    getBooksInfoPackage.insert("type", "get books");

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
                BookButton *oneBookButton = new BookButton(booksListWidget);
                oneBookButton->show();
                booksButtonsList.append(oneBookButton);
                matchedBooksButtonsList.append(oneBookButton);
                connect(oneBookButton, &BookButton::clicked, oneBookButton, &BookButton::sendSignal);
                connect(oneBookButton, &BookButton::ISBNsignal, this, &MainWindow::createBookDetailWin);



                QString ISBN = temp.value("ISBN").toString();
                QString name = temp.value("name").toString();
                QString writer = temp.value("writer").toString();
                QString type = temp.value("type").toString();
                QString press = temp.value("press").toString();
                QString publicationDate = temp.value("publicationDate").toString();
                float price = temp.value("price").toDouble();
                int bookShelf = temp.value("bookShelf").toInt();

                oneBookButton->setISBN(ISBN);
                oneBookButton->setText("ISBN：" + ISBN + "\n书名：" + name + "\n作者：" + writer + "\n类别：" + type + "\n出版社：" + press+'\n书架编号：'+bookShelf);
                oneBookButton->move(0, (i-1) * oneBookButton->height());
                booksListWidget->resize(booksListWidget->width(), booksListWidget->height() + oneBookButton->height());

                Book *oneBook = new Book(ISBN, name, writer, type, press, publicationDate, price, bookShelf);
                booksList.append(oneBook);
            }
            this->booksListWidget->show();
        }
    }
}

float MainWindow::getBookPrice(BookButton *bookButton)
{
    for(int i=0; i<booksList.count(); i++)
    {
        if(bookButton->getISBN() == booksList[i]->getISBN())
        {
            return booksList[i]->getPrice();
        }
    }
    return -1;
}

QString MainWindow::getBookProperty(BookButton *bookButton, QString propertyName)
{
    if(propertyName == "name")
    {
        for(int i=0; i<booksList.count(); i++)
        {
            if(bookButton->getISBN() == booksList[i]->getISBN())
            {
                return booksList[i]->getName();
            }
        }
    }
    else if(propertyName == "writer")
    {
        for(int i=0; i<booksList.count(); i++)
        {
            if(bookButton->getISBN() == booksList[i]->getISBN())
            {
                return booksList[i]->getWriter();
            }
        }
    }
    else if(propertyName == "type")
    {
        for(int i=0; i<booksList.count(); i++)
        {
            if(bookButton->getISBN() == booksList[i]->getISBN())
            {
                return booksList[i]->getType();
            }
        }
    }
    return "";
}
