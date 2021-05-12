#include "weapmatcalcform.h"
#include "ui_weapmatcalcform.h"
#include "global_defs.h"

#include "OdsInterface"
#include "QMessageBox"
#include "CatalogManager"
#include <OdsTemplateManager>
#include <QFileDialog>

WeapMatCalcForm::WeapMatCalcForm(int transitId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WeapMatCalcForm),
    currentTrId(transitId)
{
    ui->setupUi(this);
    init();
}

WeapMatCalcForm::~WeapMatCalcForm()
{
    delete ui;
}

void WeapMatCalcForm::init()
{
    model = new QStandardItemModel(this);
    ui->tv->setModel(model);

    initTv();
    initOther();

    connect(ui->pbWeapons, SIGNAL(released()), this, SLOT(loadWeap()));
    connect(ui->pbWeapReserve, SIGNAL(released()), this, SLOT(loadReserve()));
    connect(ui->pbCommRes, SIGNAL(released()), this, SLOT(loadComm()));
    connect(ui->pbMedProperty, SIGNAL(released()), this, SLOT(loadMed()));
    connect(ui->pbFireProperty, SIGNAL(released()), this, SLOT(loadFireproof()));
    connect(ui->pbRHBProtection, SIGNAL(released()), this, SLOT(loadRhb()));

    connect(ui->pbCalc, SIGNAL(released()), this, SLOT(calc()));
    connect(ui->pbSave, SIGNAL(released()), this, SLOT(save()));
    connect(ui->pbExit, SIGNAL(released()), this, SLOT(close()));
    connect(ui->pbPrint, SIGNAL(released()), this, SLOT(print()));
    connect(ui->pbAllPrint, SIGNAL(released()), this, SLOT(allPrint()));
}

void WeapMatCalcForm::initTv()
{
    model->clear();

    model->setHorizontalHeaderItem(0,new QStandardItem(QString("№")));
    model->setHorizontalHeaderItem(1,new QStandardItem(QString("Наименование")));
    model->setHorizontalHeaderItem(2,new QStandardItem(QString("Количество")));

    ui->tv->resizeColumnsToContents();
    ui->tv->verticalHeader()->setVisible(false);
#if QT_VERSION >= 0x050000
    ui->tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tv->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#else
    ui->tv->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tv->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#endif
//    ui->tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tv->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tv->setSelectionMode(QAbstractItemView::SingleSelection);
}

void WeapMatCalcForm::initOther()
{
    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();

    if (currentTrId == 0){
        QMessageBox::warning(0,"Ошибка", "Не найдена перевозка");
        return;
    }

    ODS::IObject trIobj = ioMan.getIObjectById(currentTrId);
    kp = trIobj.getStringAttr("Категория");

    int otherRailCount = 0;
    if (kp == "КП-1")
        otherRailCount = 9;
    else
        otherRailCount = 5;

    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"Перевозка\" ="+QString::number(currentTrId);
    ODS::IObjectCursor cur =  ioMan.getIObjects("Определение сил и средств.Вагоны определение", filter);

    if (cur.size() != 0){
        for (ODS::IObjectCursor::iterator it = cur.begin(); it != cur.end(); ++it){
            ODS::IObject iobj = *it;
            ui->leRailCount->setText(QString::number(iobj.getIntAttr("Количество вагонов")));
            ui->leRailSiCount->setText(QString::number(iobj.getIntAttr("Количество вагонов")-otherRailCount));
        }
    } else {
        QMessageBox::warning(0,"Ошибка", "Не найдены расчеты по вагонам");
    }

    postsMap.insert("Начальник эшелона", 0);
    postsMap.insert("1-й ЗНЭШ", 0);
    postsMap.insert("2-й ЗНЭШ", 0);
    postsMap.insert("Сопровождающий", 0);
    postsMap.insert("Начальник караула", 0);
    postsMap.insert("Помощник нач. караула", 0);
    postsMap.insert("Разводящий", 0);
    postsMap.insert("Расчет радиостанции", 0);
    postsMap.insert("Караульные", 0);
    postsMap.insert("Механик спец вагона", 0);

    int peopleCount = 0;
    cur =  ioMan.getIObjects("Определение сил и средств.Команда сопровождения определение", filter);
    for (ODS::IObjectCursor::iterator it = cur.begin(); it != cur.end(); ++it){
        ODS::IObject iobj = *it;
        int count = iobj.getIntAttr("Количество");
        postsMap[iobj.getLink("Должность").getStringAttr("Наименование")] = count;
        peopleCount += count;
    }

    ui->lePeopleCount->setText(QString::number(peopleCount));
}

