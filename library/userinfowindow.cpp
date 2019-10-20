#include "userinfowindow.h"

UserInfoWindow::UserInfoWindow(QWidget *parent) : QWidget(parent)
{

    this->resize(600, 400);
    this->setWindowTitle("个人信息");

    IDLabel = new QLabel("您的ID是：", this);
    IDLabel->move(20, 44);

    roleLabel = new QLabel("您的身份是：", this);
    roleLabel->move(20, 84);

    nameLabel = new QLabel("您的用户名是：", this);
    nameLabel->move(20, 124);

    primaryPasswordLabel = new QLabel("请输入您的原始密码：", this);
    primaryPasswordLabel->move(20, 164);

    newPasswordLabel = new QLabel("请输入您的新密码：", this);
    newPasswordLabel->move(20, 204);

    newPasswordAgainLabel = new QLabel("请再次输入新密码：", this);
    newPasswordAgainLabel->move(20, 244);

    maxBookNumberLabel = new QLabel("您可同时持有书籍的最大数量为：", this);
    maxBookNumberLabel->move(20, 284);

    maxDaysLabel = new QLabel("您每本书的借阅最长天数为：", this);
    maxDaysLabel->move(20, 324);

    IDLineEdit = new QLineEdit(this);
    IDLineEdit->setReadOnly(true);
    IDLineEdit->resize(330, 20);
    IDLineEdit->move(110, 40);

    roleLineEdit = new QLineEdit(this);
    roleLineEdit->setReadOnly(true);
    roleLineEdit->resize(330, 20);
    roleLineEdit->move(110, 80);

    nameLineEdit = new QLineEdit(this);
    nameLineEdit->resize(330, 20);
    nameLineEdit->move(110, 120);

    primaryPasswordLineEdit = new QLineEdit(this);
    primaryPasswordLineEdit->resize(300, 20);
    primaryPasswordLineEdit->move(140, 160);

    newPasswordLineEdit = new QLineEdit(this);
    newPasswordLineEdit->resize(300, 20);
    newPasswordLineEdit->move(140, 200);

    newPasswordAgainLineEdit = new QLineEdit(this);
    newPasswordAgainLineEdit->resize(300, 20);
    newPasswordAgainLineEdit->move(140, 240);


    maxBookNumberLineEdit = new QLineEdit(this);
    maxBookNumberLineEdit->resize(230, 20);
    maxBookNumberLineEdit->move(210, 280);
    maxBookNumberLineEdit->setValidator(new QIntValidator(0, 99999, this));

    maxDaysLineEdit = new QLineEdit(this);
    maxDaysLineEdit->resize(230, 20);
    maxDaysLineEdit->move(210, 320);
    maxDaysLineEdit->setValidator(new QIntValidator(0, 99999, this));


    hideOrShowPasswordInput("hide");
    setReadOnly(true);

    editButton = new QPushButton("编辑", this);
    editButton->resize(50, 30);
    editButton->move(480, 350);
    connect(editButton, &QPushButton::clicked, this, &UserInfoWindow::dealEdit);

    editPasswordButton = new QPushButton("修改密码", this);
    editPasswordButton->resize(60, 30);
    editPasswordButton->move(480, 160);
    editPasswordButton->hide();
    connect(editPasswordButton, &QPushButton::clicked, this, &UserInfoWindow::dealEditPassword);

    cancelEditPasswordButton = new QPushButton("取消修改密码", this);
    cancelEditPasswordButton->resize(80, 30);
    cancelEditPasswordButton->move(480, 160);
    cancelEditPasswordButton->hide();
    connect(cancelEditPasswordButton, &QPushButton::clicked, this, &UserInfoWindow::dealCancelEditPassword);

    saveButton = new QPushButton("保存", this);
    saveButton->resize(50, 30);
    saveButton->move(480, 350);
    saveButton->hide();
    connect(saveButton, &QPushButton::clicked, this, &UserInfoWindow::dealSave);
}

void UserInfoWindow::hideOrShowPasswordInput(QString op)
{
    if(op == "hide")
    {
        this->primaryPasswordLabel->hide();
        this->newPasswordLabel->hide();
        this->newPasswordAgainLabel->hide();
        this->primaryPasswordLineEdit->hide();
        this->newPasswordLineEdit->hide();
        this->newPasswordAgainLineEdit->hide();

        this->primaryPasswordLineEdit->clear();
        this->newPasswordLineEdit->clear();
        this->newPasswordAgainLineEdit->clear();
    }
    else if(op == "show")
    {
        this->primaryPasswordLabel->show();
        this->newPasswordLabel->show();
        this->newPasswordAgainLabel->show();
        this->primaryPasswordLineEdit->show();
        this->newPasswordLineEdit->show();
        this->newPasswordAgainLineEdit->show();
    }
}

void UserInfoWindow::setReadOnly(bool op)
{
    if(op)
    {

        nameLineEdit->setReadOnly(true);
        maxBookNumberLineEdit->setReadOnly(true);
        maxDaysLineEdit->setReadOnly(true);

    }
    else
    {

        nameLineEdit->setReadOnly(false);

        if(currentUserRole == "admin")
        {
            maxBookNumberLineEdit->setReadOnly(false);
            maxDaysLineEdit->setReadOnly(false);
        }
    }
}

void UserInfoWindow::dealEdit()
{

    this->saveButton->show();
    this->editPasswordButton->show();
    this->editButton->hide();

    this->setReadOnly(false);


}

