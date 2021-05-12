#include "personadmissionmodel.h"
#include <OdsInterface>
#include <CatalogManager>
#include <QMessageBox>
#include "global_defs.h"

PersonAdmissionModel::PersonAdmissionModel(int vcId, QObject *parent) :
    QStandardItemModel(parent),
    iface(ODS::OdsInterface::self()),
    currentVcid(vcId)
{
    init();
}

PersonAdmissionModel::~PersonAdmissionModel()
{
}

void PersonAdmissionModel::init()
{
    load();
}

void PersonAdmissionModel::load()
{
    removeRows(0, rowCount());

    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"Вч\" = " + QString::number(currentVcid);

    ODS::IObjectCursor cursor = ODS::OdsInterface::self()->iobjectManager().getIObjectCursor("Допуска КП.Личный состав",filter);

    ODS::IObjectCursor::iterator it = cursor.begin();
    while (it != cursor.end())
    {
        ODS::IObject obj = *it;
        makeRow(obj);
        ++it;
    }
}

void PersonAdmissionModel::makeRow(const ODS::IObject &obj)
{
    QList<QStandardItem*> list;

    QStandardItem* item = new QStandardItem;
    item->setData(Num, TypeRole);
    item->setText(QString::number(obj.getIntAttr("Порядковый номер")));
    item->setData(obj.id(), IdRole);
    item->setData(false, ChangedRole);
    list<<item;

    item = new QStandardItem;
    item->setData(Rank, TypeRole);
    IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
    IRS::Catalog::ptr catalog = catalMgr.getCatalogByName("Классификатор воинских званий");
    if (catalog){
        if (obj.getIntAttr("Воинское звание") != 0){
            IRS::CatalogEntry::ptr entry = catalog->entryById(obj.getIntAttr("Воинское звание"));
            item->setText(entry->stringValue("Наименование"));
            item->setData(entry->id(), IrsIdRole);
        }
    } else {
        QMessageBox::critical(0,"Ошибка", "Недействительный 'Классификатор воинских званий'");
    }
    list<<item;

    item = new QStandardItem;
    item->setData(Post, TypeRole);
    catalog = catalMgr.getCatalogByName("Классификатор должностей");
    if (catalog){
        if (obj.getIntAttr("Должность") != 0){
            IRS::CatalogEntry::ptr entry = catalog->entryById(obj.getIntAttr("Должность"));
            item->setText(entry->stringValue("Наименование"));
            item->setData(entry->id(), IrsIdRole);
        }
    } else {
        QMessageBox::critical(0,"Ошибка", "Недействительный 'Классификатор должностей'");
    }
    list<<item;

    item = new QStandardItem;
    item->setData(Fio, TypeRole);
    item->setText(obj.getStringAttr("ФИО"));
    list<<item;

    item = new QStandardItem;
    item->setData(Date, TypeRole);
    QDateTime datetime;
    datetime.setTime_t(obj.getDateTimeAttr("Дата назначения").getDateTime());
    item->setText(datetime.toString("dd.MM.yyyy"));
    list<<item;

    appendRow(list);
}

void PersonAdmissionModel::addEmptyRow()
{
    QList<QStandardItem*> list;

    QStandardItem* item = new QStandardItem;
    item->setData(Num, TypeRole);
    item->setData(-1, IdRole);
    item->setData(false, ChangedRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Rank, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Post, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Fio, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Date, TypeRole);
    list<<item;

    appendRow(list);
}

void PersonAdmissionModel::remove(const QModelIndex &index)
{
    int id = index.sibling(index.row(), 0).data(IdRole).toInt();
    if (id > 0 && !idToDelList.contains(id))
        idToDelList<<id;

    removeRow(index.row());
}

bool PersonAdmissionModel::save()
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
            obj = ODS::OdsInterface::self()->iobjectManager().createIObject("Допуска КП.Личный состав");
        if (!saveItem(item, obj)){
            db.rollback();
            return false;
        }
    }

    db.commit();
    return true;
}

bool PersonAdmissionModel::saveItem(QStandardItem *item, ODS::IObject &obj)
{
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
            break;
        case Rank:
            obj.setAttr("Воинское звание", item->data(IrsIdRole).toInt());
            break;
        case Post:
            obj.setAttr("Должность", item->data(IrsIdRole).toInt());
            break;
        case Fio:
            obj.setAttr("ФИО", item->text());
            break;
        case Date:
            CDate cDate;
            QDateTime dt;
            if (item->text() == ""){
                dt = QDateTime::currentDateTime();
            } else {
                dt = QDateTime::fromString(item->text(), DATE_FORMAT);
            }
            cDate.setDateTime(dt.toTime_t());
            obj.setAttr("Дата назначения", cDate);
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

void PersonAdmissionModel::changeVc(int vcId)
{
    removeRows(0, rowCount());
    currentVcid = vcId;

    ODS::IObjectCursor cursor = ODS::OdsInterface::self()->iobjectManager().getIObjectCursor("Допуска КП.Личный состав",
                                                                           QString("\"Вч\"='%1'").arg(vcId));
    ODS::IObjectCursor::iterator it = cursor.begin();
    while (it != cursor.end())
    {
        ODS::IObject obj = *it;
        makeRow(obj);
        ++it;
    }
}



