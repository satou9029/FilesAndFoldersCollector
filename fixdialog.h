#ifndef FIXDIALOG_H
#define FIXDIALOG_H

#pragma execution_character_set("utf-8")

#include <QDialog>
#include <QAbstractButton>

#include <QFileDialog>

namespace Ui {
class FixDialog;
}

class FixDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FixDialog(QWidget *parent = 0);
    ~FixDialog();
    QString use_for_find_section_in_ini;

private slots:
    void on_pushButton_clicked();

private:
    Ui::FixDialog *ui;
};

#endif // FIXDIALOG_H
