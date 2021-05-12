#include "CargoForm/CargoForm.h"
#include "header_view/ProxyModelWithHeaderModels.h"
#include "CargoForm/CargoModel.h"
#include "CargoForm/CargoItemDelegate.h"

#include <OdsInterface>
#include "global_defs.h"
#include "OrderForm/orderform.h"
#include <QDebug>

CargoForm::CargoForm(int order_id, ODS::OdsInterface *iface, QWidget *parent) :
    QWidget(parent),
    m_model(0),
    m_headerModel(0),
    m_proxy_header(0),
    m_iface(iface),
    m_order_id(order_id)
{
//    qDebug()<<"id = "<<id<<"iface"<<iface->connectionManager().isConnected();
    setupUi(this);
    init();
}
//
CargoForm::~CargoForm()
{
    delete m_proxy_header;
    delete m_headerModel;
    delete m_model;
}
//
void CargoForm::init()
{
    HierarchicalHeaderView* hv = new HierarchicalHeaderView(Qt::Horizontal, tvCargo);
    tvCargo->setHorizontalHeader(hv);

//    fillComboOrders();
//    connect(comboBox_order, SIGNAL(currentIndexChanged(int)), SLOT(changeOrder(int)) );

    m_model = new CargoModel(m_order_id, m_iface, tvCargo);
    m_model->setColumnCount(13);

    m_headerModel = new QStandardItemModel(tvCargo);
    fillHeaderModel(m_headerModel);

    m_proxy_header = new ProxyModelWithHeaderModels(tvCargo);

#if QT_VERSION >= 0x050000
    m_proxy_header->setSourceModel(m_model);
#else
    m_proxy_header->setModel(m_model);
#endif
    m_proxy_header->setHorizontalHeaderModel(m_headerModel);

    tvCargo->setItemDelegate(new CargoItemDelegate(tvCargo));
    tvCargo->setModel(m_proxy_header);
    tvCargo->resizeColumnsToContents();
    tvCargo->verticalHeader()->setVisible(false);

#if QT_VERSION >= 0x050000
    tvCargo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tvCargo->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
#else
    tvCargo->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    tvCargo->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#endif

    //
    connect (pbAdd, SIGNAL(clicked()),
             this, SLOT(addCargo()));
    connect (pbDel, SIGNAL(clicked()),
             this, SLOT(removeCargo()));
    connect (pbCancel, SIGNAL(clicked()),
             this, SLOT(cancelCargo()));
    connect (pbSave, SIGNAL(clicked()),
             this, SLOT(save()));
    connect (pushButton_exit, SIGNAL(clicked()),
             this, SLOT(closeForm()));
    connect(cbOrder, SIGNAL(currentIndexChanged(int)),
            this, SLOT(orderChanged(int)));

    initOrders();
}

void CargoForm::closeForm()
{
    this->parentWidget()->close();
}

//
void CargoForm::fillHeaderModel(QStandardItemModel* headerModel)
{
    QStandardItem* item = new QStandardItem("Наименование");
    QFontMetrics fm(item->font());
    item->setData(QSize(fm.width(item->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 0, item);

    item = new QStandardItem("Номенклат номер");
    item->setData(QSize(fm.width(item->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 1, item);

    item = new QStandardItem("Зав.№");
    item->setData(QSize(fm.width(item->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 2, item);

    item = new QStandardItem("Шифр\nтары");
    item->setData(QSize(fm.width("Шифр")+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 3, item);

    item = new QStandardItem("СГ");
    item->setData(QSize(fm.width(item->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 4, item);

    item = new QStandardItem("Блокир.");
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 5, item);

    item = new QStandardItem("Оттиски и кол-во\nпломб тары");
    item->setData(QSize(fm.width("Оттиски и кол-во")+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 6, item);

    item = new QStandardItem("Тип ТЗУ");
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 7, item);

    item = new QStandardItem("Зав.№ ТЗУ");
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 8, item);

    item = new QStandardItem("Оттиски и кол-во\nпломб ТЗУ");
    item->setData(QSize(fm.width("Оттиски и кол-во")+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 9, item);

    item = new QStandardItem("Вагон");
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 10, item);

    QList<QStandardItem*> list;

    QStandardItem* child = new QStandardItem("Тип");
    child->setData(QSize(fm.width(child->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    item->appendColumn(list<<child);
    list.clear();

    child = new QStandardItem("Номер");
    child->setData(QSize(fm.width(child->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    item->appendColumn(list<<child);
    list.clear();

    child = new QStandardItem("Пломбы");
    child->setData(QSize(fm.width(child->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    item->appendColumn(list<<child);
    list.clear();

}
//
void CargoForm::addCargo()
{
    m_model->addEmptyRow();
}
//
void CargoForm::removeCargo()
{
    QModelIndex index = tvCargo->currentIndex();
    if (!index.isValid())
        return;
    m_model->removeCargo(index);
}
//
void CargoForm::cancelCargo()
{
    m_model->load();
}
//
void CargoForm::save()
{
    if (!m_model->save())
        qDebug()<<"ERROR";
}

void CargoForm::orderChanged(int index)
{
    int order_id = cbOrder->itemData(index).toInt();
    m_model->changeOrder(order_id);
}

void CargoForm::initOrders()
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

//void CargoForm::fillComboOrders(){
//    QString querySQL = QString("SELECT DISTINCT "
//            "\"Контроль перевозок\".\"Распоряжения\".\"io_id\" as \"io_id\", "
//            "\"Контроль перевозок\".\"Распоряжения\".\"Номер\" as \"Номер\" "
//            "FROM \"Контроль перевозок\".\"Распоряжения\"");

//    ODS::IORecordCursor rec_cursor = ODS::OdsInterface::self()->iobjectManager().exec(querySQL);
//    for (ODS::IORecordCursor::iterator it = rec_cursor.begin(); it != rec_cursor.end(); ++it) {
//        ODS::IORecord iobj = *it;

//        QString orderNumb = iobj.getStringAttr("Номер");
//        int orderId = iobj.getIntAttr("io_id");

//        comboBox_order->addItem( orderNumb, QVariant(orderId)  );
//    }
//}

//void CargoForm::changeOrder(int index){
//    m_order_id = comboBox_order->itemData(index).toInt();
//}
