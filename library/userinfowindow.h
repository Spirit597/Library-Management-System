#ifndef USERINFOWINDOW_H
#define USERINFOWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QIntValidator>

#include <QJsonObject>
#include <QJsonDocument>

#include <QTcpSocket>

class UserInfoWindow : public QWidget
{
    Q_OBJECT
public:
    explicit UserInfoWindow(QWidget *parent = 0);

    void hideOrShowPasswordInput(QString op);
    void setReadOnly(bool op);
    void dealEdit();
    void dealSave();
    void dealEditPassword();
    void dealCancelEditPassword();

    void setRole(QString role1, QString role2);//前者对应操作者，后者对应被查看者
    void chooseRole();

    void getUserInfo(QTcpSocket *tcpClient, QString ID);

private:
    QLabel *IDLabel;
    QLabel *roleLabel;
    QLabel *nameLabel;
    QLabel *primaryPasswordLabel;
    QLabel *newPasswordLabel;
    QLabel *newPasswordAgainLabel;
    QLabel *maxBookNumberLabel;
    QLabel *maxDaysLabel;

    QLineEdit *IDLineEdit;
    QLineEdit *roleLineEdit;
    QLineEdit *nameLineEdit;
    QLineEdit *primaryPasswordLineEdit;
    QLineEdit *newPasswordLineEdit;
    QLineEdit *newPasswordAgainLineEdit;
    QLineEdit *maxBookNumberLineEdit;
    QLineEdit *maxDaysLineEdit;

    QPushButton *editButton;
    QPushButton *editPasswordButton;
    QPushButton *cancelEditPasswordButton;
    QPushButton *saveButton;

    QString currentUserRole = "";//操作者的角色
    QString thisRole = "";//该窗口显示资料对应的角色（即被查看者的角色）

    QTcpSocket *tcpClient = NULL;

signals:

public slots:
};

#endif // USERINFOWINDOW_H