void WeapMatCalcForm::calcWeap()
{
    IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
    IRS::Catalog::ptr catalog = catalMgr.getCatalogByName("Нормы укомплектования вооружением команды сопровождения");
    int i = 1;
    if (catalog) {
        IRS::CatalogEntry::ptrList divs_list = catalog->entries();

        for (IRS::CatalogEntry::ptrList::iterator it = divs_list.begin(); it != divs_list.end(); ++it){
            IRS::CatalogEntry::ptr div = (*it);

            QString weapName = div->stringValue("Наименование вооружения");
            int value = 0;

            for (QMap<QString, int>::iterator itMap = postsMap.begin(); itMap != postsMap.end(); ++itMap){

                if (itMap.key() == "Начальник эшелона"){
                    value += div->value("НЭШ").toInt() * itMap.value();
                } else if (itMap.key() == "1-й ЗНЭШ"){
                    value += div->value("1ЗНЭШ").toInt() * itMap.value();
                } else if (itMap.key() == "2-й ЗНЭШ"){
                    value += div->value("2ЗНЭШ").toInt() * itMap.value();
                } else if (itMap.key() == "Сопровождающий"){
                    value += div->value("Сопровождающий").toInt() * itMap.value();
                } else if (itMap.key() == "Начальник караула"){
                    value += div->value("НК").toInt() * itMap.value();
                } else if (itMap.key() == "Помощник нач. караула"){
                    value += div->value("Помощник начальника караула").toInt() * itMap.value();
                } else if (itMap.key() == "Разводящий"){
                    value += div->value("Разводящий").toInt() * itMap.value();
                } else if (itMap.key() == "Расчет радиостанции"){
                    value += div->value("Расчет радиостанции").toInt() * itMap.value();
                } else if (itMap.key() == "Караульные"){
                    value += div->value("Караульные").toInt() * itMap.value();
                } else if (itMap.key() == "Механик спец вагона"){
                    value += div->value("Механик спец вагона").toInt() * itMap.value();
                }
            }

            QList<QStandardItem*> row;
            QStandardItem* item = new QStandardItem;
            item->setText(QString::number(i));
            row<<item;
            item = new QStandardItem;
            item->setText(weapName);
            row<<item;
            item = new QStandardItem;
            item->setText(QString::number(value));
            row<<item;

            model->appendRow(row);
            i++;
        }
    } else {
        QMessageBox::warning(0,"Ошибка", "Не действительный классификатор\n'Нормы укомплектования вооружением команды сопровождения'");
        toDelPeopleIds.clear();
    }
}

void WeapMatCalcForm::calcReserve()
{
    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();
    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"Перевозка\" ="+QString::number(currentTrId);
    ODS::IObjectCursor cur =  ioMan.getIObjects("Определение сил и средств.Вооружение", filter);
    if (cur.size() == 0){
        QMessageBox::warning(0,"Ошибка", "Не найдены расчеты по вооружению");
        return;
    }

    int pistolCount = 0, aksCount = 0;
    for (ODS::IObjectCursor::iterator it = cur.begin(); it != cur.end(); ++it){
        ODS::IObject iobj = *it;
        if (iobj.getStringAttr("Наименование") == "Пистолет ПМ"){
            pistolCount = iobj.getIntAttr("Количество");
        } else if (iobj.getStringAttr("Наименование") == "Автомат АКС-74У (АК-74М)"){
            aksCount = iobj.getIntAttr("Количество");
        }
    }

    IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
    IRS::Catalog::ptr catalog = catalMgr.getCatalogByName("Нормы укомплектования резерва вооружения");
    int i = 1;
    if (catalog) {
        IRS::CatalogEntry::ptrList divs_list = catalog->entries();

        for (IRS::CatalogEntry::ptrList::iterator it = divs_list.begin(); it != divs_list.end(); ++it){
            IRS::CatalogEntry::ptr div = (*it);

            QString weapName = div->stringValue("Наименование вооружения");
            int value = div->value("Кол-во на ПМ").toInt() * pistolCount +
                           div->value("Кол-во на АКС").toInt() * aksCount +
                           div->value("Кол-во на ЖДЭ").toInt();

            QList<QStandardItem*> row;
            QStandardItem* item = new QStandardItem;
            item->setText(QString::number(i));
            row<<item;
            item = new QStandardItem;
            item->setText(weapName);
            row<<item;
            item = new QStandardItem;
            item->setText(QString::number(value));
            row<<item;

            model->appendRow(row);
            i++;
        }
    } else {
        QMessageBox::warning(0,"Ошибка", "Не действительный классификатор\n'Нормы укомплектования резерва вооружения'");
        toDelPeopleIds.clear();
    }
}

