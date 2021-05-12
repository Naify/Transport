#include "peoplecalcform.h"
#include "ui_peoplecalcform.h"
#include "global_defs.h"

#include "QComboBox"
#include "OdsInterface"
#include "QMessageBox"
#include "CatalogManager"
#include "choosepeopledialog.h"
#include "showescortdialog.h"
#include "OdsTemplateManager"
#include "QFileDialog"

PeopleCalcForm::PeopleCalcForm(int transitId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PeopleCalcForm),
    currentTrId(transitId),
    peopleCount(0)
{
    ui->setupUi(this);
    init();
}

PeopleCalcForm::~PeopleCalcForm()
{
    delete ui;
}

void PeopleCalcForm::init()
{
    model = new QStandardItemModel(this);
    ui->tv->setModel(model);

    initTv();
    initOther();

    load();

    connect(ui->tv, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(tvDblClick(QModelIndex)));
    connect(ui->pbCalc, SIGNAL(released()), this, SLOT(calc()));
    connect(ui->pbSave, SIGNAL(released()), this, SLOT(save()));
    connect(ui->pbExit, SIGNAL(released()), this, SLOT(close()));
    connect(ui->pbEscort, SIGNAL(released()), this, SLOT(escort()));
    connect(ui->pbPrintRailMaster, SIGNAL(released()), this, SLOT(printRailMater()));
}

void PeopleCalcForm::initTv()
{
    model->clear();

    model->setHorizontalHeaderItem(0,new QStandardItem(QString("№")));
    model->setHorizontalHeaderItem(1,new QStandardItem(QString("Должность")));
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
    ui->tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tv->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tv->setSelectionMode(QAbstractItemView::SingleSelection);
}

void PeopleCalcForm::initOther()
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
            ui->leAllRailCount->setText(QString::number(iobj.getIntAttr("Количество вагонов")));
            ui->leSiRailCount->setText(QString::number(iobj.getIntAttr("Количество вагонов")-otherRailCount));
        }
    } else {
        QMessageBox::warning(0,"Ошибка", "Не найдены расчеты по вагонам");
    }

    ui->leKP->setText(kp);
    ui->cbRailsTerm->addItem("Зима",0);
    ui->cbRailsTerm->addItem("Лето",1);

    ODS::IObject cargoIobj = ioMan.getIObjectById(trIobj.getIntAttr("Содержание распоряжения"));
    siId = cargoIobj.getIntAttr("Наименование груза");
}

void PeopleCalcForm::load()
{
    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();
    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"Перевозка\" ="+QString::number(currentTrId);
    filter.orderCondition = "\"Порядковый номер\" ASC";
    ODS::IObjectCursor cur =  ioMan.getIObjects("Определение сил и средств.Команда сопровождения определение", filter);

    int peopleCount = 0;

    for (ODS::IObjectCursor::iterator it = cur.begin(); it != cur.end(); ++it){
        ODS::IObject iobj = *it;

        QList<QStandardItem*> row;
        QStandardItem* item = new QStandardItem;
        item->setText(QString::number(iobj.getIntAttr("Порядковый номер")));
        item->setData(iobj.id());
        row<<item;
        item = new QStandardItem;
        item->setText(iobj.getLink("Должность").getStringAttr("Наименование"));
        item->setData(iobj.getIntAttr("Должность"));
        row<<item;
        item = new QStandardItem;
        item->setText(QString::number(iobj.getIntAttr("Количество")));
        row<<item;

        peopleCount+= iobj.getIntAttr("Количество");

        model->appendRow(row);
    }

    ui->lePeopleCount->setText(QString::number(peopleCount));
}

