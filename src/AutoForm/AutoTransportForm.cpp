#include "AutoForm/AutoTransportForm.h"
#include "header_view/ProxyModelWithHeaderModels.h"
#include "AutoForm/AutoTransportModel.h"
#include "AutoForm/AutoTransportItemDelegate.h"
#include "OrderForm/orderform.h"
#include "global_defs.h"

#include <OdsInterface>
#include <QDebug>

AutoTransportForm::AutoTransportForm(int id, ODS::OdsInterface* iface, QWidget *parent) :
    QWidget(parent),
    m_model(0),
    m_headerModel(0),
    m_proxy_header(0),
    m_iface(iface),
    m_order_id(id)
{
    setupUi(this);
    init();
}
//
AutoTransportForm::~AutoTransportForm()
{
    delete m_proxy_header;
    delete m_headerModel;
    delete m_model;
}
//
void AutoTransportForm::init()
{
    HierarchicalHeaderView* hv = new HierarchicalHeaderView(Qt::Horizontal, tvAutoTransport);
    tvAutoTransport->setHorizontalHeader(hv);

    m_model = new AutoTransportModel(m_order_id, sbCurrent->value(), m_iface, tvAutoTransport);
    m_model->setColumnCount(6);

    m_headerModel = new QStandardItemModel(tvAutoTransport);
    fillHeaderModel(m_headerModel);

    m_proxy_header = new ProxyModelWithHeaderModels(tvAutoTransport);

#if QT_VERSION >= 0x050000
    m_proxy_header->setSourceModel(m_model);
#else
    m_proxy_header->setModel(m_model);
#endif

    m_proxy_header->setHorizontalHeaderModel(m_headerModel);

    tvAutoTransport->setItemDelegate(new AutoTransportItemDelegate(tvAutoTransport));
    tvAutoTransport->setModel(m_proxy_header);
    tvAutoTransport->resizeColumnsToContents();
    tvAutoTransport->verticalHeader()->setVisible(false);

#if QT_VERSION >= 0x050000
    tvAutoTransport->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tvAutoTransport->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
#else
    tvAutoTransport->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    tvAutoTransport->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#endif

    //
    connect (pbAdd, SIGNAL(clicked()),
             this, SLOT(addAutoTransport()));
    connect (pbDel, SIGNAL(clicked()),
             this, SLOT(removeAutoTransport()));
    connect (pbCancel, SIGNAL(clicked()),
             this, SLOT(cancelAutoTransport()));
    connect(sbAll, SIGNAL(valueChanged(int)),
            this, SLOT(columnCountChanged(int)));
    connect(sbCurrent, SIGNAL(valueChanged(int)),
            this, SLOT(currentColumnChanged(int)));
    connect (pbSave, SIGNAL(clicked()),
             this, SLOT(save()));
    connect(pushButton_exit, SIGNAL(clicked()),
            this, SLOT(closeForm()));
    connect(cbOrder, SIGNAL(currentIndexChanged(int)),
            this, SLOT(orderChanged(int)));

    initOrders();
}

void AutoTransportForm::closeForm()
{
    this->parentWidget()->close();
}

//
void AutoTransportForm::fillHeaderModel(QStandardItemModel* headerModel)
{
    QStandardItem* item = new QStandardItem("Должность");
    QFontMetrics fm(item->font());
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 0, item);

    item = new QStandardItem("В/звание");
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 1, item);

    item = new QStandardItem("Фамилия, инициалы");
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 2, item);

    item = new QStandardItem("Автомобиль");
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 3, item);

    QList<QStandardItem*> list;

    QStandardItem* child = new QStandardItem("Назначение");
    child->setData(QSize(fm.width(child->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    item->appendColumn(list<<child);
    list.clear();

    child = new QStandardItem("Марка");
    child->setData(QSize(fm.width(child->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    item->appendColumn(list<<child);
    list.clear();

    child = new QStandardItem("Номер");
    child->setData(QSize(fm.width(child->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    item->appendColumn(list<<child);
    list.clear();
 }
//
void AutoTransportForm::addAutoTransport()
{
    m_model->addEmptyRow();
}

void AutoTransportForm::removeAutoTransport()
{
    QModelIndex index = tvAutoTransport->currentIndex();
    if (!index.isValid())
        return;
    m_model->removeAutoTransport(index);
}
//
void AutoTransportForm::cancelAutoTransport()
{
    m_model->load();
}
//
void AutoTransportForm::save()
{
    if (!m_model->save())
        qDebug()<<"ERROR";
}
//
void AutoTransportForm::columnCountChanged(int count)
{
    sbCurrent->setMaximum(count);
    if (!m_model->removeAutocolumnMoreThan(count))
        qDebug()<<"ERROR";
}
//
void AutoTransportForm::currentColumnChanged(int num)
{
    m_model->load(num);
}

void AutoTransportForm::orderChanged(int index)
{
    int order_id = cbOrder->itemData(index).toInt();
    m_model->changeOrder(order_id);
}

void AutoTransportForm::initOrders()
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