void WeapMatCalcForm::calcComm()
{
    IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
    IRS::Catalog::ptr catalog = catalMgr.getCatalogByName("Нормы укомплектования средствами связи и сигнализации");
    int i = 1;
    if (catalog) {
        IRS::CatalogEntry::ptrList divs_list = catalog->entries();

        for (IRS::CatalogEntry::ptrList::iterator it = divs_list.begin(); it != divs_list.end(); ++it){
            IRS::CatalogEntry::ptr div = (*it);

            QString name = div->stringValue("Наименование");
            int value = div->value("Кол-во на одного").toInt() * ui->lePeopleCount->text().toInt() +
                    div->value("Кол-во на каждый пост и помещ. НК").toInt() * postsMap["Начальник караула"] +
                           div->value("Кол-во на механика спец вагона").toInt() * postsMap["Механик спец вагона"] +
                           div->value("Кол-во на ЖДЭ").toInt();

            QList<QStandardItem*> row;
            QStandardItem* item = new QStandardItem;
            item->setText(QString::number(i));
            row<<item;
            item = new QStandardItem;
            item->setText(name);
            row<<item;
            item = new QStandardItem;
            item->setText(QString::number(value));
            row<<item;

            model->appendRow(row);
            i++;
        }
    } else {
        QMessageBox::warning(0,"Ошибка", "Не действительный классификатор\n'Нормы укомплектования средствами связи и сигнализации'");
        toDelPeopleIds.clear();
    }
}

void WeapMatCalcForm::calcMed()
{
    IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
    IRS::Catalog::ptr catalog = catalMgr.getCatalogByName("Нормы укомплектования медицинстким имуществом");
    int i = 1;
    if (catalog) {
        IRS::CatalogEntry::ptrList divs_list = catalog->entries();

        for (IRS::CatalogEntry::ptrList::iterator it = divs_list.begin(); it != divs_list.end(); ++it){
            IRS::CatalogEntry::ptr div = (*it);

            QString name = div->stringValue("Наименование");
            int value = div->value("Кол-во на одного человека").toInt() * ui->lePeopleCount->text().toInt() +
                           div->value("Кол-во на каждый вагон под СИ").toInt() * ui->leRailSiCount->text().toInt() +
                           div->value("Кол-во на ЖДЭ").toInt();

            QList<QStandardItem*> row;
            QStandardItem* item = new QStandardItem;
            item->setText(QString::number(i));
            row<<item;
            item = new QStandardItem;
            item->setText(name);
            row<<item;
            item = new QStandardItem;
            item->setText(QString::number(value));
            row<<item;

            model->appendRow(row);
            i++;
        }
    } else {
        QMessageBox::warning(0,"Ошибка", "Не действительный классификатор\n'Нормы укомплектования медицинстким имуществом'");
        toDelPeopleIds.clear();
    }
}

void WeapMatCalcForm::calcFire()
{
    IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
    IRS::Catalog::ptr catalog = catalMgr.getCatalogByName("Нормы укомплектования противопожарным имуществом");
    int i = 1;
    if (catalog) {
        IRS::CatalogEntry::ptrList divs_list = catalog->entries();

        for (IRS::CatalogEntry::ptrList::iterator it = divs_list.begin(); it != divs_list.end(); ++it){
            IRS::CatalogEntry::ptr div = (*it);

            QString name = div->stringValue("Наименование");
            int value = div->value("Кол-во на каждый вагон под СИ").toInt() * ui->leRailSiCount->text().toInt() +
                           div->value("Кол-во на ЖДЭ").toInt();

            QList<QStandardItem*> row;
            QStandardItem* item = new QStandardItem;
            item->setText(QString::number(i));
            row<<item;
            item = new QStandardItem;
            item->setText(name);
            row<<item;
            item = new QStandardItem;
            item->setText(QString::number(value));
            row<<item;

            model->appendRow(row);
            i++;
        }
    } else {
        QMessageBox::warning(0,"Ошибка", "Не действительный классификатор\n'Нормы укомплектования противопожарным имуществом'");
        toDelPeopleIds.clear();
    }
}

