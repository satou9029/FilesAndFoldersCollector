#ifndef DIALOG_H
#define DIALOG_H

#pragma execution_character_set("utf-8")

#include <QDialog>
#include <QAbstractButton>

#include <QFileDialog>

#include <QDebug>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    QString use_for_find_spli_in_ini;
    QString use_for_find_section_in_ini;

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

    void on_pushButton_clicked();

signals:
    void signalDelete();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