void PeopleCalcForm::calc()
{
    toDelPeopleIds.clear();
    int rowCount = model->rowCount();
    for (int j = 0; j != rowCount; j++){
        toDelPeopleIds.append(model->item(j,0)->data().toInt());
    }

    initTv();

    if (ui->leAllRailCount->text() == "" || ui->leSiRailCount->text() == ""){
        QMessageBox::warning(0,"Ошибка", "Нет данных в полях 'Общее кол-вовагонов в ЖДЭ'\n"
                             "или 'Количество вагонов под СИ'");
        toDelPeopleIds.clear();
        return;
    }

    getKpVals();
}

void PeopleCalcForm::getKpVals()
{
    peopleCount = 0;

    getPosVals("Начальник эшелона");
    getPosVals("1-й ЗНЭШ");
    getPosVals("2-й ЗНЭШ");
    getPosVals("Сопровождающий");
    getPosVals("Механик спец вагона");
    getPosVals("Начальник караула");
    getPosVals("Помощник нач. караула");
    getPosVals("Разводящий");
    getPosVals("Караульные");
    getPosVals("Расчет радиостанции");

    ui->lePeopleCount->setText(QString::number(peopleCount));
}

void PeopleCalcForm::getPosVals(const QString &pos)
{
    IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
    int posId = 0;

    IRS::Catalog::ptr catalog = catalMgr.getCatalogByName("Классификатор должностей");
    if (catalog) {
        QHash<QString, QString> filter;
        filter["Наименование"] = pos;
        IRS::CatalogEntry::ptrList entrylist = catalog->entries(filter);
        if (entrylist.isEmpty()){
            QMessageBox::warning(0,"Ошибка", "Нет должности " + pos + " в 'Классификатор должностей'");
            return;
        } else {
            posId = entrylist.at(0)->id();
        }
    } else {
        QMessageBox::warning(0,"Ошибка", "Не действительный классификатор 'Классификатор должностей'");
    }

    catalog = catalMgr.getCatalogByName("Нормы укомплектования команды сопровождения");
    if (catalog) {

        QHash<QString, QString> filter;
        int value = 0;

        filter["Должность"] = QString::number(posId);
        IRS::CatalogEntry::ptrList entrylist = catalog->entries(filter);
        int RailsCount = ui->leAllRailCount->text().toInt();

        if (entrylist.isEmpty()){
            QMessageBox::warning(0,"Ошибка", "Нет должности " + pos +" в 'Нормы укомплектования команды сопровождения'");
            return;
        } else {
            if (pos == "Сопровождающий"){
                if (kp == "КП-1" || kp == "КП-2"){
                    if (ui->cbRailsTerm->itemData(ui->cbRailsTerm->currentIndex()) == 1){
                        value = ui->leSiRailCount->text().toInt() * entrylist.at(0)->value("Теплый, на вагон").toInt();
                    } else {
                        value = ui->leSiRailCount->text().toInt() / 5 * entrylist.at(0)->value("Холодный, на 5 вагонов").toInt();
                    }
                }
            } else if (pos == "Механик спец вагона"){
                if (ui->cbRailsTerm->itemData(ui->cbRailsTerm->currentIndex()) == 1){
                    value = ( ui->leSiRailCount->text().toInt() * entrylist.at(0)->value("Теплый, на вагон").toInt() ) + 2;
                } else {
                    value = ( ( ui->leSiRailCount->text().toInt() / 5 )* entrylist.at(0)->value("Холодный, на 5 вагонов").toInt() ) + 2;
                }
            } else if (pos == "Караульные"){
                if (RailsCount <= 10){
                    value = entrylist.at(0)->value("До 10 вагонов").toInt();
                } else if (RailsCount > 10 && RailsCount < 16){
                    value = entrylist.at(0)->value("11-25 вагонов").toInt();
                } else if (RailsCount > 15 && RailsCount < 21){
                    value = entrylist.at(0)->value("16-20 вагонов").toInt();
                } else {
                    value = entrylist.at(0)->value("21-25 вагонов").toInt();
                }
            } else {
                value = entrylist.at(0)->value(kp).toInt();
            }
        }

        QList<QStandardItem*> row;
        QStandardItem* item = new QStandardItem;
        item->setText(QString::number(ui->tv->model()->rowCount()+1));
        row<<item;
        item = new QStandardItem;
        item->setText(pos);
        item->setData(entrylist.at(0)->linkedEntryId("Должность"));
        row<<item;
        item = new QStandardItem;
        item->setText(QString::number(value));
        row<<item;

        peopleCount +=value;
        model->appendRow(row);

    } else {
        QMessageBox::warning(0,"Ошибка", "Не действительный классификатор 'Нормы укомплектования команды сопровождения'");
    }
}

