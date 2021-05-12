#include "showescortdialogmodel.h"
#include "OdsInterface"
#include <CatalogManager>
#include <QMessageBox>
#include "global_defs.h"

ShowEscortDialogModel::ShowEscortDialogModel(QObject *parent):
    QStandardItemModel(parent),
    iface(ODS::OdsInterface::self())
{
    init();
}

ShowEscortDialogModel::~ShowEscortDialogModel()
{
}

void ShowEscortDialogModel::init()
{
//    load();
}

void ShowEscortDialogModel::load(QMap<int,int>& newpeopleList, int trId)
{
    removeRows(0, rowCount());

    newPeopleList = newpeopleList;
    curTrId = trId;

    ODS::IObjectManager ioMan = iface->iobjectManager();
    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"Перевозка\" ="+QString::number(trId);
    ODS::IObjectCursor  cur =  ioMan.getIObjects("Определение сил и средств.Команда сопровождения", filter);

    loadPos("Начальник эшелона", cur);
    loadPos("1-й ЗНЭШ", cur);
    loadPos("2-й ЗНЭШ", cur);
    loadPos("Сопровождающий", cur);
    loadPos("Механик спец вагона", cur);
    loadPos("Начальник караула", cur);
    loadPos("Помощник нач. караула", cur);
    loadPos("Разводящий", cur);
    loadPos("Караульные", cur);
    loadPos("Расчет радиостанции", cur);
}

void ShowEscortDialogModel::loadPos(const QString& posName, ODS::IObjectCursor& cur )
{
    ODS::IObjectManager ioMan = iface->iobjectManager();
    for (ODS::IObjectCursor::iterator it = cur.begin(); it != cur.end(); ++it){
        ODS::IObject iobj = *it;
        if (iobj.getLink("Должность").getStringAttr("Наименование") == posName){
            makeRow(iobj.getIntAttr("Человек"), iobj.getIntAttr("Должность"), posName, curTrId, iobj.id() );
        }
    }

    QMap<int,int>::iterator it = newPeopleList.begin();
    while (it != newPeopleList.end()) {
        int i = it.key(), j = it.value();
        QString curPosName = ioMan.getIObjectById(j).getStringAttr("Наименование");
        if (curPosName == posName) {
            makeRow(i, j, curPosName, curTrId);
        }
        ++it;
    }
}

void ShowEscortDialogModel::makeRow(int personId, int posId, const QString& post, int trId, int io_id)
{
    ODS::IObjectManager ioMan = iface->iobjectManager();

    ODS::IObject personIObj = ioMan.getIObjectById(personId);
    QString rank = personIObj.getLink("Воинское звание").getStringAttr("Наименование");
    QString fio = personIObj.getStringAttr("ФИО");

    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"Перевозка\" = "+QString::number(trId)+" AND "+"\"Человек\" = "+QString::number(personId);
    ODS::IObjectCursor cur =  ioMan.getIObjects("Определение сил и средств.Команда сопровождения", filter);

    QString weapName = "", weapNum = "", trName = "", trNum = "";
    if (cur.size() != 0){
        weapName = cur.toList().at(0).getStringAttr("Вооружение наименование");
        weapNum = cur.toList().at(0).getStringAttr("Вооружение номер");
        trName = cur.toList().at(0).getStringAttr("Вагон/авто обозначение");
        trNum = cur.toList().at(0).getStringAttr("Вагон/авто номер");
    }

    QList<QStandardItem*> row;
    QStandardItem* item = new QStandardItem;
    item->setData(Post, TypeRole);
    item->setText(post);
    item->setData(io_id, IdRole);
    item->setData(personId, PersonIdRole);
    if (io_id == 0) {
        item->setData(true, ChangedRole);
    } else {
        item->setData(false, ChangedRole);
    }
    item->setData(trId, TrIdRole);
    item->setData(posId, PostIdRole);
    row<<item;
    item = new QStandardItem;
    item->setText(rank);
    item->setData(Rank, TypeRole);
    row<<item;
    item = new QStandardItem;
    item->setText(fio);
    item->setData(Fio, TypeRole);
    row<<item;
    item = new QStandardItem;
    item->setText(weapName);
    item->setData(WeaponName, TypeRole);
    row<<item;
    item = new QStandardItem;
    item->setText(weapNum);
    item->setData(WeaponNumber, TypeRole);
    row<<item;
    item = new QStandardItem;
    item->setText(trName);
    item->setData(TransportName, TypeRole);
    row<<item;
    item = new QStandardItem;
    item->setText(trNum);
    item->setData(TransportNumber, TypeRole);
    row<<item;

    appendRow(row);
}

void ShowEscortDialogModel::remove(const QModelIndex &index)
{
    int id = index.sibling(index.row(), 0).data(IdRole).toInt();
    if (id > 0 && !idToDelList.contains(id))
        idToDelList<<id;

    removeRow(index.row());
}

bool ShowEscortDialogModel::save()
{
    ODS::OdsInterface::self()->dbManager().transaction();
    ODS::OdsInterface::self()->syncManager().syncTransaction();

    foreach(int id, idToDelList){
        if (!ODS::OdsInterface::self()->iobjectManager().deleteIObjectById(id)){
            ODS::OdsInterface::self()->dbManager().rollback();
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
            obj = ODS::OdsInterface::self()->iobjectManager().createIObject("Определение сил и средств.Команда сопровождения");
        if (!saveItem(item, obj)){
            ODS::OdsInterface::self()->dbManager().rollback();
            return false;
        }
    }

    ODS::OdsInterface::self()->dbManager().commit();
    return true;
}

bool ShowEscortDialogModel::saveItem(QStandardItem *item, ODS::IObject &obj)
{
    QStandardItem* idItem = this->item(item->row(), 0);
    bool isUpdate = item->data(IdRole).toInt() > 0;

    int column = 0;
    obj.setAttr("Перевозка", item->data(TrIdRole).toInt());
    obj.setAttr("Человек", item->data(PersonIdRole).toInt());
    obj.setAttr("Должность", item->data(PostIdRole).toInt());
    while(item)
    {
        switch (item->data(TypeRole).toInt())
        {
        case Post:
        case Rank:
        case Fio:
            break;
        case WeaponName:
            obj.setAttr("Вооружение наименование", item->text());
            break;
        case WeaponNumber:{
            obj.setAttr("Вооружение номер", item->text());
            break;
        }
        case TransportName:{
            obj.setAttr("Вагон/авто обозначение", item->text());
            break;
        }
        case TransportNumber:{
            obj.setAttr("Вагон/авто номер", item->text());
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
