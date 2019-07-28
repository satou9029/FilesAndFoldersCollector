#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // �����߲���������������˳��ʹ�ö�̬��ӵ�Ԫ�ؾ�����������������
    // ����������ɵİ�ť��û��Ԫ�ص�VBoxLayout�о��е����⣨���spacer��һ����widget��
    QVBoxLayout* sal = dynamic_cast<QVBoxLayout *> (ui->scrollAreaWidgetContents->layout());
    sal->setDirection(QBoxLayout::BottomToTop);
    QVBoxLayout* mal = dynamic_cast<QVBoxLayout *> (ui->scrollAreaWidgetContents_2->layout());
    mal->setDirection(QBoxLayout::BottomToTop);
    //���config.ini�����ļ�����������ھʹ���һ���µģ�����¼����ʱ�䡣
    QFileInfo fileInfo("config.ini");
    if(!fileInfo.isFile())
    {
        QSettings *configIni = new QSettings("config.ini", QSettings::IniFormat);
        configIni->setIniCodec("UTF-8");
        configIni->setValue("init", QStringLiteral("����ʱ�䣺") + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        configIni->setValue("route", QApplication::applicationDirPath());
        configIni->setValue("compressor", QApplication::applicationDirPath() + "/WinRAR561/WinRAR.exe");
        configIni->sync();
        delete configIni;
    }
    refreshItemList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshItemList()
{
    QList<QPushButton* > btns = ui->scrollAreaWidgetContents->findChildren<QPushButton* >();
    foreach (QPushButton* btn, btns)
    {
        delete btn;
    }
    QSettings *configIni = new QSettings("config.ini", QSettings::IniFormat);
    configIni->setIniCodec("UTF-8");
    foreach (QString group, configIni->childGroups()) {
        QPushButton* btn = new QPushButton(ui->scrollAreaWidgetContents);
        // ����ť����ObjectName�� Ϊ�˵���½���ť�½�split��ģ������
        btn->setObjectName(configIni->value("/"+group+"/name").toString());
        btn->setMinimumHeight(50);
        btn->setStyleSheet("border-bottom: 1px solid rgb(74, 89, 146);");
        QVBoxLayout* layout_btn_inner=new QVBoxLayout();
        layout_btn_inner->setMargin(0);
        QLabel* btn_lb_name = new QLabel(btn);
        btn_lb_name->setObjectName("btn_lb_name");
        btn_lb_name->setAlignment(Qt::AlignCenter);
        btn_lb_name->setStyleSheet("font-size:16px; font-family:'Microsoft-YaHei'");
        btn_lb_name->setText(configIni->value("/"+group+"/name").toString());
        QLabel* btn_lb_section=new QLabel(btn);
        btn_lb_section->setVisible(false);
        btn_lb_section->setObjectName("btn_lb_section");
        btn_lb_section->setText("/"+group+"/");
        layout_btn_inner->addWidget(btn_lb_name);
        layout_btn_inner->addWidget(btn_lb_section);
        btn->setLayout(layout_btn_inner);
        connect(btn, &QPushButton::clicked, [this, btn](){
            use_for_find_section_in_ini = btn->findChild<QLabel *>("btn_lb_section")->text();
            ui->lineEdit->setText(btn->findChild<QLabel *>("btn_lb_name")->text());

            QList<QPushButton* > btns = ui->scrollAreaWidgetContents_2->findChildren<QPushButton* >();
            foreach (QPushButton* btn, btns)
            {
                delete btn;
            }

            QSettings *configIni = new QSettings("config.ini", QSettings::IniFormat);
            configIni->setIniCodec("UTF-8");
            int itemCounter = configIni->value(use_for_find_section_in_ini + "count").toInt();
            delete configIni;
            for(int i=1;i<=itemCounter;i++)
            {
                QSettings *configIni = new QSettings("config.ini", QSettings::IniFormat);
                configIni->setIniCodec("UTF-8");
                if(configIni->value(use_for_find_section_in_ini + "spliname_" + QString::number(i, 10)).toString() == "")
                {
                    continue;
                }
                QPushButton* btn = new QPushButton(ui->scrollAreaWidgetContents);
                QVBoxLayout* layout_btn_inner=new QVBoxLayout();
                QLabel* btn_lb_name = new QLabel(btn);
                btn_lb_name->setObjectName("btn_lb_name");
                btn_lb_name->setStyleSheet("font-size:20px; font-weight: bold; font-family:'Microsoft-YaHei'");

                btn_lb_name->setText(configIni->value(use_for_find_section_in_ini + "spliname_" + QString::number(i, 10)).toString());
                QLabel* btn_lb_path=new QLabel(btn);
                btn_lb_path->setObjectName("btn_lb_path");
                btn_lb_path->setStyleSheet("font-size:14px; font-family:'Microsoft-YaHei'");
                btn_lb_path->setText(configIni->value(use_for_find_section_in_ini + "path_" + QString::number(i, 10)).toString());
                delete configIni;
                layout_btn_inner->addWidget(btn_lb_name);
                layout_btn_inner->addWidget(btn_lb_path);
                btn->setLayout(layout_btn_inner);
                btn->setMinimumHeight(60);
                btn->setStyleSheet("text-align: left;");
                btn->setToolTip(QStringLiteral("����޸��������"));
                MainWindow* thatthatmw = this;
                this->connect(btn, &QPushButton::clicked, [thatthatmw, btn](){
                    Dialog* dlg = new Dialog;
                    dlg->use_for_find_section_in_ini = thatthatmw->use_for_find_section_in_ini;
                    dlg->use_for_find_spli_in_ini = btn->findChild<QLabel *>("btn_lb_name")->text();
                    // ���spli���ڣ�����dlg����ʾ�Ѵ����������
                    QSettings *configIni = new QSettings("config.ini", QSettings::IniFormat);
                    configIni->setIniCodec("UTF-8");
                    for(int i=1;i<=configIni->value(dlg->use_for_find_section_in_ini + "count").toInt();i++)
                    {
                        if(configIni->value(dlg->use_for_find_section_in_ini + "spliname_" + QString::number(i, 10)).toString() == dlg->use_for_find_spli_in_ini)
                        {
                            dlg->findChild<QLabel *>("label")->setText(configIni->value(dlg->use_for_find_section_in_ini + "path_" + QString::number(i, 10)).toString());
                            dlg->findChild<QCheckBox *>("checkBox")->setChecked(configIni->value(dlg->use_for_find_section_in_ini + "keywords_bool_" + QString::number(i, 10)).toBool());
                            dlg->findChild<QLineEdit *>("lineEdit")->setText(configIni->value(dlg->use_for_find_section_in_ini + "keywords_" + QString::number(i, 10)).toString());
                            dlg->findChild<QComboBox *>("comboBox_2")->setCurrentIndex(configIni->value(dlg->use_for_find_section_in_ini + "latest_" + QString::number(i, 10)).toInt());
                            dlg->findChild<QLineEdit *>("lineEdit_2")->setText(configIni->value(dlg->use_for_find_section_in_ini + "latest_b_" + QString::number(i, 10)).toString());
                            dlg->findChild<QLineEdit *>("lineEdit_3")->setText(configIni->value(dlg->use_for_find_section_in_ini + "latest_e_" + QString::number(i, 10)).toString());
                            dlg->findChild<QComboBox *>("comboBox")->setCurrentIndex(configIni->value(dlg->use_for_find_section_in_ini + "type_" + QString::number(i, 10)).toInt());
                            dlg->findChild<QCheckBox *>("checkBox_2")->setChecked(configIni->value(dlg->use_for_find_section_in_ini + "rename_" + QString::number(i, 10)).toBool());
                            dlg->findChild<QCheckBox *>("checkBox_3")->setChecked(configIni->value(dlg->use_for_find_section_in_ini + "fixwords_bool_" + QString::number(i, 10)).toBool());
                            dlg->findChild<QLineEdit *>("lineEdit_4")->setText(configIni->value(dlg->use_for_find_section_in_ini + "spliname_" + QString::number(i, 10)).toString());
                        }
                    }
                    delete configIni;
                    QPushButton* thatbtn = btn;
                    MainWindow* thatmw = thatthatmw;
                    thatmw->connect(dlg, &QDialog::accepted, [thatmw, dlg, thatbtn](){
                        QSettings *configIni = new QSettings("config.ini", QSettings::IniFormat);
                        configIni->setIniCodec("UTF-8");
                        // ��section���ɵ�spli��ť��ȷ��ֻ�����޸ĵ�ǰspli�����Ĺ���
                        QString count = "";
                        for(int i=1;i<=configIni->value(dlg->use_for_find_section_in_ini + "count").toInt();i++){
                            if(configIni->value(dlg->use_for_find_section_in_ini + "spliname_" + QString::number(i, 10)).toString() == dlg->use_for_find_spli_in_ini){
                                count = QString::number(i, 10);
                            }
                            if( (dlg->use_for_find_spli_in_ini != dlg->findChild<QLineEdit *>("lineEdit_4")->text().simplified()) && (configIni->value(dlg->use_for_find_section_in_ini + "spliname_" + QString::number(i, 10)).toString() == dlg->findChild<QLineEdit *>("lineEdit_4")->text().simplified())){
                                // ���׼���޸ĵ�Ŀ�����ظ�����������
                                qDebug() << "�޸�splitʧ�ܣ����Դ�����split���ظ���";
                                thatmw->findChild<QWidget *>("scrollAreaWidgetContents")->findChild<QPushButton *>(configIni->value(thatmw->use_for_find_section_in_ini + "name").toString())->click();
                                delete configIni;
                                return;
                            }
                        }
                        if(count == "")
                            return;
                        configIni->setValue(dlg->use_for_find_section_in_ini + "path_" + count, dlg->findChild<QLabel *>("label")->text());
                        configIni->setValue(dlg->use_for_find_section_in_ini + "keywords_bool_" + count, dlg->findChild<QCheckBox *>("checkBox")->checkState());
                        configIni->setValue(dlg->use_for_find_section_in_ini + "keywords_" + count, dlg->findChild<QLineEdit *>("lineEdit")->text());
                        configIni->setValue(dlg->use_for_find_section_in_ini + "latest_" + count, dlg->findChild<QComboBox *>("comboBox_2")->currentIndex());
                        configIni->setValue(dlg->use_for_find_section_in_ini + "latest_b_" + count, dlg->findChild<QLineEdit *>("lineEdit_2")->text());
                        configIni->setValue(dlg->use_for_find_section_in_ini + "latest_e_" + count, dlg->findChild<QLineEdit *>("lineEdit_3")->text());
                        configIni->setValue(dlg->use_for_find_section_in_ini + "type_" + count, dlg->findChild<QComboBox *>("comboBox")->currentIndex());
                        configIni->setValue(dlg->use_for_find_section_in_ini + "rename_" + count, dlg->findChild<QCheckBox *>("checkBox_2")->checkState());
                        configIni->setValue(dlg->use_for_find_section_in_ini + "fixwords_bool_" + count, dlg->findChild<QCheckBox *>("checkBox_3")->checkState());
                        configIni->setValue(dlg->use_for_find_section_in_ini + "newname_" + count, dlg->findChild<QLineEdit *>("lineEdit_4")->text());
                        configIni->setValue(dlg->use_for_find_section_in_ini + "spliname_" + count, (dlg->findChild<QLineEdit *>("lineEdit_4")->text().simplified() == "")?QString("����%1").arg(count):dlg->findChild<QLineEdit *>("lineEdit_4")->text());
                        thatbtn->findChild<QLabel *>("btn_lb_name")->setText((dlg->findChild<QLineEdit *>("lineEdit_4")->text().simplified() == "")?QString("����%1").arg(count):dlg->findChild<QLineEdit *>("lineEdit_4")->text());
                        thatbtn->findChild<QLabel *>("btn_lb_path")->setText(dlg->findChild<QLabel *>("label")->text());
                        configIni->sync();
                        delete configIni;
                    });
                    thatmw->connect(dlg, &Dialog::signalDelete, [dlg, thatbtn](){
                        QSettings *configIni = new QSettings("config.ini", QSettings::IniFormat);
                        configIni->setIniCodec("UTF-8");
                        // ��section���ɵ�spli��ť��ɾ���ض�ɾ��ini�ļ��е���Ŀ
                        for(int i=1;i<=configIni->value(dlg->use_for_find_section_in_ini + "count").toInt();i++){
                            if(configIni->value(dlg->use_for_find_section_in_ini + "spliname_" + QString::number(i, 10)).toString() == dlg->use_for_find_spli_in_ini){
                                configIni->remove(dlg->use_for_find_section_in_ini + "path_" + QString::number(i, 10));
                                configIni->remove(dlg->use_for_find_section_in_ini + "keywords_bool_" + QString::number(i, 10));
                                configIni->remove(dlg->use_for_find_section_in_ini + "keywords_" + QString::number(i, 10));
                                configIni->remove(dlg->use_for_find_section_in_ini + "latest_" + QString::number(i, 10));
                                configIni->remove(dlg->use_for_find_section_in_ini + "latest_b_" + QString::number(i, 10));
                                configIni->remove(dlg->use_for_find_section_in_ini + "latest_e_" + QString::number(i, 10));
                                configIni->remove(dlg->use_for_find_section_in_ini + "type_" + QString::number(i, 10));
                                configIni->remove(dlg->use_for_find_section_in_ini + "rename_" + QString::number(i, 10));
                                configIni->remove(dlg->use_for_find_section_in_ini + "newname_" + QString::number(i, 10));
                                configIni->remove(dlg->use_for_find_section_in_ini + "fixwords_bool_" + QString::number(i, 10));
                                configIni->remove(dlg->use_for_find_section_in_ini + "spliname_" + QString::number(i, 10));
                            }
                        }
                        delete configIni;
                        delete thatbtn;
                        dlg->close();
                    });
                    dlg->show();
                });
                ui->scrollAreaWidgetContents_2->layout()->addWidget(btn);
            };
        });
        ui->scrollAreaWidgetContents->layout()->addWidget(btn);
    };
    delete configIni;
}

QVariantList MainWindow::selectData(QString &section)
{
    /*
     * ���ڲ�ѯĿ�����ݲ������ļ����ļ���·��
     */

    QVariantList sourceData_path_rename_spliname_VList;

    qDebug() << "��ǰ�������section��" << section;
    if(section == "")
        return sourceData_path_rename_spliname_VList;

    qDebug() << "��ʼ���ң������в���������򣬲�����spliParaList��";
    QSettings *configIni = new QSettings("config.ini", QSettings::IniFormat);
    configIni->setIniCodec("UTF-8");
    int itemCounter = configIni->value(section + "count").toInt();
    QVariantList spliParaList;
    for(int i=1;i<=itemCounter;i++)
    {
        QString spliName = configIni->value(section + "spliname_" + QString::number(i, 10)).toString();
        if(spliName == "")
        {
            continue;
        }else{
            QMap<QString, QVariant> paraMap;
            paraMap["spliName"] = spliName;
            paraMap["path"] = configIni->value(section + "path_" + QString::number(i, 10)).toString();
            paraMap["keywords_bool"] = configIni->value(section + "keywords_bool_" + QString::number(i, 10)).toBool();
            paraMap["keywords"] = configIni->value(section + "keywords_" + QString::number(i, 10)).toString();
            paraMap["latest"] = configIni->value(section + "latest_" + QString::number(i, 10)).toInt();
            paraMap["latest_b"] = configIni->value(section + "latest_b_" + QString::number(i, 10)).toInt();
            paraMap["latest_e"] = configIni->value(section + "latest_e_" + QString::number(i, 10)).toInt();
            paraMap["type"] = configIni->value(section + "type_" + QString::number(i, 10)).toInt();
            paraMap["rename"] = configIni->value(section + "rename_" + QString::number(i, 10)).toBool();
            paraMap["fixwords_bool"] = configIni->value(section + "fixwords_bool_" + QString::number(i, 10)).toBool();
            spliParaList.append(paraMap);
        }
    }
    delete configIni;
    qDebug() << "�����б��ȣ�" << spliParaList.length();
    qDebug() << "�����б�" << spliParaList;

    for(int i=0;i<spliParaList.length();i++)
    {
        QMap<QString, QVariant> paraMap;
        paraMap = spliParaList.at(i).toMap();

        QMap<QString, QVariant> abpath_rename_spliname;
        abpath_rename_spliname["rename"] = paraMap["rename"].toBool();
        abpath_rename_spliname["fixwords_bool"] = paraMap["fixwords_bool"].toBool();
        abpath_rename_spliname["spliname"] = paraMap["spliName"].toString();
        // Ϊ��ɸѡ������Ҫ���ļ����ļ��и������Ƚ�����ƥ���ԴĿ���ҳ������ٰ�������ȡ��
        QStringList sourceData_path_rename_spliname_List;
        QStringList sourceData_path_rename_spliname_List_Full;

        QDir dir(paraMap["path"].toString());
        qDebug() << "�����Դ·����" << paraMap["path"].toString();

        if(dir.exists())
        {
            qDebug() << "·�����ڡ�";

            switch (paraMap["type"].toInt()) {
            case 0:
                dir.setFilter(QDir::Files);
                break;
            case 1:
                dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
                break;
            default:
                dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
                break;
            }
            QFileInfoList infoList = dir.entryInfoList();
            if(infoList.count()){
                // �������ɵ��ļ����ļ��������б�ʼ
                qSort(infoList.begin(), infoList.end(), [](const QFileInfo &info1, const QFileInfo &info2)->bool
                    {
                        if (info1.lastModified().toMSecsSinceEpoch() > info2.lastModified().toMSecsSinceEpoch()){
                            return true;
                        }
                        return false;
                    }
                );
                // �����Ҫѡ���ؼ��֣��Ͱ�û�йؼ��ֵ����ݴ��б���ɾ��
                // ����ؼ����ǿգ��Ǿ��൱�ڲ���Ҫѡ���ؼ���

                if(paraMap["keywords_bool"].toBool() && (paraMap["keywords"].toString() != "") ){
                    for(int i=0;i<infoList.count();i++){
                        if(infoList[i].fileName().indexOf(paraMap["keywords"].toString()) != -1){
                            if(!paraMap["latest"].toInt()){
                                // latestΪ0��ʾ���µ�һ������ɵ���󳤶ȣ���������Ҫ�ҵĸ�����
                                sourceData_path_rename_spliname_List_Full.push_back(infoList[i].absoluteFilePath());
                            }else{
                                // latestΪ1��ʾ��ɵ�һ�������µ���󳤶ȣ���������Ҫ�ҵĸ�����
                                sourceData_path_rename_spliname_List_Full.push_front(infoList[i].absoluteFilePath());
                            }
                        }
                    }
                }else{
                    for(int i=0;i<infoList.count();i++){
                        if(!paraMap["latest"].toInt()){
                            // latestΪ0��ʾ���µ�һ������ɵ���󳤶ȣ���������Ҫ�ҵĸ�����
                            for(int i=0;i<infoList.count();i++){
                                sourceData_path_rename_spliname_List_Full.push_back(infoList[i].absoluteFilePath());
                            }
                        }else{
                            // latestΪ1��ʾ��ɵ�һ�������µ���󳤶ȣ���������Ҫ�ҵĸ�����
                            for(int i=0;i<infoList.count();i++){
                                sourceData_path_rename_spliname_List_Full.push_front(infoList[i].absoluteFilePath());
                            }
                        }
                    }
                }
            }else{
                qDebug() << "û���ҵ�Դ�ļ����ļ���·����Ҳ����Դ·�������ڡ�";
                continue;
            }
        }else{
            qDebug() << "Դ·��������";
        }
        qDebug() << "�ҵĵ��ļ��б�" << sourceData_path_rename_spliname_List_Full;

        // ��������Ҫ�󣬽�ȡfull�е�һ���ּ������list��
        int latest_b = paraMap["latest_b"].toInt();
        if(latest_b <= sourceData_path_rename_spliname_List_Full.length()){
            int latest_e = ( paraMap["latest_e"].toInt() <= sourceData_path_rename_spliname_List_Full.length() )
                         ? paraMap["latest_e"].toInt()
                         : sourceData_path_rename_spliname_List_Full.length();
            if( latest_e < latest_b)
                latest_e = latest_b;
            for(int i=( latest_b - 1 ); i<latest_e; i++){
                sourceData_path_rename_spliname_List.append(sourceData_path_rename_spliname_List_Full.at(i));
            }
        }else{
            qDebug() << "Ҫ��ĵ�һ���������������Χ��";
            continue;
        }

        abpath_rename_spliname["abpathlist"] = sourceData_path_rename_spliname_List;
        sourceData_path_rename_spliname_VList.append(abpath_rename_spliname);
    }

    return sourceData_path_rename_spliname_VList;
}

void MainWindow::on_pushButton_clicked()
{
    aditem = new AddItem;
    connect(aditem, &QDialog::accepted, [this](){
        QSettings *configIni = new QSettings("config.ini", QSettings::IniFormat);
        configIni->setIniCodec("UTF-8");
        if(aditem->findChild<QLineEdit *>("lineEdit")->text().simplified() != ""){
            /*
             * �ж������ļ����Ƿ������ģ�壬����оͲ�����
             * δ��ɸù���
            */
            QString itemNum = QString::number(configIni->childGroups().length(), 10);
            QString itemName = QString("/item%1/name").arg(itemNum);
            configIni->setValue(itemName, aditem->findChild<QLineEdit *>("lineEdit")->text());
            QString itemInit = QString("/item%1/init").arg(itemNum);
            configIni->setValue(itemInit, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            QString itemInnerCount = QString("/item%1/count").arg(itemNum);
            configIni->setValue(itemInnerCount, "0");

            QString itemFixPattern = QString("/item%1/fixPattern").arg(itemNum);
            configIni->setValue(itemFixPattern, QString("1"));
            QString itemfixLine1 = QString("/item%1/fixLine1").arg(itemNum);
            configIni->setValue(itemfixLine1, QString(""));
            QString itemfixLine2 = QString("/item%1/fixLine2").arg(itemNum);
            configIni->setValue(itemfixLine2, QString(""));
            QString itemLine2AutoPlus = QString("/item%1/fixLine2AutoPlus").arg(itemNum);
            configIni->setValue(itemLine2AutoPlus, QString("false"));
            QString itemfixLine3 = QString("/item%1/fixLine3").arg(itemNum);
            configIni->setValue(itemfixLine3, QString(""));

            QString compressed_path = QString("/item%1/compressed_path").arg(itemNum);
            configIni->setValue(compressed_path, configIni->value("route").toString());
            QString compressed_name = QString("/item%1/compressed_name").arg(itemNum);
            configIni->setValue(compressed_name, QString(""));
            QString maxsize_bool = QString("/item%1/maxsize_bool").arg(itemNum);
            configIni->setValue(maxsize_bool, QString("false"));
            QString maxsize = QString("/item%1/maxsize").arg(itemNum);
            configIni->setValue(maxsize, QString("1000"));

            configIni->sync();
            delete configIni;
            refreshItemList();
        }else{
            qDebug() << QStringLiteral("ģ�����Ʋ���Ϊ��");
        }
    });
    aditem->show();
}

void MainWindow::on_pushButton_6_clicked()
{
    ui->scrollArea->verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepSub);
}

void MainWindow::on_pushButton_7_clicked()
{
    ui->scrollArea->verticalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepAdd);
}

