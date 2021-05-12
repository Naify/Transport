#include "rescalcstartform.h"
#include "ui_rescalcstartform.h"

#include "OdsInterface"
#include "QMessageBox"
#include "global_defs.h"
#include "mainwindow.h"

ResCalcStartForm::ResCalcStartForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResCalcStartForm)
{
    ui->setupUi(this);
    init();
}

int ResCalcStartForm::currentTransitId = 0;

ResCalcStartForm::~ResCalcStartForm()
{
    delete ui;
}

void ResCalcStartForm::init()
{
    connect(ui->cbOrder, SIGNAL(currentIndexChanged(int)), this, SLOT(changeOrder(int)));
    connect(ui->cbOrderCargo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeOrderCargo(int)));
    connect(ui->pbViewOrder, SIGNAL(released()), this, SLOT(openOrderForm()));
    connect(ui->pbRails, SIGNAL(released()), this, SLOT(openRailsForm()));
    connect(ui->pbCars, SIGNAL(released()), this, SLOT(openCarsForm()));
    connect(ui->pbTzuSk, SIGNAL(released()), this, SLOT(openTzuSkForm()));
    connect(ui->pbEngTech, SIGNAL(released()), this, SLOT(openEngTechForm()));
    connect(ui->pbSupportMachines, SIGNAL(released()), this, SLOT(openSupportMachinesForm()));
    connect(ui->pbCalcRails, SIGNAL(released()), this, SLOT(openCalcRailsForm()));
    connect(ui->pbCalcPeople, SIGNAL(released()), this, SLOT(openCalcPeopleForm()));
    connect(ui->pbCalcWeapons, SIGNAL(released()), this, SLOT(openCalcWeaponsForm()));
    connect(ui->pbCreateTransit, SIGNAL(released()), this, SLOT(saveTransit()));

    QStringList strList;
    strList<<"КП-1"<<"КП-2"<<"КП-3"<<"КП-4";
    ui->cbCategory->addItems(strList);

    getOrders();
}

bool ResCalcStartForm::getOrders()
{
    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();
    ODS::IObjectCursor cursor =  ioMan.getIObjects(JOURNAL_TYPE);

    if (!cursor.size()){
        QMessageBox::warning(0, "", "Не удалось получить номера распоряжений");
        return false;
    }

    for (ODS::IObjectCursor::iterator iobjIt = cursor.begin(); iobjIt != cursor.end(); ++iobjIt ){
        ODS::IObject iobj = (*iobjIt);

        ui->cbOrder->addItem(iobj.getStringAttr(NUMBER_ATTR), iobj.id());
    }

    return true;
}

void ResCalcStartForm::changeOrder(int index)
{
    disconnect(ui->cbOrderCargo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeOrderCargo(int)));
    ui->cbOrderCargo->clear();
    ui->leSiCount->clear();
    ui->leSiType->clear();
    ui->leTransportType->clear();

    ODS::IORecordCursor cur = ODS::OdsInterface::self()->iobjectManager().exec(
             "SELECT max(\"Контроль перевозок\".\"Содержание таблица\".\"№№ п/п\")"
             " as max_num FROM \"Контроль перевозок\".\"Содержание таблица\" "
             "WHERE \"Контроль перевозок\".\"Содержание таблица\".\"Распоряжение\" = "+ui->cbOrder->itemData(index).toString());

    int maxNum = 0;
    for (ODS::IORecordCursor::iterator it = cur.begin(); it != cur.end(); ++it) {
        ODS::IORecord cur = *it;
        maxNum = cur.getIntAttr("max_num");
    }

    connect(ui->cbOrderCargo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeOrderCargo(int)));

    for (int i = maxNum; i != 0; --i) {
        ui->cbOrderCargo->addItem(QString::number(i));
    }
}

void ResCalcStartForm::changeOrderCargo(int index)
{
    currentTransitId = 0;

    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();
    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"№№ п/п\" = "+ui->cbOrderCargo->itemText(index)+ " AND "+
            "\"Распоряжение\" = " + ui->cbOrder->itemData(ui->cbOrder->currentIndex()).toString();
    ODS::IObjectCursor cur = ioMan.getIObjectCursor("Контроль перевозок.Содержание таблица",filter);

    for (ODS::IObjectCursor::iterator It = cur.begin(); It != cur.end(); ++It){
        ODS::IObject iobj = (*It);

        ui->leSiType->setText(iobj.getLink("Наименование груза").getStringAttr("Наименование"));
        ui->leSiCount->setText(iobj.getStringAttr("Кол-во"));
        ODS::IObject orderIobj = ioMan.getIObjectById(ui->cbOrder->itemData(ui->cbOrder->currentIndex()).toInt());
        QString trType = orderIobj.getLink("Содержание").getStringAttr("Вид транспорта");
        ui->leTransportType->setText(trType);
        ui->cbOrderCargo->setItemData(index, iobj.id());
        int trId = checkTransitExist(iobj.id());
        currentTransitId = trId;

    }
}

void ResCalcStartForm::saveTransit()
{
    //check fields & save
    if (ui->leSiType->text() == "" || ui->leSiCount->text() == "" || ui->leTransportType->text() == ""){
        QMessageBox::warning(0, "Внимание", "Выберите номер содержания");
    } else {

        int orderCargoId = ui->cbOrderCargo->itemData(ui->cbOrderCargo->currentIndex()).toInt();

        if (checkTransitExist(orderCargoId) != 0){
            QMessageBox::information(0, "", "Запись уже создана");
            return;
        } else {
            ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();

            ODS::OdsInterface::self()->dbManager().transaction();
            ODS::OdsInterface::self()->syncManager().syncTransaction();
            ODS::IObject iobj = ioMan.createIObject("Определение сил и средств.Перевозка");
            iobj.setAttr("Вид транспорта", ui->leTransportType->text());
            iobj.setAttr("Категория", ui->cbCategory->itemText(ui->cbCategory->currentIndex()));
            iobj.setAttr("Содержание распоряжения", orderCargoId);

            if (!iobj.save(ODS::IObject::NotRecursiveSave)){
                ODS::OdsInterface::self()->dbManager().rollback();
                QMessageBox::critical(0, "Ошибка", "Неудачная поппытка созранения записи");
                qDebug()<<"Ошибка при сохраниении данных в бд";
                return;
            } else {
                if (setIObjRights(iobj)) {
                    ODS::OdsInterface::self()->dbManager().commit();
                    currentTransitId = iobj.id();
                    msgToStatus("Перевозка создана");
                }
            }
        }
    }
}

int ResCalcStartForm::checkTransitExist(int orderCargoId)
{    
    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();
    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"Содержание распоряжения\" = "+QString::number(orderCargoId);
    ODS::IObjectCursor cur = ioMan.getIObjectCursor("Определение сил и средств.Перевозка",filter);

    if (cur.size() == 0)
        return 0;
    else {
        for (ODS::IObjectCursor::iterator It = cur.begin(); It != cur.end(); ++It){
            ODS::IObject iobj = (*It);
            return iobj.id();
        }
    }
    return 0;
}
