#include "carsmodel.h"
#include "OdsInterface"
#include <CatalogManager>
#include <QMessageBox>
#include "global_defs.h"

CarsModel::CarsModel(QObject *parent):
    QStandardItemModel(parent),
    iface(ODS::OdsInterface::self())
{
    init();
}

CarsModel::~CarsModel()
{
}

void CarsModel::init()
{
    load();
}

void CarsModel::load()
{
    removeRows(0, rowCount());

    ODS::IObjectCursor cursor = ODS::OdsInterface::self()->iobjectManager().getIObjectCursor("Определение сил и средств.Спец авто");

    ODS::IObjectCursor::iterator it = cursor.begin();
    while (it != cursor.end()) {
        ODS::IObject obj = *it;
        makeRow(obj);
        ++it;
    }
}

void CarsModel::makeRow(const ODS::IObject &obj)
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
    IRS::Catalog::ptr catalog = catalMgr.getCatalogByName("Классификатор автомобильной техники");
    int design = obj.getIntAttr("Обозначение");
    if (design == 0 || design ==-1){
        item->setText("");
        item->setData(design, DesignatRole);
    } else {
        if (catalog){
            IRS::CatalogEntry::ptr entry = catalog->entryById(design);
            item->setText(entry->stringValue("Наименование"));
            item->setData(entry->id(), DesignatRole);
        } else
            QMessageBox::critical(0,"Ошибка", "Недействительный 'Классификатор автомобильной техники'");
    }
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
    item->setData(DatePere, TypeRole);
    datetime.setTime_t(obj.getDateTimeAttr("Дата посл переосвид").getDateTime());
    item->setText(datetime.toString("dd.MM.yyyy"));
    list<<item;

    item = new QStandardItem;
    item->setData(Ready, TypeRole);
    obj.getBoolAttr("Готовность") ? item->setText("Готов") : item->setText("Не готов");
    if (!obj.getBoolAttr("Готовность"))
        item->setBackground(Qt::red);
    list<<item;

    item = new QStandardItem;
    item->setData(DateO, TypeRole);
    datetime.setTime_t(obj.getDateTimeAttr("Дата очеред освид").getDateTime());
    item->setText(datetime.toString("dd.MM.yyyy"));
    list<<item;

    item = new QStandardItem;
    item->setData(Note, TypeRole);
    item->setText(obj.getStringAttr("Примечание"));
    list<<item;

    item = new QStandardItem;
    item->setData(Away, TypeRole);
    obj.getBoolAttr("Отсутствует") ? item->setText("Отсутствует") : item->setText("Присутствует");
    if (obj.getBoolAttr("Отсутствует"))
        item->setBackground(Qt::red);
    list<<item;

    appendRow(list);
}

void CarsModel::addEmptyRow()
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
    item->setData(MilitaryNumb, TypeRole);
    list<<item;

    item = new QStandardItem;
    item->setData(DateRelease, TypeRole);
    list<<item;

    item = new QStandardItem;
    item->setData(DatePere, TypeRole);
    list<<item;

    item = new QStandardItem;
    item->setData(Ready, TypeRole);
    list<<item;

    item = new QStandardItem;
    item->setData(DateO, TypeRole);
    list<<item;

    item = new QStandardItem;
    item->setData(Note, TypeRole);
    list<<item;

    item = new QStandardItem;
    item->setData(Away, TypeRole);
    list<<item;

    appendRow(list);
}

void CarsModel::remove(const QModelIndex &index)
{
    int id = index.sibling(index.row(), 0).data(IdRole).toInt();
    if (id > 0 && !idToDelList.contains(id))
        idToDelList<<id;

    removeRow(index.row());
}

bool CarsModel::save()
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
            obj = ODS::OdsInterface::self()->iobjectManager().createIObject("Определение сил и средств.Спец авто");
        if (!saveItem(item, obj)){
            db.rollback();
            return false;
        }
    }

    db.commit();
    return true;
}

bool CarsModel::saveItem(QStandardItem *item, ODS::IObject &obj)
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
        case DatePere:{
            if (item->text() == ""){
                dt = QDateTime::currentDateTime();
            } else {
                dt = QDateTime::fromString(item->text(), DATE_FORMAT);
            }
            cDate.setDateTime(dt.toTime_t());
            obj.setAttr("Дата посл переосвид", cDate);
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
        case DateO:{
            if (item->text() == ""){
                dt = QDateTime::currentDateTime();
            } else {
                dt = QDateTime::fromString(item->text(), DATE_FORMAT);
            }
            cDate.setDateTime(dt.toTime_t());
            obj.setAttr("Дата очеред освид", cDate);
            break;
        }
        case Note:{
            obj.setAttr("Примечание", item->text());
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
            break;
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

