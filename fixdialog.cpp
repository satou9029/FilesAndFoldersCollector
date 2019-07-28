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
    QString dirPath = QFileDialog::getExistingDirectory(this, "请选择压缩包输出目录", "/");
    if (dirPath.isEmpty()){
        return;
    }
    else{
        ui->label_3->setText(dirPath);
    }
}
