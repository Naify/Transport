#include "medadmissionmodel.h"
#include "OdsInterface"
#include <CatalogManager>
#include <QMessageBox>
#include "global_defs.h"
#include "AdmissionForms/AdmissionMainForm/choosepersondialog.h"

MedAdmissionModel::MedAdmissionModel(int vcId, QObject *parent) :
    QStandardItemModel(parent),
    iface(ODS::OdsInterface::self()),
    currentVcid(vcId),
    personId(0)
{
    init();
}

MedAdmissionModel::~MedAdmissionModel()
{
}

void MedAdmissionModel::init()
{
    load();
}

void MedAdmissionModel::load()
{
    removeRows(0, rowCount());

    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"Вч\" = " + QString::number(currentVcid);

    ODS::IObjectCursor cursor = ODS::OdsInterface::self()->iobjectManager().getIObjectCursor("Допуска КП.Допуска мед",filter);

    ODS::IObjectCursor::iterator it = cursor.begin();
    while (it != cursor.end())
    {
        ODS::IObject obj = *it;
        makeRow(obj);
        ++it;
    }
}

void MedAdmissionModel::makeRow(const ODS::IObject &obj)
{
    QList<QStandardItem*> list;

    QStandardItem* item = new QStandardItem;
    item->setData(Num, TypeRole);
    item->setText(QString::number(obj.getIntAttr("Порядковый номер")));
    item->setData(obj.id(), IdRole);
    item->setData(false, ChangedRole);
    item->setData(obj.getIntAttr("Человек"), PersonIdRole);
    list<<item;

    item = new QStandardItem;
    item->setData(Rank, TypeRole);
    ODS::IObject personObj = obj.getLink("Человек");
    IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
    IRS::Catalog::ptr catalog = catalMgr.getCatalogByName("Классификатор воинских званий");
    if (catalog){
        if (personObj.getIntAttr("Воинское звание") != 0){
            IRS::CatalogEntry::ptr entry = catalog->entryById(personObj.getIntAttr("Воинское звание"));
            item->setText(entry->stringValue("Наименование"));
        }
    } else {
        QMessageBox::critical(0,"Ошибка", "Недействительный 'Классификатор воинских званий'");
    }
    list<<item;

    item = new QStandardItem;
    item->setData(Post, TypeRole);
    catalog = catalMgr.getCatalogByName("Классификатор должностей");
    if (catalog){
        if (personObj.getIntAttr("Должность") != 0){
            IRS::CatalogEntry::ptr entry = catalog->entryById(personObj.getIntAttr("Должность"));
            item->setText(entry->stringValue("Наименование"));
        }
    } else {
        QMessageBox::critical(0,"Ошибка", "Недействительный 'Классификатор должностей'");
    }
    list<<item;

    item = new QStandardItem;
    item->setData(Fio, TypeRole);
    item->setText(personObj.getStringAttr("ФИО"));
    list<<item;

    item = new QStandardItem;
    item->setData(Date, TypeRole);
    QDateTime datetime;
    datetime.setTime_t(personObj.getDateTimeAttr("Дата назначения").getDateTime());
    item->setText(datetime.toString("dd.MM.yyyy"));
    list<<item;

    item = new QStandardItem;
    item->setData(Date2, TypeRole);
    datetime.setTime_t(obj.getDateTimeAttr("Дата прохождения ВВК").getDateTime());
    item->setText(datetime.toString("dd.MM.yyyy"));
    list<<item;

    item = new QStandardItem;
    item->setData(Term, TypeRole);
    item->setText(QString::number(obj.getIntAttr("Срок заключения ВВК")));
    list<<item;

    item = new QStandardItem;
    item->setData(Result, TypeRole);
    if (obj.getBoolAttr("Заключение")){
        item->setText("Годен");
    } else {
        item->setText("Не годен");
    }
    list<<item;

    item = new QStandardItem;
    item->setData(DateEnd, TypeRole);
    datetime.setTime_t(obj.getDateTimeAttr("Дата окончания").getDateTime());
    item->setText(datetime.toString("dd.MM.yyyy"));
    list<<item;

    appendRow(list);
}

void MedAdmissionModel::addEmptyRow()
{
    QList<QStandardItem*> list;

    choosePerson();

    QStandardItem* item = new QStandardItem;
    item->setData(Num, TypeRole);
    item->setData(-1, IdRole);
    item->setData(false, ChangedRole);
    item->setData(personId, PersonIdRole);
    list<<item;

    item = new QStandardItem;
    item->setData(Rank, TypeRole);
    ODS::IObject personObj = ODS::OdsInterface::self()->iobjectManager().getIObjectById(personId);
    IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
    IRS::Catalog::ptr catalog = catalMgr.getCatalogByName("Классификатор воинских званий");
    if (catalog){
        if (personObj.getIntAttr("Воинское звание") != 0){
            IRS::CatalogEntry::ptr entry = catalog->entryById(personObj.getIntAttr("Воинское звание"));
            item->setText(entry->stringValue("Наименование"));
        }
    } else {
        QMessageBox::critical(0,"Ошибка", "Недействительный 'Классификатор воинских званий'");
    }
    list<<item;

    item = new QStandardItem;
    item->setData(Post, TypeRole);
    catalog = catalMgr.getCatalogByName("Классификатор должностей");
    if (catalog){
        if (personObj.getIntAttr("Должность") != 0){
            IRS::CatalogEntry::ptr entry = catalog->entryById(personObj.getIntAttr("Должность"));
            item->setText(entry->stringValue("Наименование"));
        }
    } else {
        QMessageBox::critical(0,"Ошибка", "Недействительный 'Классификатор должностей'");
    }
    list<<item;

    item = new QStandardItem;
    item->setData(Fio, TypeRole);
    item->setText(personObj.getStringAttr("ФИО"));
    list<<item;

    item = new QStandardItem;
    item->setData(Date, TypeRole);
    QDateTime datetime;
    datetime.setTime_t(personObj.getDateTimeAttr("Дата назначения").getDateTime());
    item->setText(datetime.toString("dd.MM.yyyy"));
    list<<item;

    item = new QStandardItem;
    item->setData(Date2, TypeRole);
    list<<item;

    item = new QStandardItem;
    item->setData(Term, TypeRole);
    list<<item;

    item = new QStandardItem;
    item->setData(Result, TypeRole);
    list<<item;

    item = new QStandardItem;
    item->setData(DateEnd, TypeRole);
    list<<item;

    appendRow(list);
}