void WeapMatCalcForm::calcRadioact()
{
    IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
    IRS::Catalog::ptr catalog = catalMgr.getCatalogByName("Нормы укомплектования средствами рхб защиты");
    int i = 1;
    if (catalog) {
        IRS::CatalogEntry::ptrList divs_list = catalog->entries();

        for (IRS::CatalogEntry::ptrList::iterator it = divs_list.begin(); it != divs_list.end(); ++it){
            IRS::CatalogEntry::ptr div = (*it);

            QString name = div->stringValue("Наименование");
            int value = div->value("Кол-во на одного человека").toInt() * ui->lePeopleCount->text().toInt() +
                           div->value("Кол-во на ЖДЭ").toInt();

            QList<QStandardItem*> row;
            QStandardItem* item = new QStandardItem;
            item->setText(QString::number(i));
            row<<item;
            item = new QStandardItem;
            item->setText(name);
            row<<item;
            item = new QStandardItem;
            item->setText(QString::number(value));
            row<<item;

            model->appendRow(row);
            i++;
        }
    } else {
        QMessageBox::warning(0,"Ошибка", "Не действительный классификатор\n'Нормы укомплектования средствами рхб защиты'");
        toDelPeopleIds.clear();
    }
}

void WeapMatCalcForm::load()
{
    initTv();

    QString tableName = "";
    if (form == Weapons){
        tableName = "Определение сил и средств.Вооружение";
    } else if (form == Reserve){
        tableName = "Определение сил и средств.Резерв вооружения";
    } else if (form == Comm){
        tableName = "Определение сил и средств.Средства связи и сигнализации";
    } else if (form == Med){
        tableName = "Определение сил и средств.Мед имущество";
    } else if (form == Firefighter){
        tableName = "Определение сил и средств.Пожарное имущество";
    } else if (form == Radioactive){
        tableName = "Определение сил и средств.Средства рхб защиты";
    }

    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();
    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"Перевозка\" ="+QString::number(currentTrId);
    filter.orderCondition = "\"Порядковый номер\" ASC";
    ODS::IObjectCursor cur =  ioMan.getIObjects(tableName, filter);

    for (ODS::IObjectCursor::iterator it = cur.begin(); it != cur.end(); ++it){
        ODS::IObject iobj = *it;

        QList<QStandardItem*> row;
        QStandardItem* item = new QStandardItem;
        item->setText(QString::number(iobj.getIntAttr("Порядковый номер")));
        item->setData(iobj.id());
        row<<item;
        item = new QStandardItem;
        item->setText(iobj.getStringAttr("Наименование"));
        row<<item;
        item = new QStandardItem;
        item->setText(QString::number(iobj.getIntAttr("Количество")));
        row<<item;

        model->appendRow(row);
    }
}

void WeapMatCalcForm::calc()
{
    toDelPeopleIds.clear();
    int rowCount = model->rowCount();
    for (int j = 0; j != rowCount; j++){
        toDelPeopleIds.append(model->item(j,0)->data().toInt());
    }

    initTv();

    if (ui->leRailCount->text() == "" || ui->leRailSiCount->text() == "" || ui->lePeopleCount->text() == ""){
        QMessageBox::warning(0,"Ошибка", "Нет данных в полях");
        return;
    }

    if (form == Weapons){
        calcWeap();
    } else if (form == Reserve){
        calcReserve();
    } else if (form == Comm){
        calcComm();
    } else if (form == Med){
        calcMed();
    } else if (form == Firefighter){
        calcFire();
    } else if (form == Radioactive){
        calcRadioact();
    } else {
        QMessageBox::warning(0,"Внимание", "Сначала выберите форму");
    }
}

