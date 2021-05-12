#include "AutoForm/AutoTransportModel.h"
#include <OdsInterface>
#include "global_defs.h"

AutoTransportModel::AutoTransportModel(int order_id, int column_num, ODS::OdsInterface* iface, QObject *parent) :
    QStandardItemModel(parent),
    m_iface(iface),
    m_order_id(order_id),
    m_column_num(column_num)
{
    init();
}
//
AutoTransportModel::~AutoTransportModel()
{

}
//
void AutoTransportModel::init()
{
    if (m_iface)
        load();
}
//
void AutoTransportModel::addEmptyRow()
{
    QList<QStandardItem*> list;

    QStandardItem* item = new QStandardItem;
    item->setData(Post, TypeRole);
    item->setData(-1, IdRole);
    item->setData(false, ChangedRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Rank, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(FIO, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Mission, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(CarType, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(CarNum, TypeRole);
    list<<item;

    appendRow(list);
}
//
void AutoTransportModel::removeAutoTransport(const QModelIndex& index)
{
    int id = index.sibling(index.row(), 0).data(IdRole).toInt();
    if (id > 0 && !m_idToDelList.contains(id))
        m_idToDelList<<id;

    removeRow(index.row());
}
//
void AutoTransportModel::load()
{
    removeRows(0, rowCount());

    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"Номер автоколонны\" = " + QString::number(m_column_num) + " AND \"Распоряжение\" = " + QString::number(m_order_id);

    ODS::IObjectCursor cursor = m_iface->iobjectManager().getIObjectCursor("Транспортные средства.Автоколонна",filter);
//                                                                           QString("\"Номер автоколонны\"='%1'").arg(m_column_num));
//                                                                           QString("\"Номер автоколонны\"='%1' AND \"Распоряжение\"='%2'").arg(m_column_num, m_order_id));

    ODS::IObjectCursor::iterator it = cursor.begin();
    while (it != cursor.end())
    {
        ODS::IObject obj = *it;
        makeRow(obj);
        ++it;
    }
}
//
void AutoTransportModel::load(int column_num)
{
    m_column_num = column_num;
    load();
}
//
void AutoTransportModel::makeRow(const ODS::IObject& obj)
{
    QList<QStandardItem*> list;

    QStandardItem* item = new QStandardItem;
    item->setData(Post, TypeRole);
    item->setText(obj.getStringAttr("Должность"));
    item->setData(obj.id(), IdRole);
    item->setData(false, ChangedRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Rank, TypeRole);
    item->setText(obj.getStringAttr("В/звание"));
    list<<item;
    item = new QStandardItem;
    item->setData(FIO, TypeRole);
    item->setText(obj.getStringAttr("ФИО"));
    list<<item;
    item = new QStandardItem;
    item->setData(Mission, TypeRole);
    item->setText(obj.getStringAttr("Назначение"));
    list<<item;
    item = new QStandardItem;
    item->setData(CarType, TypeRole);
    item->setText(obj.getStringAttr("Марка"));
    list<<item;
    item = new QStandardItem;
    item->setData(CarNum, TypeRole);
    item->setText(obj.getStringAttr("Номер"));
    list<<item;

    appendRow(list);
}
//
bool AutoTransportModel::save()
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

    for (int i = 0; i< rowCount(); i++)
    {
        QStandardItem* item = this->item(i, 0);
        if (!item->data(ChangedRole).toBool()){
            continue;
        }

        ODS::IObject obj;
        int id = item->data(IdRole).toInt();
        if (id > 0)
            obj = m_iface->iobjectManager().getIObjectById(id);
        else
            obj = m_iface->iobjectManager().createIObject("Транспортные средства.Автоколонна");

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
bool AutoTransportModel::saveItem(QStandardItem* item, ODS::IObject& obj)
{
    QStandardItem* idItem = this->item(item->row(), 0);
    bool isUpdate = item->data(IdRole).toInt() > 0;

    int column = 0;
    while(item)
    {
        switch (item->data(TypeRole).toInt())
        {
        case Post:
            obj.setAttr("Должность", item->text());
            obj.setAttr("Распоряжение", m_order_id);
            obj.setAttr("Номер автоколонны", m_column_num);
        case Rank:
            obj.setAttr("В/звание", item->text());
            break;
        case FIO:
            obj.setAttr("ФИО", item->text());
            break;
        case Mission:
            obj.setAttr("Назначение", item->text());
            break;
        case CarType:
            obj.setAttr("Марка", item->text());
            break;
        case CarNum:
            obj.setAttr("Номер", item->text());
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
//
bool AutoTransportModel::removeAutocolumnMoreThan(int column_count)
{
    return m_iface->iobjectManager().deleteIObjectByCondition("Транспортные средства.Автоколонна",
                                                   QString("\"Номер автоколонны\">'%1'").arg(column_count));
}

void AutoTransportModel::changeOrder(int order)
{
    removeRows(0, rowCount());
    m_order_id = order;

    ODS::IObjectCursor cursor = m_iface->iobjectManager().getIObjectCursor("Транспортные средства.Автоколонна",
                                                                           QString("\"Распоряжение\"='%1'").arg(order));
    ODS::IObjectCursor::iterator it = cursor.begin();
    while (it != cursor.end())
    {
        ODS::IObject obj = *it;
        makeRow(obj);
        ++it;
    }
}