void MainWindow::on_pushButton_2_clicked()
{
    if(use_for_find_section_in_ini == "")
        return;
    // ����ֱ������dlg��������ģ����section�Ӷ���ʾbtn��
    Dialog* dlg = new Dialog;
    dlg->use_for_find_section_in_ini = use_for_find_section_in_ini;
    MainWindow* thatmw = this;
    connect(dlg, &QDialog::accepted, [thatmw, dlg](){
        QSettings *configIni = new QSettings("config.ini", QSettings::IniFormat);
        configIni->setIniCodec("UTF-8");
        for(int i=1;i<=configIni->value(dlg->use_for_find_section_in_ini + "count").toInt();i++){
            if( (configIni->value(dlg->use_for_find_section_in_ini + "spliname_" + QString::number(i, 10)).toString() == dlg->findChild<QLineEdit *>("lineEdit_4")->text().simplified()) && (dlg->findChild<QLineEdit *>("lineEdit_4")->text().simplified() != "") ){
                // ���׼���޸ĵ�Ŀ�����ظ�����������
                qDebug() << "��split����ʧ�ܣ����Դ�����split���ظ���";
                thatmw->findChild<QWidget *>("scrollAreaWidgetContents")->findChild<QPushButton *>(configIni->value(thatmw->use_for_find_section_in_ini + "name").toString())->click();
                delete configIni;
                return;
            }
        }
        // ��ʼֵΪ0����ӵĵ�һ����Ŀ��������Ϊ1������count��Ϊ1
        QString count = QString::number(configIni->value(thatmw->use_for_find_section_in_ini + "count").toInt() + 1, 10);
        configIni->setValue(thatmw->use_for_find_section_in_ini + "count", count);
        configIni->setValue(thatmw->use_for_find_section_in_ini + "path_" + count, dlg->findChild<QLabel *>("label")->text());
        configIni->setValue(thatmw->use_for_find_section_in_ini + "keywords_bool_" + count, dlg->findChild<QCheckBox *>("checkBox")->checkState());
        configIni->setValue(thatmw->use_for_find_section_in_ini + "keywords_" + count, dlg->findChild<QLineEdit *>("lineEdit")->text());
        configIni->setValue(thatmw->use_for_find_section_in_ini + "latest_" + count, dlg->findChild<QComboBox *>("comboBox_2")->currentIndex());
        configIni->setValue(thatmw->use_for_find_section_in_ini + "latest_b_" + count, dlg->findChild<QLineEdit *>("lineEdit_2")->text());
        configIni->setValue(thatmw->use_for_find_section_in_ini + "latest_e_" + count, dlg->findChild<QLineEdit *>("lineEdit_3")->text());
        configIni->setValue(thatmw->use_for_find_section_in_ini + "type_" + count, dlg->findChild<QComboBox *>("comboBox")->currentIndex());
        configIni->setValue(thatmw->use_for_find_section_in_ini + "rename_" + count, dlg->findChild<QCheckBox *>("checkBox_2")->checkState());
        configIni->setValue(thatmw->use_for_find_section_in_ini + "fixwords_bool_" + count, dlg->findChild<QCheckBox *>("checkBox_3")->checkState());
        configIni->setValue(thatmw->use_for_find_section_in_ini + "newname_" + count, dlg->findChild<QLineEdit *>("lineEdit_4")->text());
        configIni->setValue(thatmw->use_for_find_section_in_ini + "spliname_" + count, (dlg->findChild<QLineEdit *>("lineEdit_4")->text().simplified() == "")?QString("����%1").arg(count):dlg->findChild<QLineEdit *>("lineEdit_4")->text());
        configIni->sync();
        // �ҵ���ǰsection������ģ����
        thatmw->findChild<QWidget *>("scrollAreaWidgetContents")->findChild<QPushButton *>(configIni->value(thatmw->use_for_find_section_in_ini + "name").toString())->click();
        delete configIni;
    });
    connect(dlg, &Dialog::signalDelete, [dlg](){
        dlg->close();
    });
    dlg->show();
}

