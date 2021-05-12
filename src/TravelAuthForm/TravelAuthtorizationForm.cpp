#include "TravelAuthForm/TravelAuthtorizationForm.h"
#include "TravelAuthForm/TravelAuthtorizationModel.h"

#include <OdsInterface>
#include "global_defs.h"
#include "OrderForm/orderform.h"
#include <QDebug>
#include <QCompleter>

TravelAuthtorizationForm::TravelAuthtorizationForm(int id, ODS::OdsInterface* iface, QWidget *parent) :
    QWidget(parent),
    m_model(0),
    m_iface(iface),
    m_order_id(id),
    m_id(0)
{
    setupUi(this);
    init();
//    initCompleter();
}
//
TravelAuthtorizationForm::~TravelAuthtorizationForm()
{
    delete m_model;
}
//
void TravelAuthtorizationForm::init()
{
    m_model = new TravelAuthtorizationModel(m_order_id, m_iface, tvTravelAuthtorization);
    m_model->setColumnCount(6);

    tvTravelAuthtorization->setModel(m_model);
    tvTravelAuthtorization->resizeColumnsToContents();
    tvTravelAuthtorization->verticalHeader()->setVisible(false);
#if QT_VERSION >= 0x050000
    tvTravelAuthtorization->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tvTravelAuthtorization->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
#else
    tvTravelAuthtorization->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    tvTravelAuthtorization->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#endif
    //

    deOrder->setDate(QDate::currentDate());
    deBegin->setDate(QDate::currentDate());
    deEnd->setDate(QDate::currentDate());

    if (!m_iface)
        return;

    ODS::IObjectCursor cursor = m_iface->iobjectManager().getIObjectCursor("Документы КП.Командировочное удостоверение",
                                                                           QString("\"Распоряжение\"='%1'").arg(m_order_id));

    if (cursor.size())
    {
        ODS::IObject obj = *cursor.begin();
        m_id = obj.id();

        CDate date = obj.getDateTimeAttr("Дата приказа");
        deOrder->setDate(QDateTime::fromTime_t(date.getDateTime()).date());
        date = obj.getDateTimeAttr("Дата начала");
        deBegin->setDate(QDateTime::fromTime_t(date.getDateTime()).date());
        date = obj.getDateTimeAttr("Дата окончания");
        deEnd->setDate(QDateTime::fromTime_t(date.getDateTime()).date());

        leOrder->setText(obj.getStringAttr("Номер приказа"));
        leDestination->setText(obj.getStringAttr("Пункты назначения"));
        leSign1->setText(obj.getStringAttr("Подпись1"));
        leSign2->setText(obj.getStringAttr("Подпись2"));
    }

    connect(pushButton_exit, SIGNAL(clicked()),
             this, SLOT(closeForm()));
    connect(cbOrder, SIGNAL(currentIndexChanged(int)),
            this, SLOT(orderChanged(int)));
    connect (pbSave, SIGNAL(clicked()),
             this, SLOT(save()));

    initOrders();
}

void TravelAuthtorizationForm::closeForm()
{
    this->parentWidget()->close();
}
//
/*
void TravelAuthtorizationForm::initCompleter()
{
    QStringList wordList;
    wordList << "в/ч12345" << "67890" << "СНПО Молибден";

    QCompleter *completer = new QCompleter(wordList, leDestination);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    leDestination->setCompleter(completer);
}
*/
//
void TravelAuthtorizationForm::save()
{
    ODS::OdsInterface::self()->dbManager().transaction();
    ODS::OdsInterface::self()->syncManager().syncTransaction();

    ODS::IObject obj;

    if (m_id > 0)
        obj = m_iface->iobjectManager().getIObjectById(m_id);
    else
        obj = m_iface->iobjectManager().createIObject("Документы КП.Командировочное удостоверение");

    obj.setAttr("Распоряжение", m_order_id);
    CDate date;
    date.setDateTime(deOrder->dateTime().toTime_t());
    obj.setAttr("Дата приказа", date);
    date.setDateTime(deBegin->dateTime().toTime_t());
    obj.setAttr("Дата начала", date);
    date.setDateTime(deEnd->dateTime().toTime_t());
    obj.setAttr("Дата окончания", date);
    obj.setAttr("Номер приказа", leOrder->text());
    obj.setAttr("Пункты назначения", leDestination->text());
    obj.setAttr("Подпись1", leSign1->text());
    obj.setAttr("Подпись2", leSign2->text());


    if ( !((m_id > 0)?
           m_iface->iobjectManager().updateIObject(obj, ODS::IObject::NotRecursiveUpdate) :
           m_iface->iobjectManager().saveIObject(obj, ODS::IObject::NotRecursiveSave)))
    {
        emit(msgToStatus("Не удалось сохранить командировочное удостоверение"));
        qDebug()<<"Не удалось сохранить командировочное удостоверение";
    } else {
        if (setIObjRights(obj)) {
            ODS::OdsInterface::self()->dbManager().commit();
            emit(msgToStatus("Данные сохранены"));
        } else {
            ODS::OdsInterface::self()->dbManager().rollback();
        }
    }
}

void TravelAuthtorizationForm::orderChanged(int index)
{
    int order_id = cbOrder->itemData(index).toInt();
    m_model->changeOrder(order_id);
    m_order_id = order_id;

    deOrder->setDate(QDate::currentDate());
    deBegin->setDate(QDate::currentDate());
    deEnd->setDate(QDate::currentDate());
    leOrder->clear();
    leDestination->clear();
    leSign1->clear();
    leSign2->clear();

    ODS::IObjectCursor cursor = m_iface->iobjectManager().getIObjectCursor("Документы КП.Командировочное удостоверение",
                                                                               QString("\"Распоряжение\"='%1'").arg(order_id));
    if (cursor.size())
    {
        ODS::IObject obj = *cursor.begin();
        m_id = obj.id();

        CDate date = obj.getDateTimeAttr("Дата приказа");
        deOrder->setDate(QDateTime::fromTime_t(date.getDateTime()).date());
        date = obj.getDateTimeAttr("Дата начала");
        deBegin->setDate(QDateTime::fromTime_t(date.getDateTime()).date());
        date = obj.getDateTimeAttr("Дата окончания");
        deEnd->setDate(QDateTime::fromTime_t(date.getDateTime()).date());

        leOrder->setText(obj.getStringAttr("Номер приказа"));
        leDestination->setText(obj.getStringAttr("Пункты назначения"));
        leSign1->setText(obj.getStringAttr("Подпись1"));
        leSign2->setText(obj.getStringAttr("Подпись2"));
    }
}

void TravelAuthtorizationForm::initOrders()
{
    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();

    ODS::IObjectCursor cursor =  ioMan.getIObjects(JOURNAL_TYPE);

    cbOrder->clear();

    for (ODS::IObjectCursor::iterator iobjIt = cursor.begin(); iobjIt != cursor.end(); ++iobjIt ){
        ODS::IObject iobj = (*iobjIt);

        cbOrder->addItem(iobj.getStringAttr(NUMBER_ATTR), QVariant(iobj.id()));
    }

    int order_id = OrderForm::get_current_order_id();
    if ( order_id == 0) {
        cbOrder->setCurrentIndex(0);
        orderChanged(0);
    } else {
        int order_index = cbOrder->findData(order_id);
        cbOrder->setCurrentIndex(order_index);
        orderChanged(order_index);
    }

}
