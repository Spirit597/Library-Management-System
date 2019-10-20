#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>

class LoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWidget(QWidget *parent = 0);

    void sendLoginSignal();

    void clearPasswordLineEdit();
private:
    QRadioButton *readerLoginButton;
    QRadioButton *adminLoginButton;

    QLabel *IDLabel;
    QLabel *passWordLabel;

    QLineEdit *IDLineEdit;
    QLineEdit *passwordLineEdit;

    QPushButton *loginButton;

signals:
    void loginClicked(QString ID, QString password, QString role);

public slots:
};

#endif // LOGINWIDGET_H
