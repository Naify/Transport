#include "TravelAuthForm/TravelAuthtorizationModel.h"
#include <OdsInterface>

TravelAuthtorizationModel::TravelAuthtorizationModel(int order_id, ODS::OdsInterface* iface, QObject *parent) :
    QStandardItemModel(parent),
    m_iface(iface),
    m_order_id(order_id)
{
    init();
}
//
TravelAuthtorizationModel::~TravelAuthtorizationModel()
{

}
//
void TravelAuthtorizationModel::init()
{
    setHorizontalHeaderLabels(QStringList()
                              << "Должность"
                              << "В/звание"
                              << "Фамилия"
                              << "Имя"
                              << "Отчество"
                              << "С ним следуют");

    if (m_iface)
        load();
}
//
void TravelAuthtorizationModel::load()
{
    removeRows(0, rowCount());
    //
    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"Распоряжение\" = " + QString::number(m_order_id);

    ODS::IObjectCursor cursor = m_iface->iobjectManager().getIObjectCursor("Состав команды.Сопровождение", filter);
    ODS::IObjectCursor::iterator it = cursor.begin();
    while (it != cursor.end())
    {
        ODS::IObject obj = *it;
        makeRow(obj);
        ++it;
    }
}
//
void TravelAuthtorizationModel::makeRow(const ODS::IObject& obj)
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

    int id = obj.getIntAttr("Сопровождает");
    if (id > 0)
    {
        for (int i =0; i< rowCount(); i++)
        {
            if (this->item(i, 0)->data(IdRole).toInt() == id)
            {
                item->setText(QString("%1 %2")
                              .arg(this->item(i,0)->text())
                              .arg(this->item(i, 1)->text()));
                break;
            }
        }
    }

    list<<item;

    appendRow(list);
}
//
void TravelAuthtorizationModel::changeOrder(int order)
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