void MainWindow::on_lineEdit_textChanged()
{
    QSettings *configIni = new QSettings("config.ini", QSettings::IniFormat);
    configIni->setIniCodec("UTF-8");
    ui->label_2->setText(configIni->value(use_for_find_section_in_ini + "init").toString());
    delete configIni;
}

void MainWindow::on_pushButton_3_clicked()
{
    QSettings *configIni = new QSettings("config.ini", QSettings::IniFormat);
    configIni->setIniCodec("UTF-8");
    if(use_for_find_section_in_ini != "")
        configIni->remove(use_for_find_section_in_ini);
    configIni->sync();
    delete configIni;
    QList<QPushButton* > btns = ui->scrollAreaWidgetContents_2->findChildren<QPushButton* >();
    foreach (QPushButton* btn, btns)
    {
        delete btn;
    }
    ui->lineEdit->setText("�ռ�ģ������");
    ui->label_2->setText("�������ڣ�2019-07-11");
    use_for_find_section_in_ini = "";
    refreshItemList();
}

void MainWindow::on_pushButton_4_clicked()
{
    if(use_for_find_section_in_ini == "")
        return;
    /*
     * �ļ����ƹ���
     */
    QVariantList sourceData_path_rename_spliname_VList = selectData(use_for_find_section_in_ini);
    qDebug() << "���µ��ļ����������ƣ�";
    qDebug() << sourceData_path_rename_spliname_VList;

    // ��ȡ��Ŀ¼
    QSettings *configIni = new QSettings("config.ini", QSettings::IniFormat);
    configIni->setIniCodec("UTF-8");
    QString routePath = configIni->value("route").toString();
    delete configIni;

    QString desPath = QFileDialog::getExistingDirectory(this, "��ѡ��Ŀ���ļ���", routePath);
    if (desPath.isEmpty()){
        return;
    }
    else{
        qDebug() << "ͨ������ѡȡ�����Ŀ¼Ϊ��" << desPath;

        // д���Ŀ¼
        QSettings *configIni = new QSettings("config.ini", QSettings::IniFormat);
        configIni->setIniCodec("UTF-8");
        configIni->setValue("route", desPath);
        configIni->sync();
        delete configIni;
        qDebug() << "����һ�δ��ڴ򿪵ĸ�Ŀ¼����Ϊ��" << desPath;

        if(sourceData_path_rename_spliname_VList.count() != 0)
        {
            for(int i=0;i<sourceData_path_rename_spliname_VList.length();i++)
            {
                QStringList abpathList = sourceData_path_rename_spliname_VList.at(i).toMap()["abpathlist"].toStringList();
                QString prefix1 = "";
                QString suffix1 = "";
                QString prefix2 = "";
                QString suffix2 = "";
                QString prefix3 = "";
                QString suffix3 = "";
                if(sourceData_path_rename_spliname_VList.at(i).toMap()["fixwords_bool"].toBool()){
                    QSettings *configIni = new QSettings("config.ini", QSettings::IniFormat);
                    configIni->setIniCodec("UTF-8");
                    if(configIni->value(use_for_find_section_in_ini + "fixPattern").toInt() == 0){
                        prefix1 = configIni->value(use_for_find_section_in_ini + "fixLine1").toString();
                        prefix2 = configIni->value(use_for_find_section_in_ini + "fixLine2").toString();
                        prefix3 = configIni->value(use_for_find_section_in_ini + "fixLine3").toString();
                    }else{
                        suffix1 = configIni->value(use_for_find_section_in_ini + "fixLine1").toString();
                        suffix2 = configIni->value(use_for_find_section_in_ini + "fixLine2").toString();
                        suffix3 = configIni->value(use_for_find_section_in_ini + "fixLine3").toString();
                    }
                    if(configIni->value(use_for_find_section_in_ini + "fixLine2AutoPlus").toBool()){
                        configIni->setValue(use_for_find_section_in_ini + "fixLine2", (configIni->value(use_for_find_section_in_ini + "fixLine2").toInt()+1) );
                        configIni->sync();
                    }
                    delete configIni;
                }
                if(sourceData_path_rename_spliname_VList.at(i).toMap()["rename"].toBool()){
                    if(abpathList.length() == 1){
                        // ֱ�Ӹ��ƺ�������
                        // ���Ʋ���
                        wchar_t wct_dataPath[MAX_PATH] = {0};
                        abpathList.at(0).toWCharArray(wct_dataPath);
                        QString q_dataDesPath = desPath.replace("/", "\\");
                        wchar_t wct_dataDesPath[MAX_PATH] = {0};
                        q_dataDesPath.toWCharArray(wct_dataDesPath);
                        SHFILEOPSTRUCT fop;
                        // FO_COPY��FO_DELETE��FO_RENAME��FO_MOVE
                        fop.wFunc = FO_COPY;
                        fop.pFrom = wct_dataPath;
                        fop.pTo = wct_dataDesPath;
                        int fop_process = SHFileOperation(&fop);
                        if(fop_process == 0){
                            qDebug() << "���ļ��������ļ������Ƴɹ���";
                        }else{
                            qDebug() << "���ļ��������ļ�������ʧ�ܡ�" << fop_process;
                        }
                        // ����������
                        QString copiedDataPath_oldname;
                        QString copiedDataPath_renamed;
                        QFileInfo info1(abpathList.at(0));
                        if(info1.isFile()){
                            copiedDataPath_renamed = desPath.replace("/", "\\") + "\\" + prefix1 + prefix2 + prefix3 + sourceData_path_rename_spliname_VList.at(i).toMap()["spliname"].toString() + suffix1 + suffix2 + suffix3 + "." + info1.completeSuffix();
                        }else{
                            copiedDataPath_renamed = desPath.replace("/", "\\") + "\\" + prefix1 + prefix2 + prefix3 + sourceData_path_rename_spliname_VList.at(i).toMap()["spliname"].toString() + suffix1 + suffix2 + suffix3;
                        }
                        copiedDataPath_oldname = q_dataDesPath + "\\" + abpathList.at(0).split("/")[abpathList.at(0).split("/").length() - 1];
                        wchar_t wct_rmdataPath[MAX_PATH] = {0};
                        copiedDataPath_oldname.toWCharArray(wct_rmdataPath);
                        wchar_t wct_rmdataPath_new[MAX_PATH] = {0};
                        copiedDataPath_renamed.toWCharArray(wct_rmdataPath_new);
                        SHFILEOPSTRUCT rmfop;
                        // FO_COPY��FO_DELETE��FO_RENAME��FO_MOVE
                        rmfop.fFlags = FOF_RENAMEONCOLLISION;
                        rmfop.wFunc = FO_RENAME;
                        rmfop.pFrom = wct_rmdataPath;
                        rmfop.pTo = wct_rmdataPath_new;
                        int rmfop_process = SHFileOperation(&rmfop);
                        if(rmfop_process == 0){
                            qDebug() << "���ļ��������ļ����������ɹ���";
                            copiedDataPathList.append(copiedDataPath_renamed);
                        }else{
                            qDebug() << "���ļ��������ļ���������ʧ�ܡ�" << rmfop_process;
                        }
                    }else{
                        // ������Ŀ����Ϊ���ֵ��ļ��в��������Ƶ��ļ�����
                        QString newNameFolder;
                        newNameFolder = desPath + "/" + prefix1 + prefix2 + prefix3 + sourceData_path_rename_spliname_VList.at(i).toMap()["spliname"].toString() + suffix1 + suffix2 + suffix3;
                        QDir folderDir;
                        while(!folderDir.exists(newNameFolder)){
                            bool res = folderDir.mkpath(newNameFolder);
                            if(res)
                                break;
                            newNameFolder += QDateTime::currentDateTime().timeZoneAbbreviation();
                        }
                        for(int j=0;j<abpathList.length();j++){
                            wchar_t wct_dataPath[MAX_PATH] = {0};
                            abpathList.at(j).toWCharArray(wct_dataPath);
                            QString q_dataDesPath = newNameFolder.replace("/", "\\");
                            // �ֶ�ƴ�ӵ��ַ����޷�ֻʹ��ָ��Ŀ¼���ƣ�����ط�������Ǵ��ļ���������·��
                            q_dataDesPath += "\\" + abpathList.at(j).split("/")[abpathList.at(j).split("/").length() - 1];
                            wchar_t wct_dataDesPath[MAX_PATH] = {0};
                            q_dataDesPath.toWCharArray(wct_dataDesPath);
                            SHFILEOPSTRUCT fop;
                            // FO_COPY��FO_DELETE��FO_RENAME��FO_MOVE
                            fop.wFunc = FO_COPY;
                            fop.pFrom = wct_dataPath;
                            fop.pTo = wct_dataDesPath;
                            int fop_process = SHFileOperation(&fop);
                            if(fop_process == 0){
                                qDebug() << "�����ļ��У����Ƴɹ���";
                                copiedDataPathList.append(newNameFolder.replace("/", "\\"));
                            }else{
                                qDebug() << "�����ļ��У�����ʧ�ܡ�" << fop_process;
                            }
                        }
                    }
                }else{
                    for(int j=0;j<abpathList.length();j++){
                        wchar_t wct_dataPath[MAX_PATH] = {0};
                        abpathList.at(j).toWCharArray(wct_dataPath);
                        QString q_dataDesPath = desPath.replace("/", "\\");
                        // ������ļ���������·��
                        q_dataDesPath += "\\" + abpathList.at(j).split("/")[abpathList.at(j).split("/").length() - 1];
                        qDebug() << q_dataDesPath;
                        wchar_t wct_dataDesPath[MAX_PATH] = {0};
                        q_dataDesPath.toWCharArray(wct_dataDesPath);
                        SHFILEOPSTRUCT fop;
                        // FO_COPY��FO_DELETE��FO_RENAME��FO_MOVE
                        fop.wFunc = FO_COPY;
                        fop.pFrom = wct_dataPath;
                        fop.pTo = wct_dataDesPath;
                        int fop_process = SHFileOperation(&fop);
                        if(fop_process == 0){
                            qDebug() << "�����������ļ������Ƴɹ���";
                            copiedDataPathList.append(q_dataDesPath);
                        }else{
                            qDebug() << "�����������ļ�������ʧ�ܡ�" << fop_process;
                        }
                    }
                }
            }
        }else{
            qDebug() << "û���ļ������ƣ���Ϊû���ҵ�Դ�ļ���";
        }
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    if(use_for_find_section_in_ini == "")
        return;
    /*
     * ���Ʋ�ѹ������
     */
    ui->pushButton_4->click();
    qDebug() << "�����ļ�����ѹ����" << copiedDataPathList;
    if(copiedDataPathList.isEmpty())
        return;
    QSettings *configIni = new QSettings("config.ini", QSettings::IniFormat);
    configIni->setIniCodec("UTF-8");
    QString compressor = configIni->value("compressor").toString();
    QString compressed_path = configIni->value(use_for_find_section_in_ini + "compressed_path").toString();
    QString compressed_name = configIni->value(use_for_find_section_in_ini + "compressed_name").toString();
    bool maxsize_bool = configIni->value(use_for_find_section_in_ini + "maxsize_bool").toBool();
    int maxsize = configIni->value(use_for_find_section_in_ini + "maxsize").toInt();
    delete configIni;
    QProcess pro(0);
    // ѹ����������·����
    QString t1 = compressor;
    QStringList t2;
    t2.append("a");
    t2.append("-ep1");
    t2.append("-r");
    t2.append("-m1"); // ��췽ʽѹ��
    t2.append("-o+");
    // ���õ���ѹ�������ֵ��Ӧ�÷��������ļ���
    if(maxsize_bool)
        t2.append(QString("-V%1k").arg(maxsize)); // �趨����ѹ�������ֵ
    // ָ��������ƣ���Ҫ����·����
    if(compressed_name != ""){
        t2.append(compressed_path + "/" + compressed_name + ".rar");
    }else{
        t2.append(compressed_path + "/" + QString::number(QDateTime::currentMSecsSinceEpoch()) +".rar");
    }
    // ���ѹ���ļ�
    for(int i=0;i<copiedDataPathList.length();i++)
        t2.append(copiedDataPathList[i]);

    if(pro.execute(t1,t2) == 0)
    {
        qDebug() << "����ѹ�����ɹ���";
    }else{
        qDebug() << "����ѹ����ʧ�ܡ�";
    }

    copiedDataPathList.clear();
}

void MainWindow::on_pushButton_8_clicked()
{
    if(use_for_find_section_in_ini == "")
        return;
    /*
     * ����ǰ��׺���ݣ��������ѹ������
     */
    fixsetter = new FixDialog;
    QSettings *configIni = new QSettings("config.ini", QSettings::IniFormat);
    configIni->setIniCodec("UTF-8");
    fixsetter->findChild<QComboBox *>("comboBox")->setCurrentIndex(configIni->value(use_for_find_section_in_ini + "fixPattern").toInt());
    fixsetter->findChild<QLineEdit *>("lineEdit")->setText(configIni->value(use_for_find_section_in_ini + "fixLine1").toString());
    fixsetter->findChild<QLineEdit *>("lineEdit_2")->setText(configIni->value(use_for_find_section_in_ini + "fixLine2").toString());
    fixsetter->findChild<QCheckBox *>("checkBox")->setChecked(configIni->value(use_for_find_section_in_ini + "fixLine2AutoPlus").toBool());
    fixsetter->findChild<QLineEdit *>("lineEdit_3")->setText(configIni->value(use_for_find_section_in_ini + "fixLine3").toString());
    fixsetter->findChild<QLabel *>("label_3")->setText(configIni->value(use_for_find_section_in_ini + "compressed_path").toString());
    fixsetter->findChild<QLineEdit *>("lineEdit_4")->setText(configIni->value(use_for_find_section_in_ini + "compressed_name").toString());
    fixsetter->findChild<QCheckBox *>("checkBox_2")->setChecked(configIni->value(use_for_find_section_in_ini + "maxsize_bool").toBool());
    fixsetter->findChild<QLineEdit *>("lineEdit_5")->setText(configIni->value(use_for_find_section_in_ini + "maxsize").toString());
    delete configIni;
    MainWindow* thatmw = this;
    QDialog* fixsetterdlg = fixsetter;
    thatmw->connect(fixsetter, &QDialog::accepted, [thatmw, fixsetterdlg](){
        QSettings *configIni = new QSettings("config.ini", QSettings::IniFormat);
        configIni->setIniCodec("UTF-8");
        configIni->setValue(thatmw->use_for_find_section_in_ini + "fixPattern", fixsetterdlg->findChild<QComboBox *>("comboBox")->currentIndex());
        configIni->setValue(thatmw->use_for_find_section_in_ini + "fixLine1", fixsetterdlg->findChild<QLineEdit *>("lineEdit")->text());
        configIni->setValue(thatmw->use_for_find_section_in_ini + "fixLine2", fixsetterdlg->findChild<QLineEdit *>("lineEdit_2")->text());
        configIni->setValue(thatmw->use_for_find_section_in_ini + "fixLine2AutoPlus", fixsetterdlg->findChild<QCheckBox *>("checkBox")->isChecked());
        configIni->setValue(thatmw->use_for_find_section_in_ini + "fixLine3", fixsetterdlg->findChild<QLineEdit *>("lineEdit_3")->text());
        configIni->setValue(thatmw->use_for_find_section_in_ini + "compressed_path", fixsetterdlg->findChild<QLabel *>("label_3")->text());
        configIni->setValue(thatmw->use_for_find_section_in_ini + "compressed_name", fixsetterdlg->findChild<QLineEdit *>("lineEdit_4")->text());
        configIni->setValue(thatmw->use_for_find_section_in_ini + "maxsize_bool", fixsetterdlg->findChild<QCheckBox *>("checkBox_2")->isChecked());
        configIni->setValue(thatmw->use_for_find_section_in_ini + "maxsize", fixsetterdlg->findChild<QLineEdit *>("lineEdit_5")->text());
        configIni->sync();
        delete configIni;
    });
    fixsetter->show();
}
