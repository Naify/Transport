#include "ShipmentForm/ShipmentModel.h"
#include <OdsInterface>
#include "global_defs.h"

ShipmentModel::ShipmentModel(int order_id, ODS::OdsInterface* iface, QObject *parent) :
    QStandardItemModel(parent),
    m_iface(iface),
    m_order_id(order_id)
{
    init();
}
//
ShipmentModel::~ShipmentModel()
{

}
//
void ShipmentModel::init()
{
    if (m_iface)
        load();
}
//
void ShipmentModel::addEmptyRow()
{
    QList<QStandardItem*> list;

    QStandardItem* item = new QStandardItem;
    item->setData(NomNum, TypeRole);
    item->setData(-1, IdRole);
    item->setData(false, ChangedRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Num, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(SG, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Block, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(ContainerSeal, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(TzuType, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(TzuNum, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(TzuSeal, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Team, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Building, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Auto, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Run, TypeRole);
    list<<item;

    appendRow(list);
}
//
//void ShipmentModel::removeShipment(const QModelIndex& index)
//{
//    int id = index.sibling(index.row(), 0).data(IdRole).toInt();
//    if (id > 0 && !m_idToDelList.contains(id))
//        m_idToDelList<<id;
//
//    removeRow(index.row());
//}
//
void ShipmentModel::load()
{
    removeRows(0, rowCount());
    //
    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"Распоряжение\" = " + QString::number(m_order_id);

    ODS::IObjectCursor cursor = m_iface->iobjectManager().getIObjectCursor(CARGO_TYPE,filter);
    ODS::IObjectCursor::iterator it = cursor.begin();
    while (it != cursor.end())
    {
        ODS::IObject obj = *it;
        makeRow(obj);
        ++it;
    }
}
//
void ShipmentModel::makeRow(const ODS::IObject& obj)
{
    QList<QStandardItem*> list;

    QStandardItem* item = new QStandardItem;
    item->setData(NomNum, TypeRole);
    item->setText(obj.getStringAttr("Номенклатура"));
    item->setData(obj.id(), IdRole);
    item->setData(false, ChangedRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Num, TypeRole);
    item->setText(obj.getStringAttr("Зав. ном-р"));
    list<<item;    
    item = new QStandardItem;
    item->setData(SG, TypeRole);
    item->setText(obj.getStringAttr("СГ"));
    list<<item;
    item = new QStandardItem;
    item->setData(Block, TypeRole);
    item->setText(obj.getStringAttr("Блок"));
    list<<item;
    item = new QStandardItem;
    item->setData(ContainerSeal, TypeRole);
    item->setText(obj.getStringAttr("Пломба тары"));
    list<<item;
    item = new QStandardItem;
    item->setData(TzuType, TypeRole);
    item->setText(obj.getStringAttr("Тип ТЗУ"));
    list<<item;
    item = new QStandardItem;
    item->setData(TzuNum, TypeRole);
    item->setText(obj.getStringAttr("Зав. ном-р ТЗУ"));
    list<<item;
    item = new QStandardItem;
    item->setData(TzuSeal, TypeRole);
    item->setText(obj.getStringAttr("Пломба ТЗУ"));
    list<<item;
    item = new QStandardItem;
    item->setData(Team, TypeRole);
    item->setText(obj.getStringAttr("Бригада"));
    list<<item;
    item = new QStandardItem;
    item->setData(Building, TypeRole);
    item->setText(obj.getStringAttr("Сооружение"));
    list<<item;
    item = new QStandardItem;
    item->setData(Auto, TypeRole);
    item->setText(obj.getStringAttr("Автоколонна"));
    list<<item;
    item = new QStandardItem;
    item->setData(Run, TypeRole);
    item->setText(obj.getStringAttr("Рейс"));
    list<<item;
    appendRow(list);
}
//
bool ShipmentModel::save()
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

        qDebug()<<i<<item->data(ChangedRole).toBool();
        if (!item->data(ChangedRole).toBool())
            continue;

        ODS::IObject obj;
        int id = item->data(IdRole).toInt();
        if (id > 0)
            obj = m_iface->iobjectManager().getIObjectById(id);
        else
            obj = m_iface->iobjectManager().createIObject(CARGO_TYPE);
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
bool ShipmentModel::saveItem(QStandardItem* item, ODS::IObject& obj)
{
    QStandardItem* idItem = this->item(item->row(), 0);
    bool isUpdate = item->data(IdRole).toInt() > 0;

    int column = 0;
    while(item)
    {
        switch (item->data(TypeRole).toInt())
        {
        case NomNum:
            obj.setAttr(NOMENKLAT_ATTR, item->text());
            obj.setAttr(ORDER_ATTR, m_order_id);
            break;
        case Num:
            obj.setAttr(ZAV_NUMB_ATTR, item->text());
            break;
        case SG:
            obj.setAttr(READYNESS_STATUS_ATTR, item->text());
            break;
        case Block:
            obj.setAttr(BLOCK2_ATTR, item->text());
            break;
        case ContainerSeal:
            obj.setAttr(PLOMB_ATTR, item->text());
            break;
        case TzuType:
            obj.setAttr(TZU_ATTR, item->text());
            break;
        case TzuNum:
            obj.setAttr(ZAV_TZU_ATTR, item->text());
            break;
        case TzuSeal:
            obj.setAttr(PLOMB_TZU_ATTR, item->text());
            break;
        case Team:
            obj.setAttr("Бригада", item->text());
            break;
        case Building:
            obj.setAttr("Сооружение", item->text());
            break;
        case Auto:
            obj.setAttr("Автоколонна", item->text());
            break;
        case Run:
            obj.setAttr("Рейс", item->text());
            break;
        }

        item = this->item(item->row(), ++column);
    }

    bool b = false;
    if (isUpdate)
        b = m_iface->iobjectManager().updateIObject(obj, ODS::IObject::NotRecursiveUpdate);
    else {
        if (m_iface->iobjectManager().saveIObject(obj, ODS::IObject::NotRecursiveSave))
            b = setIObjRights(obj);
    }

    if (b)
    {
        //QStandardItem * item2 = this->item(item->row(), 0);
        idItem->setData(obj.id(), IdRole);
        idItem->setData(false, ChangedRole);
    }
    return b;
}

void ShipmentModel::changeOrder(int order)
{
    removeRows(0, rowCount());
    m_order_id = order;
    //
    ODS::IObjectCursor cursor = m_iface->iobjectManager().getIObjectCursor(CARGO_TYPE,
                                                                           QString("\"Распоряжение\"='%1'").arg(order));
    ODS::IObjectCursor::iterator it = cursor.begin();
    while (it != cursor.end())
    {
        ODS::IObject obj = *it;
        makeRow(obj);
        ++it;
    }
}
