#include "claimmodel.h"
#include "ClaimForm/claimform.h"
#include <OdsInterface>
#include <CatalogManager>
#include <QMessageBox>
#include "global_defs.h"

ClaimModel::ClaimModel(int proID, QObject *parent) :
    QStandardItemModel(parent),
    iface(ODS::OdsInterface::self()),
    projId(proID)
{
}

ClaimModel::~ClaimModel()
{
}

void ClaimModel::load()
{
    removeRows(0, rowCount());

    ODS::IObjectQueryFilter filter;
    filter.orderCondition = "\"io_id\" ASC";
    filter.whereCondition = "\"Заявка\" = " + QString::number(projId);

    ODS::IObjectCursor cursor = ODS::OdsInterface::self()->iobjectManager().getIObjectCursor("Контроль перевозок.Содержание заявок",filter);

    ODS::IObjectCursor::iterator it = cursor.begin();
    while (it != cursor.end())
    {
        ODS::IObject obj = *it;
        makeRow(obj);
        ++it;
    }
}

void ClaimModel::makeRow(const ODS::IObject &obj)
{
    QList<QStandardItem*> list;

    QStandardItem* item = new QStandardItem;
    item->setData(Num, TypeRole);
    item->setText(QString::number(obj.getIntAttr("Порядковый номер")));
    item->setData(obj.id(), IdRole);
    item->setData(false, ChangedRole);
    list<<item;

    item = new QStandardItem;
    item->setData(Sender, TypeRole);
    IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
    IRS::Catalog::ptr catalog;
    int senderId = obj.getIntAttr("Отправитель");
    if (senderId){
        catalog = catalMgr.getCatalogByName("Классификатор воинских частей");
        if (catalog){
            IRS::CatalogEntry::ptr entry = catalog->entryById(senderId);
            if (entry.isNull()){
                catalog = catalMgr.getCatalogByName("Классификатор предприятий промышленности");
                if (catalog){
                    entry = catalog->entryById(senderId);
                    if (entry.isNull()){
                        QMessageBox::critical(0,"Ошибка", "Не найден идентификатор из 'Классификатор предприятий промышленности' или 'Классификатор воинских частей'");
                    } else {
                        item->setText(entry->stringValue("Сокращенное наименование"));
                        item->setData(entry->id(), IrsRole);
                    }
                } else {
                    QMessageBox::critical(0,"Ошибка", "Недействительный 'Классификатор предприятий промышленности'");
                }
            } else {
                item->setText(entry->stringValue("Условное наименование"));
                item->setData(entry->id(), IrsRole);
            }
        } else {
            QMessageBox::critical(0,"Ошибка", "Недействительный 'Классификатор воинских частей'");
        }
    }
    list<<item;

    item = new QStandardItem;
    item->setData(SendStation, TypeRole);
    item->setText(obj.getStringAttr("Станция отправления"));
    list<<item;

    item = new QStandardItem;
    item->setData(DateTimeCar, TypeRole);
    QDateTime datetime;
    datetime.setTime_t(obj.getDateTimeAttr("Дата время подачи").getDateTime());
    item->setText(datetime.toString("dd.MM.yyyy hh:mm"));
    list<<item;

    item = new QStandardItem;
    item->setData(CarType, TypeRole);
    int carTypeId = obj.getIntAttr("Тип вагонов");
    if (carTypeId){
        if (ClaimForm::getTrType() == "жд"){
            catalog = catalMgr.getCatalogByName("Классификатор вагонов по типам");
            if (catalog){
                IRS::CatalogEntry::ptr entry = catalog->entryById(carTypeId);
                item->setText(entry->stringValue("Тип вагона"));
                item->setData(entry->id(), IrsRole);
            } else {
                QMessageBox::critical(0,"Ошибка", "Недействительный каталог вагонов по типам");
            }
        } else if (ClaimForm::getTrType() == "авто"){
            catalog = catalMgr.getCatalogByName("Классификатор автомобильной техники");
            if (catalog){
                IRS::CatalogEntry::ptr entry = catalog->entryById(carTypeId);
                item->setText(entry->stringValue("Наименование"));
                item->setData(entry->id(), IrsRole);
            } else {
                QMessageBox::critical(0,"Ошибка", "Недействительный каталог автомобильной техники");
            }
        } else {
            item->setText(obj.getStringAttr("Тип вагонов строка"));
        }
    }
    list<<item;

    item = new QStandardItem;
    item->setData(Count, TypeRole);
    item->setText(QString::number(obj.getIntAttr("Количество")));
    list<<item;

    item = new QStandardItem;
    item->setData(Numbers, TypeRole);
    item->setText(obj.getStringAttr("Номера вагонов"));
    list<<item;

    item = new QStandardItem;
    item->setData(DateTimeSend, TypeRole);
    datetime.setTime_t(obj.getDateTimeAttr("Дата время отправления").getDateTime());
    item->setText(datetime.toString("dd.MM.yyyy hh:mm"));
    list<<item;

    item = new QStandardItem;
    item->setData(ArriveStation, TypeRole);
    item->setText(obj.getStringAttr("Станция назначения"));
    list<<item;

    item = new QStandardItem;
    item->setData(Reciever, TypeRole);
    int recieverId = obj.getIntAttr("Отправитель");
    if (recieverId){
        catalog = catalMgr.getCatalogByName("Классификатор воинских частей");
        if (catalog){
            IRS::CatalogEntry::ptr entry = catalog->entryById(obj.getIntAttr("Получатель"));
            if (entry.isNull()){
                catalog = catalMgr.getCatalogByName("Классификатор предприятий промышленности");
                if (catalog){
                    entry = catalog->entryById(obj.getIntAttr("Получатель"));
                    if (entry.isNull()){
                        QMessageBox::critical(0,"Ошибка", "Не найден идентификатор из 'Классификатор предприятий промышленности' или 'Классификатор воинских частей'");
                    } else {
                        item->setText(entry->stringValue("Сокращенное наименование"));
                        item->setData(entry->id(), IrsRole);
                    }
                } else {
                    QMessageBox::critical(0,"Ошибка", "Недействительный 'Классификатор предприятий промышленности'");
                }
            } else {
                item->setText(entry->stringValue("Условное наименование"));
                item->setData(entry->id(), IrsRole);
            }
        } else {
            QMessageBox::critical(0,"Ошибка", "Недействительный 'Классификатор воинских частей'");
        }
    }
    list<<item;

    item = new QStandardItem;
    item->setData(Oficers, TypeRole);
    item->setText(QString::number(obj.getIntAttr("Офицеры")));
    list<<item;

    item = new QStandardItem;
    item->setData(Ensignes, TypeRole);
    item->setText(QString::number(obj.getIntAttr("Прапорщики")));
    list<<item;

    item = new QStandardItem;
    item->setData(Sergants, TypeRole);
    item->setText(QString::number(obj.getIntAttr("Сержанты")));
    list<<item;

    item = new QStandardItem;
    item->setData(Soldier, TypeRole);
    item->setText(QString::number(obj.getIntAttr("Солдаты")));
    list<<item;

    appendRow(list);
}

