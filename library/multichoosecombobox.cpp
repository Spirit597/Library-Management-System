#include "multichoosecombobox.h"

MultiChooseComboBox::MultiChooseComboBox()
{

    typeList = new QListWidget(this);
    typeChosen = new QLineEdit(this);
    connect(typeChosen, &QLineEdit::textChanged, this, &MultiChooseComboBox::dealTextChanged);

    QListWidgetItem *typeItem1 = new QListWidgetItem;
    typeList->addItem(typeItem1);
    QCheckBox *typeCheckBox1 = new QCheckBox(this);
    typeCheckBox1->setText("A 马克思主义、列宁主义、毛泽东思想、邓小平理论");
   // typeList->addItem(typeCheckBox);
    typeList->setItemWidget(typeItem1, typeCheckBox1);
    connect(typeCheckBox1, &QCheckBox::stateChanged, this, &MultiChooseComboBox::dealStateChanged);


    QListWidgetItem *typeItem2 = new QListWidgetItem;
    typeList->addItem(typeItem2);
    QCheckBox *typeCheckBox2 = new QCheckBox(this);
    typeCheckBox2->setText("B 哲学、宗教");
   // typeList->addItem(typeCheckBox);
    typeList->setItemWidget(typeItem2, typeCheckBox2);
    connect(typeCheckBox2, &QCheckBox::stateChanged, this, &MultiChooseComboBox::dealStateChanged);

    QListWidgetItem *typeItem3 = new QListWidgetItem;
    typeList->addItem(typeItem3);
    QCheckBox *typeCheckBox3 = new QCheckBox(this);
    typeCheckBox3->setText("C 社会科学总论");
   // typeList->addItem(typeCheckBox);
    typeList->setItemWidget(typeItem3, typeCheckBox3);
    connect(typeCheckBox3, &QCheckBox::stateChanged, this, &MultiChooseComboBox::dealStateChanged);

    QListWidgetItem *typeItem4 = new QListWidgetItem;
    typeList->addItem(typeItem4);
    QCheckBox *typeCheckBox4 = new QCheckBox(this);
    typeCheckBox4->setText("D 政治、法律");
   // typeList->addItem(typeCheckBox);
    typeList->setItemWidget(typeItem4, typeCheckBox4);
    connect(typeCheckBox4, &QCheckBox::stateChanged, this, &MultiChooseComboBox::dealStateChanged);

    QListWidgetItem *typeItem5 = new QListWidgetItem;
    typeList->addItem(typeItem5);
    QCheckBox *typeCheckBox5 = new QCheckBox(this);
    typeCheckBox5->setText("I 文学");
   // typeList->addItem(typeCheckBox);
    typeList->setItemWidget(typeItem5, typeCheckBox5);
    connect(typeCheckBox5, &QCheckBox::stateChanged, this, &MultiChooseComboBox::dealStateChanged);

    QListWidgetItem *typeItem6 = new QListWidgetItem;
    typeList->addItem(typeItem6);
    QCheckBox *typeCheckBox6 = new QCheckBox(this);
    typeCheckBox6->setText("J 艺术");
   // typeList->addItem(typeCheckBox);
    typeList->setItemWidget(typeItem6, typeCheckBox6);
    connect(typeCheckBox6, &QCheckBox::stateChanged, this, &MultiChooseComboBox::dealStateChanged);

    QListWidgetItem *typeItem7 = new QListWidgetItem;
    typeList->addItem(typeItem7);
    QCheckBox *typeCheckBox7 = new QCheckBox(this);
    typeCheckBox7->setText("K 历史、地理");
   // typeList->addItem(typeCheckBox);
    typeList->setItemWidget(typeItem7, typeCheckBox7);
    connect(typeCheckBox7, &QCheckBox::stateChanged, this, &MultiChooseComboBox::dealStateChanged);

    this->setModel(typeList->model());
    this->setView(typeList);
    this->setLineEdit(typeChosen);
    typeChosen->setReadOnly(true);
}

void MultiChooseComboBox::dealTextChanged()
{
    typeChosen->setText(typeChosenStr);
}

void MultiChooseComboBox::dealStateChanged(int state)
{
    typeChosenStr = "";
    QObject *object = QObject::sender();
    QCheckBox *senderCheckBox = (QCheckBox *)object;
    for(int i=0; i<typeList->count(); i++)
    {
        QListWidgetItem *tempItem = typeList->item(i);
        QWidget *tempWidget = typeList->itemWidget(tempItem);
        QCheckBox *tempCheckBox = (QCheckBox *)tempWidget;
        if(tempCheckBox->isChecked())
        {
            typeChosenStr.append(tempCheckBox->text()).append(";");
        }
    }
    if(typeChosenStr.endsWith(";"))
    {
        typeChosenStr = typeChosenStr.left(typeChosenStr.length()-1);
    }
    typeChosen->setText(typeChosenStr);

}

