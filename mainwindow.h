#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDateTime>
#include "Windows.h"
#include <QProcess>

#include <QMainWindow>
#include <QScrollBar>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>

#include "additem.h"
#include "dialog.h"
#include "fixdialog.h"

#include <QFileInfo>
#include <QSettings>
#include <QFileInfo>
#include <QFileInfoList>

#include <QDebug>

#pragma execution_character_set("utf-8")
#pragma comment(lib, "shell32")

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_2_clicked();

    void on_lineEdit_textChanged();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_8_clicked();

private:
    void refreshItemList();
    QVariantList selectData(QString &section);
    QString use_for_find_section_in_ini;
    Ui::MainWindow *ui;
    QDialog *aditem;
    QDialog *dlg;
    QDialog *fixsetter;
    QStringList copiedDataPathList;
};

#endif // MAINWINDOW_H