void PeopleCalcForm::save()
{
    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();
    ODS::OdsInterface::self()->dbManager().transaction();
    ODS::OdsInterface::self()->syncManager().syncTransaction();

    int rowCount = model->rowCount();

    for (int j = 0; j != toDelPeopleIds.size(); j++){
            ioMan.deleteIObjectById(toDelPeopleIds.at(j));
    }
    toDelPeopleIds.clear();

    for (int i = 0; i != rowCount; i++){

        ODS::IObject iobj = ioMan.createIObject("Определение сил и средств.Команда сопровождения определение");
        iobj.setAttr("Перевозка", currentTrId);
        iobj.setAttr("Должность", model->item(i,1)->data().toInt());
        iobj.setAttr("Количество", model->item(i,2)->text().toInt());
        iobj.setAttr("Порядковый номер", model->item(i,0)->text().toInt());

        if (!iobj.save(ODS::IObject::NotRecursiveSave)){
            ODS::OdsInterface::self()->dbManager().rollback();
            QMessageBox::critical(0, "Ошибка", "Неудачная попытка созранения записи");
            qDebug()<<"Ошибка при сохраниении данных в бд";
            return;
        } else {
            if (!setIObjRights(iobj)) {
                QMessageBox::critical(0, "Ошибка", "Неудачная попытка выдачи прав");
                qDebug()<<"Ошибка при сохраниении данных в бд";
                return;
            } else {
                model->item(i,0)->setData(iobj.id());
            }
        }
    }
    ODS::OdsInterface::self()->dbManager().commit();
    msgToStatus("Расчет сохранен");
}

void PeopleCalcForm::tvDblClick(QModelIndex index)
{
    QStandardItem* postItem = model->item(index.row(), 1);
    QStandardItem* countItem = model->item(index.row(), 2);
    int postId = postItem->data().toInt();
    int count = countItem->text().toInt();

    ChoosePeopleDialog* peopDialog = new ChoosePeopleDialog(postId, count, siId, this);
    peopDialog->setModal(true);
    connect(peopDialog, SIGNAL(sendIds(QMap<int,int>)), this, SLOT(savePersonIds(QMap<int,int>)));
    peopDialog->exec();
}

void PeopleCalcForm::savePersonIds(QMap<int,int> personPostIds)
{
    QMapIterator<int, int> i(personPostIds);
     while (i.hasNext()) {
         i.next();
         newPeople.insert(i.key(), i.value());
     }
}

void PeopleCalcForm::delFromEscort(int personId)
{
    newPeople.remove(personId);
}