void WeapMatCalcForm::save()
{    
    ODS::OdsInterface::self()->dbManager().transaction();
    ODS::OdsInterface::self()->syncManager().syncTransaction();

    QString tableName = "";
    if (form == Weapons){
        tableName = "Определение сил и средств.Вооружение";
    } else if (form == Reserve){
        tableName = "Определение сил и средств.Резерв вооружения";
    } else if (form == Comm){
        tableName = "Определение сил и средств.Средства связи и сигнализации";
    } else if (form == Med){
        tableName = "Определение сил и средств.Мед имущество";
    } else if (form == Firefighter){
        tableName = "Определение сил и средств.Пожарное имущество";
    } else if (form == Radioactive){
        tableName = "Определение сил и средств.Средства рхб защиты";
    }

    for (int j = 0; j != toDelPeopleIds.size(); j++){
        ODS::OdsInterface::self()->iobjectManager().deleteIObjectById(toDelPeopleIds.at(j));
    }
    toDelPeopleIds.clear();

    for (int i = 0; i< model->rowCount(); i++){
        QStandardItem* item = model->item(i, 0);

        ODS::IObject iobj;
        int id = item->data().toInt();

        if (id > 0)
            iobj = ODS::OdsInterface::self()->iobjectManager().getIObjectById(id);
        else
            iobj = ODS::OdsInterface::self()->iobjectManager().createIObject(tableName);

        iobj.setAttr("Перевозка", currentTrId);
        iobj.setAttr("Наименование", model->item(i,1)->text());
        iobj.setAttr("Количество", model->item(i,2)->text().toInt());
        iobj.setAttr("Порядковый номер", model->item(i,0)->text().toInt());

        if (!iobj.update(ODS::IObject::NotRecursiveUpdate)){
            ODS::OdsInterface::self()->dbManager().rollback();
            QMessageBox::critical(0, "Ошибка", "Неудачная попытка созранения записи");
            qDebug()<<"Ошибка при сохраниении данных в бд";
            return;
        } else {
            if (!setIObjRights(iobj)) {
                QMessageBox::critical(0, "Ошибка", "Неудачная попытка выдачи прав");
                qDebug()<<"Ошибка при сохраниении данных в бд";
                return;
            }
        }
    }

    ODS::OdsInterface::self()->dbManager().commit();
    msgToStatus("Расчет сохранен");
}

void WeapMatCalcForm::print()
{
    OdsTemplateManager tmpMan = OdsTemplateManager(ODS::OdsInterface::self());
    bool ok = false;
    QString templName = "", tagName = "", tableName = "";
    if (form == Weapons){
        templName = "Вооружение КС";
        tableName = "Определение сил и средств.Вооружение";
        tagName = "weapons";
    } else if (form == Reserve){
        templName = "Резерв вооружения КС";
        tableName = "Определение сил и средств.Резерв вооружения";
        tagName = "reserve";
    } else if (form == Comm){
        templName = "Средства связи и сигнализации";
        tableName = "Определение сил и средств.Средства связи и сигнализации";
        tagName = "comm";
    } else if (form == Med){
        templName = "Медицинское имущество";
        tableName = "Определение сил и средств.Мед имущество";
        tagName = "med";
    } else if (form == Firefighter){
        templName = "Противопожарное имущество";
        tableName = "Определение сил и средств.Пожарное имущество";
        tagName = "fire";
    } else if (form == Radioactive){
        templName = "Средства РХБ защиты";
        tableName = "Определение сил и средств.Средства рхб защиты";
        tagName = "rhb";
    } else {
        QMessageBox::warning(0,"Внимание", "Сначала выберите форму");
        return;
    }

    OdsTemplate templ = tmpMan.getTemplate(templName, &ok);

    if (!ok){
        QString str = "Не выполнена загрузка шаблона: "+templName;
        QMessageBox::critical(0,"Ошибка", str);
        emit msgToStatus(str);
        return;
    }

    if (!templ.isValid()){
        QString str = "Недействительный шаблон: "+templName;
        QMessageBox::critical(0,"Ошибка", str);
        emit msgToStatus(str);
        return;
    }

    WordTable wordTable(0, 3);

    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();
    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"Перевозка\" ="+QString::number(currentTrId);
    filter.orderCondition = "\"Порядковый номер\" ASC";
    ODS::IObjectCursor cur =  ioMan.getIObjects(tableName, filter);

    if (cur.size() == 0){
        QMessageBox::warning(0,"Внимание", "Нет сохраненных данных");
        emit(msgToStatus("Нет данных для сохранения"));
    }

    for (ODS::IObjectCursor::iterator It = cur.begin(); It != cur.end(); ++It){
        ODS::IObject iobj = (*It);

        wordTable.appendRow();

        wordTable.addText(wordTable.TRCount(), 1, QString::number(iobj.getIntAttr("Порядковый номер")));
        wordTable.addText(wordTable.TRCount(), 2, iobj.getStringAttr("Наименование"));
        wordTable.addText(wordTable.TRCount(), 3, QString::number(iobj.getIntAttr("Количество")));
    }

    templ.insertTableData(tagName, &wordTable);

    QString fileName = QFileDialog::getSaveFileName(0, "Сохранить в файл", templName, "Docx Files (*.docx)");

    if (fileName.isEmpty()){
        emit msgToStatus("Документ не сохранен");
        return;
    }

    if (!templ.saveTemplate(fileName, WordProcessingDocument::DocX)){
        QMessageBox::critical(0,"Ошибка", "Ошибка при сохранении документа!");
        emit msgToStatus("Ошибка при сохранении документа!");
        qDebug()<<"Ошибка при сохранении в менеджере шаблонов!";
        return;
    } else {
        emit msgToStatus("Документ сохранен");
    }
}

