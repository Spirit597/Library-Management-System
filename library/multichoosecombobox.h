#ifndef MULTICHOOSECOMBOBOX_H
#define MULTICHOOSECOMBOBOX_H

#include <QComboBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLineEdit>
#include <QCheckBox>
#include <QDebug>

class MultiChooseComboBox : public QComboBox
{
public:
    MultiChooseComboBox();
    void dealTextChanged();
    void dealStateChanged(int state);

private:
    QListWidget *typeList;
    QLineEdit *typeChosen;
    QString typeChosenStr;
};

#endif // MULTICHOOSECOMBOBOX_H
