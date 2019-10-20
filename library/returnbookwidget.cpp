#include "returnbookwidget.h"

ReturnBookWidget::ReturnBookWidget(QWidget *parent) : QFrame(parent)
{
    this->resize(500, 100);
    this->setFrameStyle(QFrame::Box | QFrame::Sunken);
    this->setLineWidth(3);


    ISBN = new QLabel(this);
    ISBN->setMinimumWidth(420);
    ISBN->move(10, 5);

    name = new QLabel(this);
    name->setMinimumWidth(420);
    name->move(10, 20);

    writer = new QLabel(this);
    writer->setMinimumWidth(420);
    writer->move(10, 35);

    type = new QLabel(this);
    type->setMinimumWidth(420);
    type->move(10, 50);

    days = new QLabel(this);
    days->setMinimumWidth(210);
    days->move(10, 65);

    extraDays = new QLabel(this);
    extraDays->setMinimumWidth(210);
    extraDays->move(220, 65);

    debt = new QLabel(this);
    debt->setMinimumWidth(420);
    debt->move(10, 80);


    returnBookButton = new QPushButton("还书", this);
    connect(returnBookButton, &QPushButton::clicked, this, &ReturnBookWidget::dealReturn);
    returnBookButton->resize(50, 30);
    returnBookButton->move(440, 20);

    repayDebtButton = new QPushButton("还款", this);
    connect(repayDebtButton, &QPushButton::clicked, this, &ReturnBookWidget::dealRepayDebt);
    repayDebtButton->resize(50, 30);
    repayDebtButton->move(440, 60);
    repayDebtButton->hide();
}

void ReturnBookWidget::setISBN(QString ISBN)
{
    this->ISBN->setText("ISBN：" + ISBN);
}

void ReturnBookWidget::setName(QString name)
{
    this->name->setText("书名：" + name);
}

void ReturnBookWidget::setWriter(QString writer)
{
    this->writer->setText("作者：" + writer);
}

void ReturnBookWidget::setType(QString type)
{
    this->type->setText("类别：" + type);
}

void ReturnBookWidget::setDays(int days)
{
    this->days->setText("已借阅天数：" + QString::number(days));
}

void ReturnBookWidget::setExtraDays(int extraDays)
{
    this->extraDays->setText("已超时天数：" + QString::number(extraDays));
}

void ReturnBookWidget::setDebt(float debt)
{
    if(debt > 0)
    {
        repayDebtButton->show();
    }
    this->debt->setText("罚款：" + QString::number(debt, 'f', 2) + "元");
}

void ReturnBookWidget::setIndex(int index)
{
    this->index = index;
}

void ReturnBookWidget::setTcpClient(QTcpSocket *tcpClient)
{
    this->tcpClient = tcpClient;
}

void ReturnBookWidget::indexMinusOne()
{
    this->index --;
}

QString ReturnBookWidget::getISBN()
{
    return ISBN->text();
}

void ReturnBookWidget::dealReturn()
{
    QJsonObject returnBookPackage;

    returnBookPackage.insert("type", "return book");
    returnBookPackage.insert("ISBN", this->ISBN->text().mid(5));

    QByteArray byte_array = QJsonDocument(returnBookPackage).toJson();
    tcpClient->write(byte_array);
    if(tcpClient->waitForReadyRead(1000))//阻塞式连接
    {
        QByteArray result = tcpClient->readAll();
        if(!result.isEmpty())
        {
            QJsonObject resultInfo = QJsonDocument::fromJson(result).object();
            if(resultInfo.value("result").toString() == "suc")
            {
                QMessageBox::StandardButton info = QMessageBox::information(NULL, "", "您本次申请还书的操作成功");
                emit indexSignal(this->index);
            }
            else if(resultInfo.value("result").toString() == "in debt")
            {
                QMessageBox::StandardButton info = QMessageBox::information(NULL, "", "请您先缴纳罚款之后进行还书申请");
            }
            else
            {
                QMessageBox::StandardButton info = QMessageBox::information(NULL, "Error", "您本次申请还书的操作失败");
            }
        }

    }

}

void ReturnBookWidget::dealRepayDebt()
{
    QJsonObject repayDebtPackage;

    repayDebtPackage.insert("type", "repay debt");
    repayDebtPackage.insert("ISBN", this->ISBN->text().mid(5));

    QByteArray byte_array = QJsonDocument(repayDebtPackage).toJson();
    tcpClient->write(byte_array);
    if(tcpClient->waitForReadyRead(1000))//阻塞式连接
    {
        QByteArray result = tcpClient->readAll();
        if(!result.isEmpty())
        {
            QJsonObject resultInfo = QJsonDocument::fromJson(result).object();
            if(resultInfo.value("result").toString() == "suc")
            {
                QMessageBox::StandardButton info = QMessageBox::information(NULL, "", "您本次缴纳罚款的操作成功");
                repayDebtButton->hide();
                this->setDebt(0);
            }
            else
            {
                QMessageBox::StandardButton info = QMessageBox::information(NULL, "", "您本次缴纳罚款的操作失败");
            }
        }
    }
}