void UserInfoWindow::dealSave()
{
    if(cancelEditPasswordButton->isVisible())
    {
        if(primaryPasswordLineEdit->text() == "")
        {
            QMessageBox::StandardButton info = QMessageBox::information(NULL, "Error", "原始密码的输入不能为空");
            return;
        }
        if(newPasswordLineEdit->text() == "")
        {
            QMessageBox::StandardButton info = QMessageBox::information(NULL, "Error", "新密码的输入不能为空");
            return;
        }
        if(newPasswordAgainLineEdit->text() == "")
        {
            QMessageBox::StandardButton info = QMessageBox::information(NULL, "Error", "新密码的再次输入不能为空");
            return;
        }
        if(newPasswordLineEdit->text() != newPasswordAgainLineEdit->text())
        {
            QMessageBox::StandardButton info = QMessageBox::information(NULL, "Error", "您两次输入的新密码不一致");
            return;
        }
    }
    QJsonObject editUserInfoPackage;

    editUserInfoPackage.insert("type", "edit user information");
    editUserInfoPackage.insert("id", this->IDLineEdit->text());
    editUserInfoPackage.insert("role", this->thisRole);
    editUserInfoPackage.insert("name", this->nameLineEdit->text());
    if(cancelEditPasswordButton->isVisible())//已经修改了密码
    {
        editUserInfoPackage.insert("isPasswordEdited", true);
        editUserInfoPackage.insert("priPassword", this->primaryPasswordLineEdit->text());
        editUserInfoPackage.insert("newPassword", this->newPasswordLineEdit->text());
    }
    else
    {
        editUserInfoPackage.insert("isPasswordEdited", false);
    }
    if(this->thisRole == "reader")
    {
        if(maxBookNumberLineEdit->text() == "")
        {
            QMessageBox::StandardButton info = QMessageBox::information(NULL, "Error", "可同时持有书籍的最大数量的输入不能为空");
            return;
        }
        editUserInfoPackage.insert("maxBookNumber", this->maxBookNumberLineEdit->text().toInt());

        if(maxDaysLineEdit->text() == "")
        {
            QMessageBox::StandardButton info = QMessageBox::information(NULL, "Error", "每本书的借阅最长天数不能为空");
            return;
        }
        editUserInfoPackage.insert("maxDays", this->maxDaysLineEdit->text().toInt());
        editUserInfoPackage.insert("isNumberAndDaysEdited", true);
    }
    else
    {
        editUserInfoPackage.insert("isNumberAndDaysEdited", false);
    }


    QByteArray byte_array = QJsonDocument(editUserInfoPackage).toJson();
    tcpClient->write(byte_array);
    if(tcpClient->waitForReadyRead(1000))//阻塞式连接
    {
        QByteArray result = tcpClient->readAll();
        if(!result.isEmpty())
        {
            QJsonObject resultInfo = QJsonDocument::fromJson(result).object();
            if(resultInfo.value("result").toString() == "suc")
            {
                QMessageBox::StandardButton info = QMessageBox::information(NULL, "", "修改资料成功");
                if(info == QMessageBox::Ok)
                {
                    this->close();
                }
            }
            else if(resultInfo.value("result").toString() == "primary password wrong")
            {
                QMessageBox::StandardButton info = QMessageBox::information(NULL, "Error", "您输入的原始密码错误");
            }
            else
            {
                QMessageBox::StandardButton info = QMessageBox::information(NULL, "Error", "修改资料失败");
                if(info == QMessageBox::Ok)
                {
                    this->close();
                }
            }
        }
    }

}

void UserInfoWindow::dealEditPassword()
{
    this->editPasswordButton->hide();
    this->cancelEditPasswordButton->show();
    this->hideOrShowPasswordInput("show");
}

void UserInfoWindow::dealCancelEditPassword()
{
    this->cancelEditPasswordButton->hide();
    this->editPasswordButton->show();
    this->hideOrShowPasswordInput("hide");
}

void UserInfoWindow::setRole(QString role1, QString role2)
{
    this->currentUserRole = role1;
    this->thisRole = role2;
}

void UserInfoWindow::chooseRole()
{
    if(this->thisRole == "admin")
    {
        this->maxBookNumberLabel->hide();
        this->maxBookNumberLineEdit->hide();
        this->maxDaysLabel->hide();
        this->maxDaysLineEdit->hide();
    }
}

void UserInfoWindow::getUserInfo(QTcpSocket *tcpClient, QString ID)
{
    this->tcpClient = tcpClient;
    QJsonObject getUserInfoPackage;

    getUserInfoPackage.insert("type", "get user information");
    getUserInfoPackage.insert("role", this->thisRole);
    getUserInfoPackage.insert("id", ID);

    QByteArray byte_array = QJsonDocument(getUserInfoPackage).toJson();
    tcpClient->write(byte_array);
    if(tcpClient->waitForReadyRead(1000))//阻塞式连接
    {
        QByteArray result = tcpClient->readAll();
        if(!result.isEmpty())
        {
            QJsonObject resultInfo = QJsonDocument::fromJson(result).object();
            this->IDLineEdit->setText(ID);
            if(this->thisRole == "admin")
            {
                this->roleLineEdit->setText("管理员");
                this->nameLineEdit->setText(resultInfo.value("name").toString());
            }
            else if(this->thisRole == "reader")
            {
                this->roleLineEdit->setText("读者");
                this->nameLineEdit->setText(resultInfo.value("name").toString());
                this->maxBookNumberLineEdit->setText(QString::number(resultInfo.value("maxBookNumber").toInt()));
                this->maxDaysLineEdit->setText(QString::number(resultInfo.value("maxDays").toInt()));
            }
        }
    }
}