void ClaimModel::addEmptyRow()
{
    QList<QStandardItem*> list;

    QStandardItem* item = new QStandardItem;
    item->setData(Num, TypeRole);
    item->setData(-1, IdRole);
    item->setData(false, ChangedRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Sender, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(SendStation, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(DateTimeCar, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(CarType, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Count, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Numbers, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(DateTimeSend, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(ArriveStation, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Reciever, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Oficers, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Ensignes, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Sergants, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Soldier, TypeRole);
    list<<item;

    appendRow(list);
}

void ClaimModel::remove(const QModelIndex &index)
{
    int id = index.sibling(index.row(), 0).data(IdRole).toInt();
    if (id > 0 && !idToDelList.contains(id))
        idToDelList<<id;

    removeRow(index.row());
}

bool ClaimModel::save()
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
            obj = ODS::OdsInterface::self()->iobjectManager().createIObject("Контроль перевозок.Содержание заявок");
        if (!saveItem(item, obj)){
            db.rollback();
            return false;
        }
    }

    db.commit();
    return true;
}

bool ClaimModel::saveItem(QStandardItem *item, ODS::IObject &obj)
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
            obj.setAttr("Заявка", projId);
            break;
        case Sender:
            obj.setAttr("Отправитель", item->data(IrsRole).toInt());
            break;
        case SendStation:
            obj.setAttr("Станция отправления", item->text());
            break;
        case DateTimeCar:{
            CDate cDate;
            QDateTime dt;
            if (item->text() == ""){
                dt = QDateTime::currentDateTime();
            } else {
                dt = QDateTime::fromString(item->text(), "dd.MM.yyyy hh:mm");
            }
            cDate.setDateTime(dt.toTime_t());
            obj.setAttr("Дата время подачи", cDate);
            break;
        }
        case CarType:
            if (ClaimForm::getTrType() == "жд" || ClaimForm::getTrType() == "авто"){
                obj.setAttr("Тип вагонов", item->data(IrsRole).toInt());
            } else {
                obj.setAttr("Тип вагонов строка", item->text());
            }
            break;
        case Count:
            obj.setAttr("Количество", item->text().toInt());
            break;
        case Numbers:
            obj.setAttr("Номера вагонов", item->text());
            break;
        case DateTimeSend:{
            CDate cDate;
            QDateTime dt;
            if (item->text() == ""){
                dt = QDateTime::currentDateTime();
            } else {
                dt = QDateTime::fromString(item->text(), "dd.MM.yyyy hh:mm");
            }
            cDate.setDateTime(dt.toTime_t());
            obj.setAttr("Дата время отправления", cDate);
            break;
        }
        case ArriveStation:
            obj.setAttr("Станция назначения", item->text());
            break;
        case Reciever:
            obj.setAttr("Получатель", item->data(IrsRole).toInt());
            break;
        case Oficers:
            obj.setAttr("Офицеры", item->text().toInt());
            break;
        case Ensignes:
            obj.setAttr("Прапорщики", item->text().toInt());
            break;
        case Sergants:
            obj.setAttr("Сержанты", item->text().toInt());
            break;
        case Soldier:
            obj.setAttr("Солдаты", item->text().toInt());
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

void ClaimModel::changeProj(int projectId)
{
    projId = projectId;
}


