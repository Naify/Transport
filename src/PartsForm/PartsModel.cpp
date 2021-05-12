#include "PartsForm/PartsModel.h"
#include <OdsInterface>
#include "global_defs.h"

PartsModel::PartsModel(int order_id, ODS::OdsInterface* iface, QObject *parent) :
    QStandardItemModel(parent),
    m_iface(iface),
    m_order_id(order_id)
{
    init();
}
//
PartsModel::~PartsModel()
{

}
//
void PartsModel::init()
{
    if (m_iface)
        load();
}
//
void PartsModel::addEmptyRow()
{
    QList<QStandardItem*> list;

    QStandardItem* item = new QStandardItem;
    item->setData(Name, TypeRole);
    item->setData(-1, IdRole);
    item->setData(false, ChangedRole);
    list<<item;
    item = new QStandardItem;
    item->setData(NomNum, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Index, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Num, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Grif, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Code, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(PlaceCount, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(Seal, TypeRole);
    list<<item;
    item = new QStandardItem;
    item->setData(SealCount, TypeRole);
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
void PartsModel::removeParts(const QModelIndex& index)
{
    int id = index.sibling(index.row(), 0).data(IdRole).toInt();
    if (id > 0 && !m_idToDelList.contains(id))
        m_idToDelList<<id;

    removeRow(index.row());
}
//
void PartsModel::load()
{
    removeRows(0, rowCount());
    //
    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"Распоряжение\" = " + QString::number(m_order_id);

    ODS::IObjectCursor cursor = m_iface->iobjectManager().getIObjectCursor("Перечень грузов.Запчасти", filter);
    ODS::IObjectCursor::iterator it = cursor.begin();
    while (it != cursor.end())
    {
        ODS::IObject obj = *it;
        makeRow(obj);
        ++it;
    }
}
//
void PartsModel::makeRow(const ODS::IObject& obj)
{
    QList<QStandardItem*> list;

    QStandardItem* item = new QStandardItem;
    item->setData(Name, TypeRole);
    item->setText(obj.getStringAttr("Наименование"));
    item->setData(obj.id(), IdRole);
    item->setData(false, ChangedRole);
    list<<item;
    item = new QStandardItem;
    item->setData(NomNum, TypeRole);
    item->setText(obj.getStringAttr("Номенклатура"));
    list<<item;
    item = new QStandardItem;
    item->setData(Index, TypeRole);
    item->setText(obj.getStringAttr("Индекс"));
    list<<item;
    item = new QStandardItem;
    item->setData(Num, TypeRole);
    item->setText(obj.getStringAttr("Зав. ном-р"));
    list<<item;
    item = new QStandardItem;
    item->setData(Grif, TypeRole);
    item->setData(obj.getIntAttr("Гриф"), GrifRole);
    item->setText(textByGrif(obj.getIntAttr("Гриф")));
    list<<item;
    item = new QStandardItem;
    item->setData(Code, TypeRole);
    item->setText(obj.getStringAttr("Шифр"));
    list<<item;
    item = new QStandardItem;
    item->setData(PlaceCount, TypeRole);
    item->setText(obj.getStringAttr("К-во мест"));
    list<<item;
    item = new QStandardItem;
    item->setData(Seal, TypeRole);
    item->setText(obj.getStringAttr("Пломба"));
    list<<item;
    item = new QStandardItem;
    item->setData(SealCount, TypeRole);
    item->setText(obj.getStringAttr("К-во пломб"));
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
bool PartsModel::save()
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
            obj = m_iface->iobjectManager().createIObject("Перечень грузов.Запчасти");
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
bool PartsModel::saveItem(QStandardItem* item, ODS::IObject& obj)
{
    QStandardItem* idItem = this->item(item->row(), 0);
    bool isUpdate = item->data(IdRole).toInt() > 0;

    int column = 0;
    while(item)
    {
        switch (item->data(TypeRole).toInt())
        {
        case Name:
            obj.setAttr(NAME_ATTR, item->text());
            obj.setAttr(ORDER_ATTR, m_order_id);
            break;
        case NomNum:
            obj.setAttr(NOMENKLAT_ATTR, item->text());
            break;
        case Index:
            obj.setAttr("Индекс", item->text());
            break;
        case Num:
            obj.setAttr("Зав. ном-р", item->text());
            break;
        case Grif:
            obj.setAttr("Гриф", item->data(GrifRole).toInt());
            break;
        case Code:
            obj.setAttr("Шифр", item->text());
            break;
        case PlaceCount:
            obj.setAttr("К-во мест", item->text());
            break;
        case Seal:
            obj.setAttr("Пломба", item->text());
            break;
        case SealCount:
            obj.setAttr("К-во пломб", item->text());
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
//
QString PartsModel::textByGrif(int grif)
{
    switch (grif)
    {
    case 0:
        return "нс";
        break;
    case 1:
        return "с";
        break;
    case 2:
        return "сс";
        break;
    }
    return QString();
}

void PartsModel::changeOrder(int order)
{
    removeRows(0, rowCount());
    m_order_id = order;
    //
    ODS::IObjectCursor cursor = m_iface->iobjectManager().getIObjectCursor("Перечень грузов.Запчасти",
                                                                           QString("\"Распоряжение\"='%1'").arg(order));
    ODS::IObjectCursor::iterator it = cursor.begin();
    while (it != cursor.end())
    {
        ODS::IObject obj = *it;
        makeRow(obj);
        ++it;
    }
}
