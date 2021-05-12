#include "TrainForm/TransportForm.h"
#include "header_view/ProxyModelWithHeaderModels.h"
#include "TrainForm/TransportModel.h"
#include "TrainForm/TransportItemDelegate.h"

#include <OdsInterface>
#include "global_defs.h"
#include "OrderForm/orderform.h"
#include <QDebug>

TransportForm::TransportForm(int id, ODS::OdsInterface* iface, QWidget *parent) :
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
TransportForm::~TransportForm()
{
    delete m_proxy_header;
    delete m_headerModel;
    delete m_model;
}
//
void TransportForm::init()
{
    HierarchicalHeaderView* hv = new HierarchicalHeaderView(Qt::Horizontal, tvTransport);
    tvTransport->setHorizontalHeader(hv);

    m_model = new TransportModel(m_order_id, m_iface, tvTransport);
    m_model->setColumnCount(5);

    m_headerModel = new QStandardItemModel(tvTransport);
    fillHeaderModel(m_headerModel);

    m_proxy_header = new ProxyModelWithHeaderModels(tvTransport);

#if QT_VERSION >= 0x050000
    m_proxy_header->setSourceModel(m_model);
#else
    m_proxy_header->setModel(m_model);
#endif
    m_proxy_header->setHorizontalHeaderModel(m_headerModel);

    tvTransport->setItemDelegate(new TransportItemDelegate(tvTransport));
    tvTransport->setModel(m_proxy_header);
    tvTransport->resizeColumnsToContents();
    tvTransport->verticalHeader()->setVisible(false);
#if QT_VERSION >= 0x050000
    tvTransport->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tvTransport->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
#else
    tvTransport->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    tvTransport->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#endif
    //
    connect (pbAdd, SIGNAL(clicked()),
             this, SLOT(addTransport()));
    connect (pbDel, SIGNAL(clicked()),
             this, SLOT(removeTransport()));
    connect (pbCancel, SIGNAL(clicked()),
             this, SLOT(cancelTransport()));
    connect (pbSave, SIGNAL(clicked()),
             this, SLOT(save()));
    connect (pushButton_exit, SIGNAL(clicked()),
             this, SLOT(closeForm()));
    connect(cbOrder, SIGNAL(currentIndexChanged(int)),
            this, SLOT(orderChanged(int)));

    initOrders();
}

void TransportForm::closeForm()
{
    this->parentWidget()->close();
}

//
void TransportForm::fillHeaderModel(QStandardItemModel* headerModel)
{
    QStandardItem* item = new QStandardItem("Тип вагона");
    QFontMetrics fm(item->font());
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 0, item);

    item = new QStandardItem("Назначение");
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 1, item);

    item = new QStandardItem("Заводской №");
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 2, item);

    item = new QStandardItem("Сцепка");
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 3, item);

    QList<QStandardItem*> list;

    QStandardItem* child = new QStandardItem("Фланг");
    child->setData(QSize(fm.width(child->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    item->appendColumn(list<<child);
    list.clear();

    child = new QStandardItem("Номер\nво фланге");
    child->setData(QSize(fm.width("во фланге")+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    item->appendColumn(list<<child);
    list.clear();
 }
//
void TransportForm::addTransport()
{
    m_model->addEmptyRow();
}
//
void TransportForm::removeTransport()
{
    QModelIndex index = tvTransport->currentIndex();
    if (!index.isValid())
        return;
   // qDebug()<<Q_FUNC_INFO<<index<<index.row();
    m_model->removeTransport(index);
}
//
void TransportForm::cancelTransport()
{
    m_model->load();
}
//
void TransportForm::save()
{
    if (!m_model->save())
        qDebug()<<"ERROR";
}

void TransportForm::orderChanged(int index)
{
    int order_id = cbOrder->itemData(index).toInt();
    m_model->changeOrder(order_id);
}

void TransportForm::initOrders()
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
