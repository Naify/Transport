#include "admissionstartform.h"
#include "ui_admissionstartform.h"

#include "StaffStructureManager"
#include "global_defs.h"
#include <QMessageBox>
#include <CatalogManager>

using namespace STAFF;
using namespace ODS;

AdmissionStartForm::AdmissionStartForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdmissionStartForm),
    model(0)
{
    ui->setupUi(this);
    init();
}

int AdmissionStartForm::currentVC = 0;

AdmissionStartForm::~AdmissionStartForm()
{
    delete model;
    delete ui;
}

void AdmissionStartForm::init()
{    
    model = new QStandardItemModel(ui->tv);

    getVCs();

    model->setColumnCount(4);
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Форма")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Воинское\nзвание")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Должность")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("ФИО")));
    ui->tv->setModel(model);
    ui->tv->resizeColumnsToContents();
    ui->tv->verticalHeader()->setVisible(false);
#if QT_VERSION >= 0x050000
    ui->tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tv->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
#else
    ui->tv->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tv->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#endif
    connect(ui->tv, SIGNAL(clicked(QModelIndex)), this, SLOT(approveWarning(QModelIndex)));

    connect(ui->cbVc, SIGNAL(currentIndexChanged(int)), this, SLOT(changeVc(int)));
    connect(ui->pbPersonal, SIGNAL(released()), this, SLOT(openPersonalForm()));
    connect(ui->pbSI, SIGNAL(released()), this, SLOT(openSIForm()));
    connect(ui->pbMed, SIGNAL(released()), this, SLOT(openMedForm()));
    connect(ui->pbAGroup, SIGNAL(released()), this, SLOT(openAGroupForm()));
    connect(ui->pbZDE, SIGNAL(released()), this, SLOT(openZDEForm()));
    connect(ui->pbAK, SIGNAL(released()), this, SLOT(openAKForm()));
    connect(ui->pbSopr, SIGNAL(released()), this, SLOT(openSoprForm()));
    connect(ui->pbZNK, SIGNAL(released()), this, SLOT(openZNKForm()));
    connect(ui->pbGuard, SIGNAL(released()), this, SLOT(openGuardForm()));
    connect(ui->pbKran, SIGNAL(released()), this, SLOT(openKranForm()));
    connect(ui->pbDamagedSI, SIGNAL(released()), this, SLOT(openDamagedSIForm()));
    connect(ui->pbTechLPA, SIGNAL(released()), this, SLOT(openTechLPAForm()));
    connect(ui->pb1ZDE, SIGNAL(released()), this, SLOT(open1ZDEForm()));
    connect(ui->pb2ZDE, SIGNAL(released()), this, SLOT(open2ZDEForm()));
    connect(ui->pbMech, SIGNAL(released()), this, SLOT(openMechForm()));
    connect(ui->pbPZNK, SIGNAL(released()), this, SLOT(openPZNKForm()));
    connect(ui->pbRadio, SIGNAL(released()), this, SLOT(openRadioForm()));
    connect(ui->pbRazvod, SIGNAL(released()), this, SLOT(openRazvodForm()));

    checkAdmissionWarnings("Допуска КП.Допуска к СИ");
    checkAdmissionWarnings("Допуска КП.Допуска группы А");
    checkAdmissionWarnings("Допуска КП.Допуска к аварийным СИ");
    checkAdmissionWarnings("Допуска КП.Допуска ЗНЖДЭ");
    checkAdmissionWarnings("Допуска КП.Допуска ЗНАК");
    checkAdmissionWarnings("Допуска КП.Допуска ЗНК");
    checkAdmissionWarnings("Допуска КП.Допуска караульные");
    checkAdmissionWarnings("Допуска КП.Допуска крановщики");
    checkAdmissionWarnings("Допуска КП.Допуска сопровождающие");
    checkAdmissionWarnings("Допуска КП.Допуска к ЛПА");
    checkAdmissionWarnings("Допуска КП.Допуска мед");
    checkAdmissionWarnings("Допуска КП.Допуска 1ЗНЖДЭ");
    checkAdmissionWarnings("Допуска КП.Допуска 2ЗНЖДЭ");
    checkAdmissionWarnings("Допуска КП.Допуска механики спец вагонов");
    checkAdmissionWarnings("Допуска КП.Допуска помощники начальника караула");
    checkAdmissionWarnings("Допуска КП.Допуска расчет радиостанции");
    checkAdmissionWarnings("Допуска КП.Допуска разводящие");
}

