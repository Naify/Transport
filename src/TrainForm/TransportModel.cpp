#include "TrainForm/TransportModel.h"
#include <OdsInterface>
#include "global_defs.h"

TransportModel::TransportModel(int order_id, ODS::OdsInterface* iface, QObject *parent) :
    QStandardItemModel(parent),
    m_iface(iface),
    m_order_id(order_id)
{
    init();
}
//
TransportModel::~TransportModel()
{

}
//
void TransportModel::init()
{
    if (m_iface)
        load();
}
//
void TransportModel::addEmptyRow()
{
    QList<QStandardItem*> list;

    QStandardItem* item = new QStandardItem;
    item->setData(CarType, TypeRole);
    item->setData(-1, IdRole);
    item->setData(false, ChangedRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Mission, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Num, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Flank, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(FlankNum, TypeRole);
    list<<item;

    appendRow(list);
}
//
void TransportModel::removeTransport(const QModelIndex& index)
{
    int id = index.sibling(index.row(), 0).data(IdRole).toInt();
    if (id > 0 && !m_idToDelList.contains(id))
        m_idToDelList<<id;

    removeRow(index.row());
}
//
void TransportModel::load()
{
    removeRows(0, rowCount());
    //
    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"Распоряжение\" = " + QString::number(m_order_id);

    ODS::IObjectCursor cursor = m_iface->iobjectManager().getIObjectCursor("Транспортные средства.Вагоны", filter);
    ODS::IObjectCursor::iterator it = cursor.begin();
    while (it != cursor.end())
    {
        ODS::IObject obj = *it;
        makeRow(obj);
        ++it;
    }
}
//
void TransportModel::makeRow(const ODS::IObject& obj)
{
    QList<QStandardItem*> list;

    QStandardItem* item = new QStandardItem;
    item->setData(CarType, TypeRole);
    item->setText(obj.getStringAttr("Тип вагона"));
    item->setData(obj.id(), IdRole);
    item->setData(false, ChangedRole);

    list<<item;
    item = new QStandardItem;
    item->setData(Mission, TypeRole);
    item->setText(obj.getStringAttr("Назначение"));
    list<<item;
    item = new QStandardItem;
    item->setData(Num, TypeRole);
    item->setText(obj.getStringAttr("Регистр. номер"));
    list<<item;
    item = new QStandardItem;
    item->setData(Flank, TypeRole);
    item->setText(obj.getStringAttr("Фланг"));
    list<<item;
    item = new QStandardItem;
    item->setData(FlankNum, TypeRole);
    item->setText(QString::number(obj.getIntAttr("Номер во фланге")));
    list<<item;

    appendRow(list);
}
//
bool TransportModel::save()
{
    ODS::DBManager db = m_iface->dbManager();
    db.transaction();

    ODS::OdsInterface::self()->syncManager().syncTransaction();

    foreach(int id, m_idToDelList)
    {
        if (!m_iface->iobjectManager().deleteIObjectById(id))
        {
            db.rollback();
            return false;
        }
    }
    //
    for (int i = 0; i< rowCount(); i++)
    {
        QStandardItem* item = this->item(i, 0);
        if (!item->data(ChangedRole).toBool())
            continue;

        ODS::IObject obj;
        int id = item->data(IdRole).toInt();
        if (id > 0)
            obj = m_iface->iobjectManager().getIObjectById(id);
        else
            obj = m_iface->iobjectManager().createIObject("Транспортные средства.Вагоны");
        if (!saveItem(item, obj))
        {
            db.rollback();
            return false;
        }
    }

    db.commit();
    return true;
}
//
bool TransportModel::saveItem(QStandardItem* item, ODS::IObject& obj)
{
    QStandardItem* idItem = this->item(item->row(), 0);
    bool isUpdate = item->data(IdRole).toInt() > 0;

    int column = 0;
    while(item)
    {
        switch (item->data(TypeRole).toInt())
        {
        case CarType:
            obj.setAttr("Тип вагона", item->text());
            obj.setAttr("Распоряжение", m_order_id);
            break;
        case Mission:
            obj.setAttr("Назначение", item->text());
            break;
        case Num:
            obj.setAttr("Регистр. номер", item->text());
            break;
        case Flank:
            obj.setAttr("Фланг", item->text());
            break;
        case FlankNum:
            obj.setAttr("Номер во фланге", item->text().toInt());
            break;
        }

        item = this->item(item->row(), ++column);
    }

    bool b = false;
    if (isUpdate)
        b = m_iface->iobjectManager().updateIObject(obj, ODS::IObject::NotRecursiveUpdate);
    else
        if (m_iface->iobjectManager().saveIObject(obj, ODS::IObject::NotRecursiveSave))
            b = setIObjRights(obj);

    if (b)
    {
//        item = this->item(item->row(), 0);
        idItem->setData(obj.id(), IdRole);
        idItem->setData(false, ChangedRole);
    }

    return b;
}

void TransportModel::changeOrder(int order)
{
    removeRows(0, rowCount());
    m_order_id = order;
    //
    ODS::IObjectCursor cursor = m_iface->iobjectManager().getIObjectCursor("Транспортные средства.Вагоны",
                                                                           QString("\"Распоряжение\"='%1'").arg(order));
    ODS::IObjectCursor::iterator it = cursor.begin();
    while (it != cursor.end())
    {
        ODS::IObject obj = *it;
        makeRow(obj);
        ++it;
    }
}
