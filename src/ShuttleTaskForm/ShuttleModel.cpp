#include "ShuttleTaskForm/ShuttleModel.h"
#include <OdsInterface>
#include "global_defs.h"

ShuttleModel::ShuttleModel(int shuttle_id, ODS::OdsInterface* iface, QObject *parent) :
    QStandardItemModel(parent),
    m_iface(iface),
    m_shuttle_id(shuttle_id)
{
    init();
}
//
ShuttleModel::~ShuttleModel()
{

}
//
void ShuttleModel::init()
{
    if (m_iface)
        load();
}
//
void ShuttleModel::addEmptyRow()
{
    QList<QStandardItem*> list;

    QStandardItem* item = new QStandardItem;
    item->setData(Num, TypeRole);
    item->setData(-1, IdRole);
    item->setData(false, ChangedRole);
    list<<item;
    item = new QStandardItem;
    item->setData(NumTrain, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Pa, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Pnvt, TypeRole);
    list<<item;

    appendRow(list);
}
//
//void ShuttleModel::remove(const QModelIndex& index)
//{
//    int id = index.sibling(index.row(), 0).data(IdRole).toInt();
//    if (id > 0 && !m_idToDelList.contains(id))
//        m_idToDelList<<id;
//
//    removeRow(index.row());
//}
//
void ShuttleModel::load()
{
    removeRows(0, rowCount());
    //
    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"Марш задание\" = " + QString::number(m_shuttle_id);

    ODS::IObjectCursor cursor = m_iface->iobjectManager().getIObjectCursor("Контроль перевозок.Станции ПА и ПНВТ",filter);
    ODS::IObjectCursor::iterator it = cursor.begin();
    while (it != cursor.end())
    {
        ODS::IObject obj = *it;
        makeRow(obj);
        ++it;
    }
}

void ShuttleModel::makeRow(const ODS::IObject& obj)
{
    QList<QStandardItem*> list;

    QStandardItem* item = new QStandardItem;
    item->setData(Num, TypeRole);
    item->setText(QString::number(obj.getIntAttr("Порядковый номер")));
    item->setData(obj.id(), IdRole);
    item->setData(false, ChangedRole);
    list<<item;
    item = new QStandardItem;
    item->setData(NumTrain, TypeRole);
    item->setText(obj.getStringAttr("Номер ЖДЭ"));
    list<<item;
    item = new QStandardItem;
    item->setData(Pa, TypeRole);
    item->setText(obj.getStringAttr("Станция ПА"));
    list<<item;
    item = new QStandardItem;
    item->setData(Pnvt, TypeRole);
    item->setText(obj.getStringAttr("ПНВТ"));
    list<<item;

    appendRow(list);
}

//
/*
bool ShuttleModel::save()
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
        if (!item->data(ChangedRole).toBool())
            continue;

        ODS::IObject obj;
        int id = item->data(IdRole).toInt();
        if (id > 0)
            obj = m_iface->iobjectManager().getIObjectById(id);
        else
            obj = m_iface->iobjectManager().createIObject("Контроль перевозок.Станции ПА и ПНВТ");
        if (!saveItem(item, obj))
        {
            db.rollback();
            return false;
        }
    }

    db.commit();
    return true;
}
*/
//
/*
bool ShuttleModel::saveItem(QStandardItem* item, ODS::IObject& obj)
{
    QStandardItem* idItem = this->item(item->row(), 0);
    bool isUpdate = item->data(IdRole).toInt() > 0;

    int column = 0;
    while(item)
    {
        switch (item->data(TypeRole).toInt())
        {
        case Num:
            obj.setAttr("Порядковый номер", item->text());
            obj.setAttr("Марш задание", m_shuttle_id);
            break;
        case NumTrain:
            obj.setAttr("Номер ЖДЭ", item->text());
            break;
        case Pa:
            obj.setAttr("Станция ПА", item->text());
            break;
        case Pnvt:
            obj.setAttr("ПНВТ", item->text());
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
        idItem->setData(obj.id(), IdRole);
        idItem->setData(false, ChangedRole);
    }

    return b;
}
*/
void ShuttleModel::setId(int id)
{
    m_shuttle_id = id;
}

void ShuttleModel::changeShuttle(int shuttle)
{
    removeRows(0, rowCount());
    m_shuttle_id = shuttle;
    //
    ODS::IObjectCursor cursor = m_iface->iobjectManager().getIObjectCursor("Контроль перевозок.Станции ПА и ПНВТ",
                                                                           QString("\"Марш задание\"='%1'").arg(shuttle));
    ODS::IObjectCursor::iterator it = cursor.begin();
    while (it != cursor.end())
    {
        ODS::IObject obj = *it;
        makeRow(obj);
        ++it;
    }
}
