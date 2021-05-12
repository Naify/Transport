#include "supportmachinesmodel.h"
#include "OdsInterface"
#include <CatalogManager>
#include <QMessageBox>
#include "global_defs.h"

SupportMachinesModel::SupportMachinesModel(QObject *parent):
    QStandardItemModel(parent),
    iface(ODS::OdsInterface::self())
{
    init();
}

SupportMachinesModel::~SupportMachinesModel()
{
}

void SupportMachinesModel::init()
{
    load();
}

void SupportMachinesModel::load()
{
    removeRows(0, rowCount());

    ODS::IObjectCursor cursor = ODS::OdsInterface::self()->iobjectManager().getIObjectCursor("Определение сил и средств.Машины обеспечения");

    ODS::IObjectCursor::iterator it = cursor.begin();
    while (it != cursor.end()) {
        ODS::IObject obj = *it;
        makeRow(obj);
        ++it;
    }
}

void SupportMachinesModel::makeRow(const ODS::IObject &obj)
{
    QList<QStandardItem*> list;

    QStandardItem* item = new QStandardItem;
    item->setData(Num, TypeRole);
    item->setText(QString::number(obj.getIntAttr("Порядковый номер")));
    item->setData(obj.id(), IdRole);
    item->setData(false, ChangedRole);
    list<<item;

    item = new QStandardItem;
    item->setData(Name, TypeRole);
    item->setText(obj.getStringAttr("Наименование"));
    list<<item;

    item = new QStandardItem;
    item->setData(Designation, TypeRole);
    item->setText(obj.getStringAttr("ОбозначениеСтрока"));
    list<<item;

    item = new QStandardItem;
    item->setData(FactoryNumb, TypeRole);
    item->setText(obj.getStringAttr("Заводской номер"));
    list<<item;

    item = new QStandardItem;
    item->setData(MilitaryNumb, TypeRole);
    item->setText(obj.getStringAttr("Воен номер"));
    list<<item;

    item = new QStandardItem;
    item->setData(DateRelease, TypeRole);
    QDateTime datetime;
    datetime.setTime_t(obj.getDateTimeAttr("Год выпуска").getDateTime());
    item->setText(datetime.toString("dd.MM.yyyy"));
    list<<item;

    item = new QStandardItem;
    item->setData(Ready, TypeRole);
    obj.getBoolAttr("Готовность") ? item->setText("Готов") : item->setText("Не готов");
    if (!obj.getBoolAttr("Готовность"))
        item->setBackground(Qt::red);
    list<<item;

    appendRow(list);
}

void SupportMachinesModel::addEmptyRow()
{
    QList<QStandardItem*> list;

    QStandardItem* item = new QStandardItem;
    item->setData(Num, TypeRole);
    item->setData(-1, IdRole);
    item->setData(false, ChangedRole);
    list<<item;

    item = new QStandardItem;
    item->setData(Name, TypeRole);
    list<<item;

    item = new QStandardItem;
    item->setData(Designation, TypeRole);
    list<<item;

    item = new QStandardItem;
    item->setData(FactoryNumb, TypeRole);
    list<<item;

    item = new QStandardItem;
    item->setData(MilitaryNumb, TypeRole);
    list<<item;

    item = new QStandardItem;
    item->setData(DateRelease, TypeRole);
    list<<item;

    item = new QStandardItem;
    item->setData(Ready, TypeRole);
    list<<item;

    appendRow(list);
}

void SupportMachinesModel::remove(const QModelIndex &index)
{
    int id = index.sibling(index.row(), 0).data(IdRole).toInt();
    if (id > 0 && !idToDelList.contains(id))
        idToDelList<<id;

    removeRow(index.row());
}

bool SupportMachinesModel::save()
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
            obj = ODS::OdsInterface::self()->iobjectManager().createIObject("Определение сил и средств.Машины обеспечения");
        if (!saveItem(item, obj)){
            db.rollback();
            return false;
        }
    }

    db.commit();
    return true;
}

bool SupportMachinesModel::saveItem(QStandardItem *item, ODS::IObject &obj)
{
    QStandardItem* idItem = this->item(item->row(), 0);
    bool isUpdate = item->data(IdRole).toInt() > 0;

    int column = 0;
    CDate cDate;
    QDateTime dt;
    while(item)
    {
        switch (item->data(TypeRole).toInt())
        {
        case Num:
            obj.setAttr("Порядковый номер", item->text().toInt());
            break;
        case Name:{
            obj.setAttr("Наименование", item->text());
            break;
        }
        case Designation:{
            obj.setAttr("ОбозначениеСтрока", item->text());
            break;
        }
        case FactoryNumb:{
            obj.setAttr("Заводской номер", item->text());
            break;
        }
        case MilitaryNumb:{
            obj.setAttr("Воен номер", item->text());
            break;
        }
        case DateRelease:{
            if (item->text() == ""){
                dt = QDateTime::currentDateTime();
            } else {
                dt = QDateTime::fromString(item->text(), DATE_FORMAT);
            }
            cDate.setDateTime(dt.toTime_t());
            obj.setAttr("Год выпуска", cDate);
            break;
        }
        case Ready:
            item->text() == "Готов" ? obj.setAttr("Готовность", true) : obj.setAttr("Готовность", false);
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
