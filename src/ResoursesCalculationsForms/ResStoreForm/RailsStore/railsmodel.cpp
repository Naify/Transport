#include "railsmodel.h"
#include "OdsInterface"
#include <CatalogManager>
#include <QMessageBox>
#include "global_defs.h"

RailsModel::RailsModel(QObject *parent):
    QStandardItemModel(parent),
    iface(ODS::OdsInterface::self())
{
    init();
}

RailsModel::~RailsModel()
{
}

void RailsModel::init()
{
    load();
}

void RailsModel::load()
{
    removeRows(0, rowCount());

    ODS::IObjectCursor cursor = ODS::OdsInterface::self()->iobjectManager().getIObjectCursor("Определение сил и средств.Спец вагоны");

    ODS::IObjectCursor::iterator it = cursor.begin();
    while (it != cursor.end()) {
        ODS::IObject obj = *it;
        makeRow(obj);
        ++it;
    }
}

void RailsModel::makeRow(const ODS::IObject &obj)
{
    QList<QStandardItem*> list;

    QStandardItem* item = new QStandardItem;
    item->setData(Num, TypeRole);
    item->setText(QString::number(obj.getIntAttr("Порядковый номер")));
    item->setData(obj.id(), IdRole);
    item->setData(false, ChangedRole);
    list<<item;

    item = new QStandardItem;
    item->setData(Designation, TypeRole);
    IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
    IRS::Catalog::ptr catalog = catalMgr.getCatalogByName("Классификатор вагонов по типам");
    int design = obj.getIntAttr("Обозначение");
    if (design == 0 || design ==-1){
        item->setText("");
        item->setData(design, DesignatRole);
    } else {
        if (catalog){
            IRS::CatalogEntry::ptr entry = catalog->entryById(design);
            item->setText(entry->stringValue("Тип вагона"));
            item->setData(entry->id(), DesignatRole);
        } else
            QMessageBox::critical(0,"Ошибка", "Недействительный 'Классификатор вагонов по типам'");
    }
    list<<item;


    item = new QStandardItem;
    item->setData(FactoryNumb, TypeRole);
    item->setText(obj.getStringAttr("Заводской номер"));
    list<<item;

    item = new QStandardItem;
    item->setData(CommonNumb, TypeRole);
    item->setText(obj.getStringAttr("Общесетевой номер"));
    list<<item;

    item = new QStandardItem;
    item->setData(DateRelease, TypeRole);
    QDateTime datetime;
    datetime.setTime_t(obj.getDateTimeAttr("Год постройки").getDateTime());
    item->setText(datetime.toString("dd.MM.yyyy"));
    list<<item;

    item = new QStandardItem;
    item->setData(DateLastEtr, TypeRole);
    datetime.setTime_t(obj.getDateTimeAttr("Дата посл ЕТР").getDateTime());
    item->setText(datetime.toString("dd.MM.yyyy"));
    list<<item;

    item = new QStandardItem;
    item->setData(DateLastDr, TypeRole);
    datetime.setTime_t(obj.getDateTimeAttr("Дата посл ДР").getDateTime());
    item->setText(datetime.toString("dd.MM.yyyy"));
    list<<item;

    item = new QStandardItem;
    item->setData(DateLastZr, TypeRole);
    datetime.setTime_t(obj.getDateTimeAttr("Дата посл ЗР").getDateTime());
    item->setText(datetime.toString("dd.MM.yyyy"));
    list<<item;

    item = new QStandardItem;
    item->setData(Ready, TypeRole);
    obj.getBoolAttr("Готовность") ? item->setText("Готов") : item->setText("Не готов");
    if (!obj.getBoolAttr("Готовность"))
        item->setBackground(Qt::red);
    list<<item;

    item = new QStandardItem;
    item->setData(DateNextEtr, TypeRole);
    datetime.setTime_t(obj.getDateTimeAttr("Дата очеред ЕТР").getDateTime());
    item->setText(datetime.toString("dd.MM.yyyy"));
    list<<item;

    item = new QStandardItem;
    item->setData(DateNextDr, TypeRole);
    datetime.setTime_t(obj.getDateTimeAttr("Дата очеред ДР").getDateTime());
    item->setText(datetime.toString("dd.MM.yyyy"));
    list<<item;

    item = new QStandardItem;
    item->setData(DateNextZr, TypeRole);
    datetime.setTime_t(obj.getDateTimeAttr("Дата очеред ЗР").getDateTime());
    item->setText(datetime.toString("dd.MM.yyyy"));
    list<<item;

    item = new QStandardItem;
    item->setData(SecurityType, TypeRole);
    item->setText(obj.getStringAttr("Тип охранной системы"));
    list<<item;

    item = new QStandardItem;
    item->setData(Away, TypeRole);
    obj.getBoolAttr("Отсутствует") ? item->setText("Отсутствует") : item->setText("Присутствует");
    if (obj.getBoolAttr("Отсутствует"))
        item->setBackground(Qt::red);
    list<<item;

    appendRow(list);
}

