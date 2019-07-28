#include "fixdialog.h"
#include "ui_fixdialog.h"

FixDialog::FixDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FixDialog)
{
    ui->setupUi(this);
}

FixDialog::~FixDialog()
{
    delete ui;
}

void FixDialog::on_pushButton_clicked()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, "��ѡ��ѹ�������Ŀ¼", "/");
    if (dirPath.isEmpty()){
        return;
    }
    else{
        ui->label_3->setText(dirPath);
    }
}
