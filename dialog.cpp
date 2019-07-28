#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(QStringLiteral("ȷ��"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(QStringLiteral("ȡ��"));
    ui->buttonBox->button(QDialogButtonBox::RestoreDefaults)->setText(QStringLiteral("ɾ��"));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setFocus();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->standardButton(button) == QDialogButtonBox::RestoreDefaults){
        emit signalDelete();
    }
}

void Dialog::on_pushButton_clicked()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, "��ѡ���ļ������ļ���", "/");
    if (dirPath.isEmpty()){
        return;
    }
    else{
        ui->label->setText(dirPath);
    }
}
