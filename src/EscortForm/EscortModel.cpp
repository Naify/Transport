#include "EscortForm/EscortModel.h"
#include <OdsInterface>
#include "global_defs.h"

EscortModel::EscortModel(int order_id, ODS::OdsInterface* iface, QObject *parent) :
    QStandardItemModel(parent),
    m_iface(iface),
    m_order_id(order_id)
{
    init();
}
//
EscortModel::~EscortModel()
{

}
//
void EscortModel::init()
{
    if (m_iface)
        load();
}
//
void EscortModel::addEmptyRow()
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
    item->setData(LastName, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(FirstName, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(MidName, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(With, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(WeaponName, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(WeaponNum, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(CarType, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(CarNum, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Comment, TypeRole);
    list<<item;

    appendRow(list);
}
//
void EscortModel::removeEscort(const QModelIndex& index)
{
    int id = index.sibling(index.row(), 0).data(IdRole).toInt();
    if (id > 0 && !m_idToDelList.contains(id))
        m_idToDelList<<id;

    removeRow(index.row());
}
//
void EscortModel::load()
{
    removeRows(0, rowCount());
    //
    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"Распоряжение\" = " + QString::number(m_order_id);

    ODS::IObjectCursor cursor = m_iface->iobjectManager().getIObjectCursor("Состав команды.Сопровождение",filter);
    ODS::IObjectCursor::iterator it = cursor.begin();
    while (it != cursor.end())
    {
        ODS::IObject obj = *it;
        makeRow(obj);
        ++it;
    }
}
//
void EscortModel::makeRow(const ODS::IObject& obj)
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
    item->setData(LastName, TypeRole);
    item->setText(obj.getStringAttr("Фамилия"));
    list<<item;
    item = new QStandardItem;
    item->setData(FirstName, TypeRole);
    item->setText(obj.getStringAttr("Имя"));
    list<<item;
    item = new QStandardItem;
    item->setData(MidName, TypeRole);
    item->setText(obj.getStringAttr("Отчество"));
    list<<item;
    item = new QStandardItem;
    item->setData(With, TypeRole);
    item->setData(obj.getIntAttr("В группе с"), ConvoyRole);
    list<<item;
    item = new QStandardItem;
    item->setData(WeaponName, TypeRole);
    item->setText(obj.getStringAttr("Наимен. вооруж."));
    list<<item;
    item = new QStandardItem;
    item->setData(WeaponNum, TypeRole);
    item->setText(obj.getStringAttr("Номер вооруж."));
    list<<item;
    item = new QStandardItem;
    item->setData(CarType, TypeRole);
    item->setText(obj.getStringAttr("Назнач. вагона"));
    list<<item;
    item = new QStandardItem;
    item->setData(CarNum, TypeRole);
    item->setText(obj.getStringAttr("Номер вагона"));
    list<<item;
    item = new QStandardItem;
    item->setData(Comment, TypeRole);
    item->setText(obj.getStringAttr("Примечание"));
    list<<item;

    appendRow(list);
}
//
bool EscortModel::save()
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
            obj = m_iface->iobjectManager().createIObject("Состав команды.Сопровождение");
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
bool EscortModel::saveItem(QStandardItem* item, ODS::IObject& obj)
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
            break;
        case Rank:
            obj.setAttr("В/звание", item->text());
            break;
        case LastName:
            obj.setAttr("Фамилия", item->text());
            break;
        case FirstName:
            obj.setAttr("Имя", item->text());
            break;
        case MidName:
            obj.setAttr("Отчество", item->text());
            break;
        case With:
            if (item->data(ConvoyRole).toInt() > 0)
                obj.setAttr("В группе с", item->data(ConvoyRole).toInt());
            else
                obj.setAttr("В группе с", -1);
            break;
        case WeaponName:
            obj.setAttr("Наимен. вооруж.", item->text());
            break;
        case WeaponNum:
            obj.setAttr("Номер вооруж.", item->text());
            break;
        case CarType:
            obj.setAttr("Назнач. вагона", item->text());
            break;
        case CarNum:
            obj.setAttr("Номер вагона", item->text());
            break;
        case Comment:
            obj.setAttr("Примечание", item->text());
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

void EscortModel::changeOrder(int order)
{
    removeRows(0, rowCount());
    m_order_id = order;
    //
    ODS::IObjectCursor cursor = m_iface->iobjectManager().getIObjectCursor("Состав команды.Сопровождение",
                                                                           QString("\"Распоряжение\"='%1'").arg(order));
    ODS::IObjectCursor::iterator it = cursor.begin();
    while (it != cursor.end())
    {
        ODS::IObject obj = *it;
        makeRow(obj);
        ++it;
    }
}
