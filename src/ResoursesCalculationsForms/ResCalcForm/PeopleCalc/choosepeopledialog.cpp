#include "choosepeopledialog.h"
#include "ui_choosepeopledialog.h"

#include <OdsInterface>
#include "StaffStructureManager"
#include "QMessageBox"

ChoosePeopleDialog::ChoosePeopleDialog(int postId, int positionCount, int siId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChoosePeopleDialog),
    posId(postId),
    count(positionCount),
    siId(siId)
{
    ui->setupUi(this);
    init();
}

ChoosePeopleDialog::~ChoosePeopleDialog()
{
    delete ui;
}

void ChoosePeopleDialog::init()
{
//    ui->lw->setSelectionMode(QAbstractItemView::MultiSelection);

    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();
    posName = ioMan.getIObjectById(posId).getStringAttr("Наименование");

    getValidPeople();

    connect(ui->bB, SIGNAL(accepted()), this, SLOT(handleOkButton()));
    connect(ui->lw, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(lwClicked(QListWidgetItem*)));
}

void ChoosePeopleDialog::getValidPeople()
{
    if (posName == "Начальник эшелона"){
        checkAdmissionWarnings("Допуска КП.Допуска ЗНЖДЭ");
    } else if (posName == "1-й ЗНЭШ"){
        checkAdmissionWarnings("Допуска КП.Допуска 1ЗНЖДЭ");
    } else if (posName == "2-й ЗНЭШ"){
        checkAdmissionWarnings("Допуска КП.Допуска 2ЗНЖДЭ");
    } else if (posName == "Сопровождающий"){
        checkAdmissionWarnings("Допуска КП.Допуска сопровождающие");
    } else if (posName == "Помощник нач. караула"){
        checkAdmissionWarnings("Допуска КП.Допуска помощники начальника караула");
    } else if (posName == "Начальник караула"){
        checkAdmissionWarnings("Допуска КП.Допуска ЗНК");
    } else if (posName == "Разводящий"){
        checkAdmissionWarnings("Допуска КП.Допуска разводящие");
    } else if (posName == "Расчет радиостанции"){
        checkAdmissionWarnings("Допуска КП.Допуска расчет радиостанции");
    } else if (posName == "Караульные"){
        checkAdmissionWarnings("Допуска КП.Допуска караульные");
    } else if (posName == "Механик спец вагона"){
        checkAdmissionWarnings("Допуска КП.Допуска механики спец вагонов");
    }
}

void ChoosePeopleDialog::checkAdmissionWarnings(QString table)
{
    STAFF::StaffStructureManager staffMgr(ODS::OdsInterface::self());
    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();

    STAFF::Division::ptr div = staffMgr.getCurrentDivision();
    if (div.isNull()){
        QMessageBox::critical(this, "Ошибка", "В ОШС не задан текущий объект");
        return;
    }

    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"ОШС Вч\" = " + QString::number(div->id());
    int currentVC = 0;
    ODS::IObjectCursor cur =  ioMan.getIObjects("Допуска КП.Вч для допусков", filter);
    if (cur.size() == 0){
        QMessageBox::critical(this, "Ошибка", "Не создана вч в системе допусков");
    } else {
        currentVC = cur.toList().at(0).id();
    }

    filter.whereCondition = "\"Вч\" = " + QString::number(currentVC);
    ODS::IObjectCursor cursor = ODS::OdsInterface::self()->iobjectManager().getIObjectCursor(table,filter);


    for (ODS::IObjectCursor::iterator it = cursor.begin(); it != cursor.end(); ++it){
        ODS::IObject iobj = *it;

        QDateTime dt;
        dt.setTime_t(iobj.getDateTimeAttr("Дата окончания").getDateTime());

        ODS::IObject personIobj = iobj.getLink("Человек");

        if (dt.daysTo(QDateTime::currentDateTime()) < 0 && checkPerson(currentVC, personIobj.id())){

            QString fio = personIobj.getStringAttr("ФИО");
            QListWidgetItem* item = new QListWidgetItem(fio,ui->lw);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setCheckState(Qt::Unchecked);
            item->setData(6,personIobj.id());
        }
    }
}

bool ChoosePeopleDialog::checkPerson(int vcId, int personId)
{
    bool agroup = false, med = false, si = false;

    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"Вч\" = " + QString::number(vcId) + " AND \"Человек\" = " + QString::number(personId) +
            " AND \"Результат аттестации\" = 'TRUE'";
    ODS::IObjectCursor cursor = ODS::OdsInterface::self()->iobjectManager().getIObjectCursor("Допуска КП.Допуска группы А",filter);

    for (ODS::IObjectCursor::iterator it = cursor.begin(); it != cursor.end(); ++it){
        ODS::IObject iobj = *it;

        QDateTime dt;
        dt.setTime_t(iobj.getDateTimeAttr("Дата окончания").getDateTime());
        if (dt.daysTo(QDateTime::currentDateTime()) < 0){
            agroup = true;
        }
    }
    \
    filter.whereCondition = "\"Вч\" = " + QString::number(vcId) + " AND \"Человек\" = " + QString::number(personId)+
                " AND \"Заключение\" = 'TRUE'";
    cursor = ODS::OdsInterface::self()->iobjectManager().getIObjectCursor("Допуска КП.Допуска мед",filter);

    for (ODS::IObjectCursor::iterator it = cursor.begin(); it != cursor.end(); ++it){
        ODS::IObject iobj = *it;

        QDateTime dt;
        dt.setTime_t(iobj.getDateTimeAttr("Дата окончания").getDateTime());
        if (dt.daysTo(QDateTime::currentDateTime()) < 0){
            med = true;
        }
    }

    filter.whereCondition = "\"Вч\" = " + QString::number(vcId) + " AND \"Человек\" = " + QString::number(personId) + " AND \"СИ\" = " + QString::number(siId);
    cursor = ODS::OdsInterface::self()->iobjectManager().getIObjectCursor("Допуска КП.Допуска к СИ",filter);

    for (ODS::IObjectCursor::iterator it = cursor.begin(); it != cursor.end(); ++it){
        ODS::IObject iobj = *it;

        QDateTime dt;
        dt.setTime_t(iobj.getDateTimeAttr("Дата окончания").getDateTime());
        if (dt.daysTo(QDateTime::currentDateTime()) < 0){
            si = true ;
        }
    }

    if (agroup && med && si) {
        return true;
    }
    return false;
}

void ChoosePeopleDialog::lwClicked(QListWidgetItem *item)
{
    int personId = item->data(6).toInt();
    if (item->checkState() == Qt::Checked){
        item->setCheckState(Qt::Unchecked);
        idList.remove(personId);
    } else {
        item->setCheckState(Qt::Checked);
        idList.insert(personId, posId);
    }
}

void ChoosePeopleDialog::handleOkButton()
{
    sendIds(idList);
}
