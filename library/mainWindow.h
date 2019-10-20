#ifndef MainWindow_H
#define MainWindow_H

#include <QFrame>

#include <QTcpSocket>

#include "book.h"

#include "multichoosecombobox.h"
#include "bookbutton.h"
#include "bookdetailwidget.h"
#include "returnbookwindow.h"
#include "libraryinfowindow.h"
#include "userinfowindow.h"

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

    void dealSortingMethodChoose(int index);
    void dealSearchingMethodChoose(int index);
    void dealSearch();
    void dealDeleteBook(QString ISBN);
    void dealEditBookSuc(QString ISBN, QString name, QString wri, QString type, QString press, QString pubDate, float price);

    void upDateCurrentUser(QString ID, QString role);
    //void offerCurrentUserAndSocket();

    void setTcpClient(QTcpSocket* tcpClient);

    void createReturnBookWin();
    void createViewLibraryInfoWin();
    void createUserInfoWin();
    void createBookDetailWin(QString ISBN);
    void createNewBook();

    void sendLogoutSignal();

    void changeButton(QString role);

    void getBooksInfo();

    float getBookPrice(BookButton *bookButton);
    QString getBookProperty(BookButton *bookButton, QString propertyName);

private:


    QFrame *topBar;

    QPushButton *viewUserInfoButton;
    QPushButton *viewLibraryInfoButton;
    QPushButton *logOutButton;
    QPushButton *returnBooksButton;
    QPushButton *searchButton;
    QPushButton *refreshBooksButton;
    QPushButton *createBookButton;

    QComboBox *searchingMethodList;
    QComboBox *sortingMethodList;
    QLineEdit *searchingInput;
    MultiChooseComboBox *typeList;

    QWidget *booksListWidget;
    QScrollArea *scrollArea;

    QString currentUserID = "";
    QString currentUserRole = "";

    QTcpSocket *tcpClient;

    QVector <Book *> booksList;
    QVector <BookButton *> booksButtonsList;
    QVector <BookButton *> matchedBooksButtonsList;


signals:
    //void currentUserInfoAndSocket(QString ID, QString role, QTcpSocket *tcpClient);

    void logoutSignal();

public slots:
};

#endif // MainWindow_H