void AdmissionStartForm::approveWarning(QModelIndex index){
    QStandardItem* item = model->item(index.row(), 0);
    if (item->background().color() == Qt::yellow || item->background().color() == Qt::red){
        for (int i = 0; i != model->columnCount(); ++i){
            QStandardItem* item2 = model->item(index.row(), i);
            item2->setBackground(Qt::white);
        }
    }
}

bool AdmissionStartForm::getVCs()
{
    //установить соответствия вч из ошс и кп. проверить все ли вч есть у кп. если нет -
    //создать и назначить время изменения текущим

    StaffStructureManager staffMgr(OdsInterface::self());
    IObjectManager ioMan = OdsInterface::self()->iobjectManager();

    Division::ptr div = staffMgr.getCurrentDivision();
    if (div.isNull()){
        QMessageBox::critical(this, "Ошибка", "В ОШС не задан текущий объект");
        return false;
    }

    IObjectCursor  cur =  ioMan.getIObjects("Допуска КП.Вч для допусков");
    for (IObjectCursor::iterator it = cur.begin(); it != cur.end(); ++it) {
        ODS::IObject iobj = *it;
        Division::ptr div2 = staffMgr.getDivisionById((iobj.getIntAttr("ОШС Вч")));
        ui->cbVc->addItem(div2->name(), iobj.id());
        QDateTime datetime;
        datetime.setTime_t( iobj.getDateTimeAttr("Дата изменения").getDateTime() );
        ui->dtE->setDateTime(datetime);
    }

    IObjectQueryFilter filter;
    filter.whereCondition = "\"ОШС Вч\" = " + QString::number(div->id());

    cur =  ioMan.getIObjects("Допуска КП.Вч для допусков", filter);
    if (cur.size() == 0){
        createVC(div);
    } else {
        int index = ui->cbVc->findData(cur.toList().at(0).id());
        if (index != -1){
            ui->cbVc->setCurrentIndex(index);
            currentVC = cur.toList().at(0).id();
            QDateTime datetime;
            datetime.setTime_t(cur.toList().at(0).getDateTimeAttr("Дата изменения").getDateTime() );
            ui->dtE->setDateTime(datetime);
        }
    }

    return true;
}

bool AdmissionStartForm::createVC(Division::ptr div)
{
    IObjectManager ioMan = OdsInterface::self()->iobjectManager();
    ODS::OdsInterface::self()->dbManager().transaction();
    ODS::OdsInterface::self()->syncManager().syncTransaction();
    IObject iobj = ioMan.createIObject("Допуска КП.Вч для допусков");
    IObject linkObj = ioMan.getIObjectById(div->id());
    if (!iobj.setAttr("ОШС Вч", linkObj)){
        OdsInterface::self()->dbManager().rollback();
        QMessageBox::critical(this, "Ошибка", "Не удалось задать атрибут \"ОШС Вч\" для вч");
        return false;
    }
    CDate cDate;
    if (!cDate.setCurrentDateTime()){
        OdsInterface::self()->dbManager().rollback();
        QMessageBox::critical(this, "Ошибка", "Не получить текущую дату");
        qDebug()<<"CDate не выставил currentDateTime";
        return false;
    }
    if (!iobj.setDateTime("Дата изменения", cDate)){
        OdsInterface::self()->dbManager().rollback();
        QMessageBox::critical(this, "Ошибка", "Не удалось задать атрибут \"Дата изменения\" для вч");
        return false;
    }
     if (iobj.save(IObject::NotRecursiveSave)){
        if (setIObjRights(iobj)){
            ODS::OdsInterface::self()->dbManager().commit();
            ui->cbVc->addItem(div->name(), QVariant(iobj.id()));
            currentVC = iobj.id();
            ui->dtE->setDateTime(QDateTime::currentDateTime());
            return true;
        } else {
            OdsInterface::self()->dbManager().rollback();
            QMessageBox::critical(this, "Ошибка", "Не удалось задать права для вч");
            return false;
        }
     } else {
        OdsInterface::self()->dbManager().rollback();
        QMessageBox::critical(this, "Ошибка", "Не удалось сохранить запись вч");
        return false;
     }
}

