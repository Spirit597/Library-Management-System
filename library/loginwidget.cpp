#include "loginwidget.h"

LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("登陆");
    this->resize(300, 200);

    IDLabel = new QLabel("账号：");
    IDLabel->setParent(this);
    IDLabel->move(20, 15);
    passWordLabel = new QLabel("密码：");
    passWordLabel->setParent(this);
    passWordLabel->move(20, 45);

    IDLineEdit = new QLineEdit;
    IDLineEdit->setParent(this);
    IDLineEdit->move(100, 10);
    passwordLineEdit = new QLineEdit;
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setParent(this);
    passwordLineEdit->move(100, 40);

    readerLoginButton = new QRadioButton(this);
    readerLoginButton->setText("我是读者");
    readerLoginButton->move(20, 70);
    adminLoginButton = new QRadioButton(this);
    adminLoginButton->setText("我是管理员");
    adminLoginButton->move(180, 70);

    loginButton = new QPushButton("登陆");
    loginButton->setParent(this);
    loginButton->move(110, 110);

    connect(loginButton, &QPushButton::clicked, this, &LoginWidget::sendLoginSignal);


}

void LoginWidget::sendLoginSignal()
{
    if(readerLoginButton->isChecked())
    {
        emit loginClicked(IDLineEdit->text(), passwordLineEdit->text(), "reader");
    }
    else if(adminLoginButton->isChecked())
    {
        emit loginClicked(IDLineEdit->text(), passwordLineEdit->text(), "admin");
    }
    else
    {
        QMessageBox::StandardButton info = QMessageBox::information(NULL, "Error", "请先选择登录方式");
    }
}

void LoginWidget::clearPasswordLineEdit()
{
    this->passwordLineEdit->clear();
}
