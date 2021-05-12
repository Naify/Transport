#include "ShipmentForm/ShipmentForm.h"
#include "header_view/ProxyModelWithHeaderModels.h"
#include "ShipmentForm/ShipmentModel.h"
#include "ShipmentForm/ShipmentItemDelegate.h"

#include <OdsInterface>
#include "global_defs.h"
#include "OrderForm/orderform.h"
#include <QDebug>
#include <QStandardItem>

ShipmentForm::ShipmentForm(int id, ODS::OdsInterface* iface, QWidget *parent) :
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
ShipmentForm::~ShipmentForm()
{
    delete m_proxy_header;
    delete m_headerModel;
    delete m_model;
}
//
void ShipmentForm::init()
{
    HierarchicalHeaderView* hv = new HierarchicalHeaderView(Qt::Horizontal, tvShipment);
    tvShipment->setHorizontalHeader(hv);

    m_model = new ShipmentModel(m_order_id, m_iface, tvShipment);
    m_model->setColumnCount(12);

    m_headerModel = new QStandardItemModel(tvShipment);
    fillHeaderModel(m_headerModel);

    m_proxy_header = new ProxyModelWithHeaderModels(tvShipment);

#if QT_VERSION >= 0x050000
    m_proxy_header->setSourceModel(m_model);
#else
    m_proxy_header->setModel(m_model);
#endif
    m_proxy_header->setHorizontalHeaderModel(m_headerModel);

    tvShipment->setItemDelegate(new ShipmentItemDelegate(tvShipment));
    tvShipment->setModel(m_proxy_header);
    tvShipment->resizeColumnsToContents();
    tvShipment->verticalHeader()->setVisible(false);
#if QT_VERSION >= 0x050000
    tvShipment->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tvShipment->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
#else
    tvShipment->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    tvShipment->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#endif
    //
//    connect (pbAdd, SIGNAL(clicked()),
//             this, SLOT(addShipment()));
//    connect (pbDel, SIGNAL(clicked()),
//             this, SLOT(removeShipment()));
    connect (pbCancel, SIGNAL(clicked()),
             this, SLOT(cancelShipment()));
    connect (pbSave, SIGNAL(clicked()),
             this, SLOT(save()));
    connect (pushButton_exit, SIGNAL(clicked()),
             this, SLOT(closeForm()));
    connect(cbOrder, SIGNAL(currentIndexChanged(int)),
            this, SLOT(orderChanged(int)));

    initOrders();
}

void ShipmentForm::closeForm()
{
    this->parentWidget()->close();
}

//
void ShipmentForm::fillHeaderModel(QStandardItemModel* headerModel)
{
    QStandardItem* item = new QStandardItem("Номенклатура");
    QFontMetrics fm(item->font());
    item->setData(QSize(fm.width("(номенклатура)")+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 0, item);

    item = new QStandardItem("Зав.№");
    item->setData(QSize(fm.width(item->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 1, item);

    item = new QStandardItem("СГ");
    item->setData(QSize(fm.width(item->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 2, item);

    item = new QStandardItem("Блокир.");
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 3, item);

    item = new QStandardItem("Оттиски и кол-во\nпломб тары");
    item->setData(QSize(fm.width("Оттиски и кол-во")+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 4, item);

    item = new QStandardItem("ТЗУ");
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 5, item);

    QList<QStandardItem*> list;

    QStandardItem* child = new QStandardItem("Тип");
    child->setData(QSize(fm.width(child->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    item->appendColumn(list<<child);
    list.clear();

    child = new QStandardItem("Зав. №");
    child->setData(QSize(fm.width(child->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    item->appendColumn(list<<child);
    list.clear();

    item = new QStandardItem("Оттиски и кол-во\nпломб ТЗУ");
    item->setData(QSize(fm.width("Оттиски и кол-во")+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 6, item);

    item = new QStandardItem("Выдача");
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 7, item);

    child = new QStandardItem("Бригада");
    child->setData(QSize(fm.width(child->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    item->appendColumn(list<<child);
    list.clear();

    child = new QStandardItem("Сооруж.");
    child->setData(QSize(fm.width(child->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    item->appendColumn(list<<child);
    list.clear();

    child = new QStandardItem("Автоколонна");
    child->setData(QSize(fm.width(child->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    item->appendColumn(list<<child);
    list.clear();

    child = new QStandardItem("Рейс");
    child->setData(QSize(fm.width(child->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    item->appendColumn(list<<child);
    list.clear();
}
//
void ShipmentForm::addShipment()
{
    m_model->addEmptyRow();
}
//
//void ShipmentForm::removeShipment()
//{
//    QModelIndex index = tvShipment->currentIndex();
//    if (!index.isValid())
//        return;
//    m_model->removeShipment(index);
//}
//
void ShipmentForm::cancelShipment()
{
    m_model->load();
}
//
void ShipmentForm::save()
{
    if (!m_model->save())
        qDebug()<<"ERROR";
}

void ShipmentForm::orderChanged(int index)
{
    int order_id = cbOrder->itemData(index).toInt();
    m_model->changeOrder(order_id);
}

void ShipmentForm::initOrders()
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