void RailsModel::addEmptyRow()
{
    QList<QStandardItem*> list;

    QStandardItem* item = new QStandardItem;
    item->setData(Num, TypeRole);
    item->setData(-1, IdRole);
    item->setData(false, ChangedRole);
    list<<item;

    item = new QStandardItem;
    item->setData(Designation, TypeRole);
    item->setData(-1, DesignatRole);
    list<<item;

    item = new QStandardItem;
    item->setData(FactoryNumb, TypeRole);
    list<<item;

    item = new QStandardItem;
    item->setData(CommonNumb, TypeRole);
    list<<item;

    item = new QStandardItem;
    item->setData(DateRelease, TypeRole);
    list<<item;

    item = new QStandardItem;
    item->setData(DateLastEtr, TypeRole);
    list<<item;

    item = new QStandardItem;
    item->setData(DateLastDr, TypeRole);
    list<<item;

    item = new QStandardItem;
    item->setData(DateLastZr, TypeRole);
    list<<item;

    item = new QStandardItem;
    item->setData(Ready, TypeRole);
    list<<item;

    item = new QStandardItem;
    item->setData(DateNextEtr, TypeRole);
    list<<item;

    item = new QStandardItem;
    item->setData(DateNextDr, TypeRole);
    list<<item;

    item = new QStandardItem;
    item->setData(DateNextZr, TypeRole);
    list<<item;

    item = new QStandardItem;
    item->setData(SecurityType, TypeRole);
    list<<item;

    item = new QStandardItem;
    item->setData(Away, TypeRole);
    list<<item;

    appendRow(list);
}

void RailsModel::remove(const QModelIndex &index)
{
    int id = index.sibling(index.row(), 0).data(IdRole).toInt();
    if (id > 0 && !idToDelList.contains(id))
        idToDelList<<id;

    removeRow(index.row());
}

bool RailsModel::save()
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
            obj = ODS::OdsInterface::self()->iobjectManager().createIObject("Определение сил и средств.Спец вагоны");
        if (!saveItem(item, obj)){
            db.rollback();
            return false;
        }
    }

    db.commit();
    return true;
}

bool RailsModel::saveItem(QStandardItem *item, ODS::IObject &obj)
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
        case Designation:{
            ODS::IObject iobj = ODS::OdsInterface::self()->iobjectManager().getIObjectById(item->data(DesignatRole).toInt());
            obj.setAttr("Обозначение", iobj);
            break;
        }
        case FactoryNumb:{
            obj.setAttr("Заводской номер", item->text());
            break;
        }
        case CommonNumb:{
            obj.setAttr("Общесетевой номер", item->text());
            break;
        }
        case DateRelease:{
            if (item->text() == ""){
                dt = QDateTime::currentDateTime();
            } else {
                dt = QDateTime::fromString(item->text(), DATE_FORMAT);
            }
            cDate.setDateTime(dt.toTime_t());
            obj.setAttr("Год постройки", cDate);
            break;
        }
        case DateLastEtr:{
            if (item->text() == ""){
                dt = QDateTime::currentDateTime();
            } else {
                dt = QDateTime::fromString(item->text(), DATE_FORMAT);
            }
            cDate.setDateTime(dt.toTime_t());
            obj.setAttr("Дата посл ЕТР", cDate);
            break;
        }
        case DateLastDr:{
            if (item->text() == ""){
                dt = QDateTime::currentDateTime();
            } else {
                dt = QDateTime::fromString(item->text(), DATE_FORMAT);
            }
            cDate.setDateTime(dt.toTime_t());
            obj.setAttr("Дата посл ДР", cDate);
            break;
        }
        case DateLastZr:{
            if (item->text() == ""){
                dt = QDateTime::currentDateTime();
            } else {
                dt = QDateTime::fromString(item->text(), DATE_FORMAT);
            }
            cDate.setDateTime(dt.toTime_t());
            obj.setAttr("Дата посл ЗР", cDate);
            break;
        }
        case Ready:
            if (item->text() == "Готов"){
                obj.setAttr("Готовность", true);
                item->setBackground(Qt::white);
            } else {
                item->setBackground(Qt::red);
                obj.setAttr("Готовность", false);
            }
            break;
        case DateNextEtr:{
            if (item->text() == ""){
                dt = QDateTime::currentDateTime();
            } else {
                dt = QDateTime::fromString(item->text(), DATE_FORMAT);
            }
            cDate.setDateTime(dt.toTime_t());
            obj.setAttr("Дата очеред ЕТР", cDate);
            break;
        }
        case DateNextDr:{
            if (item->text() == ""){
                dt = QDateTime::currentDateTime();
            } else {
                dt = QDateTime::fromString(item->text(), DATE_FORMAT);
            }
            cDate.setDateTime(dt.toTime_t());
            obj.setAttr("Дата очеред ДР", cDate);
            break;
        }
        case DateNextZr:
            if (item->text() == ""){
                dt = QDateTime::currentDateTime();
            } else {
                dt = QDateTime::fromString(item->text(), DATE_FORMAT);
            }
            cDate.setDateTime(dt.toTime_t());
            obj.setAttr("Дата очеред ЗР", cDate);
            break;
        case SecurityType:{
            obj.setAttr("Тип охранной системы", item->text());
            break;
        }
        case Away:{
            if (item->text() == "Отсутствует"){
                obj.setAttr("Отсутствует", true);
                item->setBackground(Qt::red);
            } else {
                obj.setAttr("Отсутствует", false);
                item->setBackground(Qt::white);
            }
        }
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