void MedAdmissionModel::choosePerson()
{
    //выводит диалог окно с выбором человека из личного состава
    ChoosePersonDialog* personDialog = new ChoosePersonDialog();
    personDialog->setModal(true);
    personDialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(personDialog, SIGNAL(sendId(int)), this, SLOT(setPersonId(int)));
    personDialog->exec();
}

void MedAdmissionModel::remove(const QModelIndex &index)
{
    int id = index.sibling(index.row(), 0).data(IdRole).toInt();
    if (id > 0 && !idToDelList.contains(id))
        idToDelList<<id;

    removeRow(index.row());
}

bool MedAdmissionModel::save()
{
    ODS::DBManager db = ODS::OdsInterface::self()->dbManager();

    db.transaction();
    ODS::OdsInterface::self()->syncManager().syncTransaction();

    foreach(int id, idToDelList){
        if (!ODS::OdsInterface::self()->iobjectManager().deleteIObjectById(id)){
            db.rollback();
            return false;
        }
    }

    for (int i = 0; i< rowCount(); i++){
        QStandardItem* item = this->item(i, 0);

        if (!item->data(ChangedRole).toBool())
            continue;

        ODS::IObject obj;
        int id = item->data(IdRole).toInt();

        if (id > 0)
            obj = ODS::OdsInterface::self()->iobjectManager().getIObjectById(id);
        else
            obj = ODS::OdsInterface::self()->iobjectManager().createIObject("Допуска КП.Допуска мед");
        if (!saveItem(item, obj)){
            db.rollback();
            return false;
        }
    }

    db.commit();
    return true;
}

bool MedAdmissionModel::saveItem(QStandardItem *item, ODS::IObject &obj)
{
    qDebug()<<Q_FUNC_INFO;

    QStandardItem* idItem = this->item(item->row(), 0);
    bool isUpdate = item->data(IdRole).toInt() > 0;

    int column = 0;
    while(item)
    {
        switch (item->data(TypeRole).toInt())
        {
        case Num:
            obj.setAttr("Порядковый номер", item->text().toInt());
            obj.setAttr("Вч", currentVcid);
            obj.setAttr("Человек", item->data(PersonIdRole).toInt());
            break;
        case Rank:
            break;
        case Post:
            break;
        case Fio:
            break;
        case Date:
            break;
        case Date2:{
            CDate cDate;
            QDateTime dt;
            if (item->text() == ""){
                dt = QDateTime::currentDateTime();
            } else {
                dt = QDateTime::fromString(item->text(), DATE_FORMAT);
            }
            cDate.setDateTime(dt.toTime_t());
            obj.setAttr("Дата прохождения ВВК", cDate);
            break;
        }
        case Term:
            obj.setAttr("Срок заключения ВВК", item->text().toInt());
            break;
        case Result:
            if (item->data(AttestResultRole).toInt() == 1){
                obj.setAttr("Заключение", true);
            } else {
                obj.setAttr("Заключение", false);
            }
            break;
        case DateEnd:
            CDate cDate;
            QDateTime dt;
            if (item->text() == ""){
                dt = QDateTime::currentDateTime();
            } else {
                dt = QDateTime::fromString(item->text(), DATE_FORMAT);
            }
            cDate.setDateTime(dt.toTime_t());
            obj.setAttr("Дата окончания", cDate);
            break;
        }

        item = this->item(item->row(), ++column);
    }

    bool b = false;
    if (isUpdate)
        b = ODS::OdsInterface::self()->iobjectManager().updateIObject(obj, ODS::IObject::NotRecursiveUpdate);
    else {
        if (ODS::OdsInterface::self()->iobjectManager().saveIObject(obj, ODS::IObject::NotRecursiveSave))
            b = setIObjRights(obj);
    }

    if (b){
        idItem->setData(obj.id(), IdRole);
        idItem->setData(false, ChangedRole);
    }
    return b;
}

void MedAdmissionModel::changeVc(int vcId)
{
    removeRows(0, rowCount());
    currentVcid = vcId;

    ODS::IObjectCursor cursor = ODS::OdsInterface::self()->iobjectManager().getIObjectCursor("Допуска КП.Допуска мед",
                                                                           QString("\"Вч\"='%1'").arg(vcId));
    ODS::IObjectCursor::iterator it = cursor.begin();
    while (it != cursor.end())
    {
        ODS::IObject obj = *it;
        makeRow(obj);
        ++it;
    }
}