void WeapMatCalcForm::allPrint()
{
    OdsTemplateManager tmpMan = OdsTemplateManager(ODS::OdsInterface::self());
    bool ok = false;
    QList<QString> tables; QList<QString> tags;
    tables<<"Определение сил и средств.Вооружение"<<"Определение сил и средств.Резерв вооружения"
            <<"Определение сил и средств.Средства связи и сигнализации"
            <<"Определение сил и средств.Мед имущество"<<"Определение сил и средств.Пожарное имущество"
            <<"Определение сил и средств.Средства рхб защиты";
    tags<<"weapons"<<"reserve"<<"comm"<<"med"<<"fire"<<"rhb";

    OdsTemplate templ = tmpMan.getTemplate("Перечень вооруж_средств_имущества", &ok);

    if (!ok){
        QString str = "Не выполнена загрузка шаблона: Перечень вооруж_средств_имущества";
        QMessageBox::critical(0,"Ошибка", str);
        emit msgToStatus(str);
        return;
    }

    if (!templ.isValid()){
        QString str = "Недействительный шаблон: Перечень вооруж_средств_имущества";
        QMessageBox::critical(0,"Ошибка", str);
        emit msgToStatus(str);
        return;
    }

    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();
    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"Перевозка\" ="+QString::number(currentTrId);
    filter.orderCondition = "\"Порядковый номер\" ASC";

    for (int i = 0; i != tables.size(); ++i){

        WordTable wordTable(0, 3);
        ODS::IObjectCursor cur =  ioMan.getIObjects(tables[i], filter);

        if (cur.size() == 0){
            QMessageBox::warning(0,"Внимание", "Нет сохраненных данных в"+tables[i]);
            emit(msgToStatus("Нет данных для сохранения"));
        }

        for (ODS::IObjectCursor::iterator It = cur.begin(); It != cur.end(); ++It){
            ODS::IObject iobj = (*It);

            wordTable.appendRow();

            wordTable.addText(wordTable.TRCount(), 1, QString::number(iobj.getIntAttr("Порядковый номер")));
            wordTable.addText(wordTable.TRCount(), 2, iobj.getStringAttr("Наименование"));
            wordTable.addText(wordTable.TRCount(), 3, QString::number(iobj.getIntAttr("Количество")));
        }

        templ.insertTableData(tags[i], &wordTable);
    }

    QString fileName = QFileDialog::getSaveFileName(0, "Сохранить в файл", "Перечень имущества ЖДЭ", "Docx Files (*.docx)");

    if (fileName.isEmpty()){
        emit msgToStatus("Документ не сохранен");
        return;
    }

    if (!templ.saveTemplate(fileName, WordProcessingDocument::DocX)){
        QMessageBox::critical(0,"Ошибка", "Ошибка при сохранении документа!");
        emit msgToStatus("Ошибка при сохранении документа!");
        return;
    } else {
        emit msgToStatus("Документ сохранен");
    }
}

void WeapMatCalcForm::close()
{
    this->parentWidget()->close();
}
