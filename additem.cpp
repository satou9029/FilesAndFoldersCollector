#include "additem.h"
#include "ui_additem.h"

AddItem::AddItem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddItem)
{
    ui->setupUi(this);
    ui->lineEdit->setFocus();
}

AddItem::~AddItem()
{
    delete ui;
}
