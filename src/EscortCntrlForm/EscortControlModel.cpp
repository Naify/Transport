#include "EscortCntrlForm/EscortControlModel.h"
#include <OdsInterface>
#include "global_defs.h"

EscortControlModel::EscortControlModel(ODS::OdsInterface* iface, QObject *parent) :
    QStandardItemModel(parent),
    m_iface(iface)
{
    init();
}
//
EscortControlModel::~EscortControlModel()
{

}
//
void EscortControlModel::init()
{
    if (m_iface)
        load();
}
//
void EscortControlModel::load()
{
    removeRows(0, rowCount());
    //
    QString query=QString("SELECT "
                          "\"Документы КП\".\"Командировочное удостоверение\".\"Дата приказа\", "
                          "\"Документы КП\".\"Командировочное удостоверение\".\"Дата начала\", "
                          "\"Документы КП\".\"Командировочное удостоверение\".\"Дата окончания\", "
                          "\"Документы КП\".\"Командировочное удостоверение\".\"Пункты назначения\", "
                          "\"Документы КП\".\"Командировочное удостоверение\".\"Порядковый номер\", "

                          "\"Состав команды\".\"Сопровождение\".\"В/звание\", "
                          "\"Состав команды\".\"Сопровождение\".\"Фамилия\", "
                          "\"Состав команды\".\"Сопровождение\".\"Имя\", "
                          "\"Состав команды\".\"Сопровождение\".\"Отчество\" "

//                          "\"Учет\".\"Командировка\".\"io_id\", "
//                          "\"Учет\".\"Командировка\".\"Дата начала\", "
//                          "\"Учет\".\"Командировка\".\"Дата окончания\", "
//                          "\"Учет\".\"Командировка\".\"Примечание\" "

                          "FROM \"Состав команды\".\"Сопровождение\" "

                          "INNER JOIN \"Документы КП\".\"Командировочное удостоверение\" "
                          "ON \"Документы КП\".\"Командировочное удостоверение\".\"Распоряжение\" = "
                          "\"Состав команды\".\"Сопровождение\".\"Распоряжение\" "

//                          "LEFT JOIN \"Учет\".\"Командировка\" "
//                          "ON \"Учет\".\"Командировка\".\"Командированный\" = "
//                          "\"Состав команды\".\"Сопровождение\".\"io_id\" "

                          "WHERE (\"Состав команды\".\"Сопровождение\".\"В группе с\" < 1 OR "
                          "\"Состав команды\".\"Сопровождение\".\"В группе с\" IS NULL) "

                          //не работает !!!!1
//                          "GROUP BY \"Состав команды\".\"Сопровождение\".\"Распоряжение\""
                          );



    ODS::IORecordCursor cursor = m_iface->iobjectManager().exec(query);
    qDebug()<<"cursor size = "<<cursor.size();
    ODS::IORecordCursor::iterator it = cursor.begin();
    while (it != cursor.end())
    {
        ODS::IORecord obj = *it;
        makeRow(obj);
        ++it;
    }
}
//
void EscortControlModel::makeRow(const ODS::IORecord& obj)
{
    QList<QStandardItem*> list;

    QStandardItem* item = new QStandardItem;
    item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
    item->setText(obj.getStringAttr("Документы КП.Командировочное удостоверение.Порядковый номер"));
    item->setData(obj.getIntAttr("Учет.Командировка.io_id"), IdRole);
    list<<item;
    item = new QStandardItem;
    item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
    CDate date = obj.getDateTimeAttr("Документы КП.Командировочное удостоверение.Дата приказа");
    QDateTime dt = QDateTime::fromTime_t(date.getDateTime());
    item->setText(dt.date().toString("dd.MM.yy"));
    list<<item;
    item = new QStandardItem;
    item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
    item->setText(obj.getStringAttr("Состав команды.Сопровождение.В/звание"));
    list<<item;

    QString fio;
    QString tmp;
    fio = obj.getStringAttr("Состав команды.Сопровождение.Фамилия");
    tmp = obj.getStringAttr("Состав команды.Сопровождение.Имя");
    if (!tmp.isEmpty())
        fio += QString(" %1.").arg(tmp.at(0));
    tmp = obj.getStringAttr("Состав команды.Сопровождение.Отчество");
    if (!tmp.isEmpty())
        fio += QString(" %1.").arg(tmp.at(0));

    item = new QStandardItem;
    item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
    item->setText(fio);
    list<<item;
    item = new QStandardItem;
    item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
    item->setText(obj.getStringAttr("Документы КП.Командировочное удостоверение.Пункты назначения"));
    list<<item;

    // !!!!! Цель командировки
    item = new QStandardItem;
    item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
    // item->setText(obj.getIntAttr("Цель"));
    list<<item;

    item = new QStandardItem;
    item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
    date = obj.getDateTimeAttr("Документы КП.Командировочное удостоверение.Дата начала");
    dt = QDateTime::fromTime_t(date.getDateTime());
    item->setText(dt.date().toString("dd.MM.yy"));
    list<<item;

    item = new QStandardItem;
    item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable);
    item->setData(In, TypeRole);
    date = obj.getDateTimeAttr("Учет.Командировка.Дата начала");
    dt = QDateTime::fromTime_t(date.getDateTime());
    item->setText(dt.date().toString("dd.MM.yy"));
    list<<item;

    item = new QStandardItem;
    item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
    date = obj.getDateTimeAttr("Документы КП.Командировочное удостоверение.Дата окончания");
    dt = QDateTime::fromTime_t(date.getDateTime());
    item->setText(dt.date().toString("dd.MM.yy"));
    list<<item;

    item = new QStandardItem;
    item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable);
    item->setData(Out, TypeRole);
    date = obj.getDateTimeAttr("Учет.Командировка.Дата окончания");
    dt = QDateTime::fromTime_t(date.getDateTime());
    item->setText(dt.date().toString("dd.MM.yy"));
    list<<item;

    item = new QStandardItem;
    item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable);
    item->setData(Comment, TypeRole);
    item->setText(obj.getStringAttr("Учет.Командировка.Примечание"));
    list<<item;

    appendRow(list);
}
//
bool EscortControlModel::save(const QModelIndex& index)
{
    QModelIndex idIndex = index.sibling(index.row(), 0);
    int id = idIndex.data(IdRole).toInt();
    bool isUpdate = id > 0;

    ODS::IObject obj;
    if (isUpdate)
        obj = m_iface->iobjectManager().getIObjectById(id);
    else
        obj = m_iface->iobjectManager().createIObject("Учет.Командировка");

    switch (index.data(TypeRole).toInt())
    {
    case In:
    {
        CDate date;
        date.setDateTime(QDateTime::fromString(index.data().toString(), "dd.MM.yy").toTime_t());
        obj.setAttr("Дата начала", date);
        break;
    }
    case Out:
    {
        CDate date;
        date.setDateTime(QDateTime::fromString(index.data().toString(), "dd.MM.yy").toTime_t());
        obj.setAttr("Дата окончания", date);
        break;
    }
    case Comment:
        obj.setAttr("Примечание", index.data().toString());
        break;
    }

    ODS::DBManager db = m_iface->dbManager();
    db.transaction();

    ODS::OdsInterface::self()->syncManager().syncTransaction();

    bool b = false;
    if (isUpdate)
        b = m_iface->iobjectManager().updateIObject(obj, ODS::IObject::NotRecursiveUpdate) ;
    else
        if (m_iface->iobjectManager().saveIObject(obj, ODS::IObject::NotRecursiveSave))
            b = setIObjRights(obj);

    qDebug()<<"b = "<<b;
    obj.dumpInfo();

    if (b)
    {
        db.commit();
        setData(idIndex, obj.id(), IdRole);
    }
    else
    {
        qDebug()<<"Error";
        db.rollback();
        return false;
    }
    return true;
}
//
