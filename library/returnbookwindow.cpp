#include "returnbookwindow.h"

ReturnBookWindow::ReturnBookWindow(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle("已借阅的图书");
    this->resize(600, 400);

    borrowedBookListWidget = new QWidget;
    borrowedBookListWidget->setMinimumWidth(500);
    borrowedBookListWidget->resize(500, 0);

    bookNumLabel = new QLabel(this);
    bookNumLabel->move(40, 20);

    scrollArea = new QScrollArea(this);
    scrollArea->resize(520,350);
    scrollArea->move(40, 50);
    scrollArea->setWidget(borrowedBookListWidget);




}

void ReturnBookWindow::getBorrowedBookInfo(QString ID, QTcpSocket *tcpClient)
{
    QJsonObject getBorrowedBookPackage;

    getBorrowedBookPackage.insert("type", "get borrowed");
    getBorrowedBookPackage.insert("id", ID);

    QByteArray byte_array = QJsonDocument(getBorrowedBookPackage).toJson();
    tcpClient->write(byte_array);
    if(tcpClient->waitForReadyRead(1000))//阻塞式连接
    {
        QByteArray result = tcpClient->readAll();
        if(!result.isEmpty())
        {
            QJsonObject resultInfo = QJsonDocument::fromJson(result).object();
            bookNum = resultInfo.count();
            for(int i=1; i<=resultInfo.count(); i++)
            {
                QJsonObject temp = resultInfo.value("book" + QString::number(i)).toObject();
                ReturnBookWidget *oneBorrowedBook = new ReturnBookWidget(borrowedBookListWidget);
                oneBorrowedBook->setIndex(i - 1);
                returnBookWidgets.append(oneBorrowedBook);
                connect(oneBorrowedBook, &ReturnBookWidget::indexSignal, this, &ReturnBookWindow::refreshReturnBookWin);

                oneBorrowedBook->setTcpClient(tcpClient);

                oneBorrowedBook->setISBN(temp.value("ISBN").toString());
                oneBorrowedBook->setName(temp.value("name").toString());
                oneBorrowedBook->setWriter(temp.value("writer").toString());
                oneBorrowedBook->setType(temp.value("type").toString());
                oneBorrowedBook->setDays(temp.value("days").toInt());
                if(temp.value("days").toInt() - temp.value("maxDays").toInt() > 0)
                {
                    oneBorrowedBook->setExtraDays(temp.value("days").toInt() - temp.value("maxDays").toInt());
                }
                else
                {
                    oneBorrowedBook->setExtraDays(0);
                }
                oneBorrowedBook->setDebt(temp.value("debt").toDouble());

                oneBorrowedBook->move(0, (i - 1) * oneBorrowedBook->height());
                borrowedBookListWidget->resize(borrowedBookListWidget->width(), borrowedBookListWidget->height() + oneBorrowedBook->height());

            }
            if(bookNum == 0)
            {
                bookNumLabel->setText("您当前未借阅书籍");
            }
            else
            {
                bookNumLabel->setText("您一共借阅了" + QString::number(bookNum) + "本书");
            }
        }
    }
}

void ReturnBookWindow::refreshReturnBookWin(int index)
{
    borrowedBookListWidget->resize(borrowedBookListWidget->width(), borrowedBookListWidget->height() - returnBookWidgets[index]->height());
    delete returnBookWidgets[index];
    returnBookWidgets[index] = NULL;
    returnBookWidgets.remove(index);
    bookNum --;
    if(bookNum == 0)
    {
        bookNumLabel->setText("您当前未借阅书籍");
    }
    else
    {
        bookNumLabel->setText("您一共借阅了" + QString::number(bookNum) + "本书");
    }

    for(int i = index; bookNum>0 && i<bookNum; i++)
    {
        returnBookWidgets[i]->indexMinusOne();
        returnBookWidgets[i]->move(0, i * returnBookWidgets[i]->height());

    }
    this->show();
}