void PeopleCalcForm::printRailMater()
{
    OdsTemplateManager tmpMan = OdsTemplateManager(ODS::OdsInterface::self());
    bool ok=false;
    OdsTemplate templ = tmpMan.getTemplate("Удостоверение НЭШ", &ok);

    if (!ok) {
        emit(msgToStatus("Не выполнена загрузка шаблона: Удостоверение НЭШ "));
        return;
    }

    if (!templ.isValid()) {
        emit(msgToStatus("Недействительный шаблон: Удостоверение НЭШ"));
        return;
    }

    QString querySQL = "SELECT \"Контроль перевозок\".\"Станции ПА и ПНВТ\".\"Номер ЖДЭ\" "
            "as \"Номер ЖДЭ\" FROM \"Определение сил и средств\".\"Перевозка\" "
            "JOIN \"Контроль перевозок\".\"Содержание таблица\" ON \"Контроль перевозок\"."
            "\"Содержание таблица\".\"io_id\" = \"Определение сил и средств\".\"Перевозка\"."
            "\"Содержание распоряжения\" JOIN \"Контроль перевозок\".\"Распоряжения\" ON "
            "\"Контроль перевозок\".\"Распоряжения\".\"io_id\" = \"Контроль перевозок\"."
            "\"Содержание таблица\".\"Распоряжение\" JOIN \"Контроль перевозок\"."
            "\"Маршрутное задание\" ON \"Контроль перевозок\".\"Маршрутное задание\"."
            "\"Распоряжение\" = \"Контроль перевозок\".\"Распоряжения\".\"io_id\" JOIN "
            "\"Контроль перевозок\".\"Станции ПА и ПНВТ\" ON \"Контроль перевозок\"."
            "\"Станции ПА и ПНВТ\".\"Марш задание\" = \"Контроль перевозок\"."
            "\"Маршрутное задание\".\"io_id\" WHERE \"Определение сил и средств\"."
            "\"Перевозка\".\"io_id\" = "+QString::number(currentTrId);

    ODS::IORecordCursor rec_cursor = ODS::OdsInterface::self()->iobjectManager().exec(querySQL);

    if (rec_cursor.size() == 0){
        QMessageBox::warning(0, "Внимание", "Не найдены данные о номерах эшелонов в дорожке/марш задании");
        emit msgToStatus("Документ не создан");
        return;
    }

    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();
    QString result;

    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"Перевозка\" ="+QString::number(currentTrId);
    ODS::IObjectCursor  cur =  ioMan.getIObjects("Определение сил и средств.Команда сопровождения", filter);
    if (!cur.size()){
        QMessageBox::warning(0, "Внимание", "Нет сохраненного личного состава команды сопровождения");
        emit msgToStatus("Документ не создан");
        return;
    }
    for (ODS::IObjectCursor::iterator it = cur.begin(); it != cur.end(); ++it){
        ODS::IObject personIObj = *it;

        QString post = personIObj.getLink("Должность").getStringAttr("Наименование");
        if (post == "1-й ЗНЭШ" || post == "2-й ЗНЭШ" || post == "Начальник эшелона" ||
                post == "Начальник автомобильной колонны" || post == "Зам начальника автомобильной колонны")  {
            QString fio = personIObj.getStringAttr("ФИО");

            result = post + " " + fio;

            templ.insertText("fioPos", result);

            for (ODS::IORecordCursor::iterator It = rec_cursor.begin(); It != rec_cursor.end(); ++It){
                ODS::Record iobj = (*It);

                templ.insertText("trainNumb", iobj.getStringAttr("Номер ЖДЭ"));

                QString docName("Удостоверение НЭШ ");
                docName += fio+" "+iobj.getStringAttr("Номер ЖДЭ");

                QString fileName = QFileDialog::getSaveFileName(0, "Сохранить в файл", docName,
                                                                "Docx Files (*.docx)");

                if (fileName.isEmpty()){
                    emit msgToStatus("Документ не сохранен");
                    return;
                }
                if (!templ.saveTemplate(fileName, WordProcessingDocument::DocX)){
                    emit msgToStatus("Ошибка при сохранении документа!");
                    return;
                } else {
                    emit msgToStatus("Документ сохранен");
                }
            }
        }
    }
}

void PeopleCalcForm::clearPeopleList()
{
    newPeople.clear();
}

void PeopleCalcForm::close()
{
    this->parentWidget()->close();
}

void PeopleCalcForm::escort()
{
    showEscortDialog* escDialog = new showEscortDialog(newPeople, currentTrId);
    escDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(escDialog, SIGNAL(delId(int)), this, SLOT(delFromEscort(int)));
    connect(escDialog, SIGNAL(clearNewPeople()), this, SLOT(clearPeopleList()));
    escDialog->exec();
}