void AdmissionStartForm::checkAdmissionWarnings(QString table)
{
    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"Вч\" = " + QString::number(currentVC);
    ODS::IObjectCursor cursor = ODS::OdsInterface::self()->iobjectManager().getIObjectCursor(table,filter);

    ODS::IObjectCursor::iterator it = cursor.begin();
    while (it != cursor.end())
    {
        ODS::IObject iobj = *it;

        QDateTime dt;
        dt.setTime_t(iobj.getDateTimeAttr("Дата окончания").getDateTime());

        if (dt.daysTo(QDateTime::currentDateTime()) >= -90 && dt.daysTo(QDateTime::currentDateTime()) < -10){
            createAdmisWarning(table, iobj.getLink("Человек"), Qt::yellow);
        } else if ((dt.daysTo(QDateTime::currentDateTime()) >= -10) ){
            createAdmisWarning(table, iobj.getLink("Человек"), Qt::red);
        }

        ++it;
    }
}

void AdmissionStartForm::createAdmisWarning(QString table, ODS::IObject iobj, Qt::GlobalColor color)
{
    QStandardItem *item = new QStandardItem();

    table.remove("Допуска КП.");
    item->setData(table,Qt::DisplayRole);
    QStandardItem *item2 = new QStandardItem();
    IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
    IRS::Catalog::ptr catalog = catalMgr.getCatalogByName("Классификатор воинских званий");
    if (iobj.getIntAttr("Воинское звание") != 0){
        IRS::CatalogEntry::ptr entry = catalog->entryById(iobj.getIntAttr("Воинское звание"));
        item2->setData(entry->stringValue("Наименование"),Qt::DisplayRole);
    }
    QStandardItem *item3 = new QStandardItem();
    catalog = catalMgr.getCatalogByName("Классификатор должностей");
        if (iobj.getIntAttr("Должность") != 0){
            IRS::CatalogEntry::ptr entry = catalog->entryById(iobj.getIntAttr("Должность"));
            item3->setData(entry->stringValue("Наименование"),Qt::DisplayRole);
        }
    QStandardItem *item4 = new QStandardItem();
    item4->setData(iobj.getStringAttr("ФИО"), Qt::DisplayRole);

    item->setBackground(color);
    item2->setBackground(color);
    item3->setBackground(color);
    item4->setBackground(color);

    QList<QStandardItem*> list;
    list <<item<< item2<<item3<<item4;
    model->appendRow(list);
}

void AdmissionStartForm::changeVc(int index)
{
    IObjectManager ioMan = OdsInterface::self()->iobjectManager();
    QDateTime datetime;
    IObject iobj = ioMan.getIObjectById(ui->cbVc->itemData(index).toInt());
    datetime.setTime_t(iobj.getDateTimeAttr("Дата изменения").getDateTime());
    ui->dtE->setDateTime(datetime);
    currentVC = ui->cbVc->itemData(ui->cbVc->currentIndex()).toInt();
}

void AdmissionStartForm::openPersonalForm()
{
    emit openPersonal();
}

void AdmissionStartForm::openSIForm()
{
    emit openSI();
}

void AdmissionStartForm::openMedForm()
{
    emit openMed();
}

void AdmissionStartForm::openAGroupForm()
{
    emit openAGroup();
}

void AdmissionStartForm::openZDEForm()
{
    emit openZDE();
}

void AdmissionStartForm::openAKForm()
{
    emit openAK();
}

void AdmissionStartForm::openSoprForm()
{
    emit openSopr();
}

void AdmissionStartForm::openZNKForm()
{
    emit openZNK();
}

void AdmissionStartForm::openGuardForm()
{
    openGuard();
}

void AdmissionStartForm::openKranForm()
{
    emit openKran();
}

void AdmissionStartForm::openDamagedSIForm()
{
    emit openDamagedSI();
}

void AdmissionStartForm::openTechLPAForm()
{
    emit openTechLPA();
}

void AdmissionStartForm::open1ZDEForm()
{
    emit open1ZDE();
}

void AdmissionStartForm::open2ZDEForm()
{
    emit open2ZDE();
}

void AdmissionStartForm::openMechForm()
{
    emit openMech();
}

void AdmissionStartForm::openPZNKForm()
{
    emit openPZNK();
}

void AdmissionStartForm::openRadioForm()
{
    emit openRadio();
}

void AdmissionStartForm::openRazvodForm()
{
    emit openRazvod();
}
