#include "EscortForm/EscortForm.h"
#include "header_view/ProxyModelWithHeaderModels.h"
#include "EscortForm/EscortModel.h"
#include "EscortForm/EscortItemDelegate.h"
#include <OdsInterface>
#include "global_defs.h"
#include "OrderForm/orderform.h"
#include <QDebug>

EscortForm::EscortForm(int id, ODS::OdsInterface* iface, QWidget *parent) :
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
EscortForm::~EscortForm()
{
    delete m_proxy_header;
    delete m_headerModel;
    delete m_model;
}
//
void EscortForm::init()
{
    HierarchicalHeaderView* hv = new HierarchicalHeaderView(Qt::Horizontal, tvEscort);
    tvEscort->setHorizontalHeader(hv);

    m_model = new EscortModel(m_order_id, m_iface, tvEscort);
    m_model->setColumnCount(11);

    m_headerModel = new QStandardItemModel(tvEscort);
    fillHeaderModel(m_headerModel);

    m_proxy_header = new ProxyModelWithHeaderModels(tvEscort);

#if QT_VERSION >= 0x050000
    m_proxy_header->setSourceModel(m_model);
#else
    m_proxy_header->setModel(m_model);
#endif
    m_proxy_header->setHorizontalHeaderModel(m_headerModel);

    tvEscort->setItemDelegate(new EscortItemDelegate(tvEscort));
    tvEscort->setModel(m_proxy_header);
    tvEscort->resizeColumnsToContents();
    tvEscort->verticalHeader()->setVisible(false);
#if QT_VERSION >= 0x050000
    tvEscort->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tvEscort->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
#else
    tvEscort->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    tvEscort->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#endif

    //
    connect (pbAdd, SIGNAL(clicked()),
             this, SLOT(addEscort()));
    connect (pbDel, SIGNAL(clicked()),
             this, SLOT(removeEscort()));
    connect (pbCancel, SIGNAL(clicked()),
             this, SLOT(cancelEscort()));
    connect (pbSave, SIGNAL(clicked()),
             this, SLOT(save()));
    connect (pushButton_exit, SIGNAL(clicked()),
             this, SLOT(closeForm()));
    connect(cbOrder, SIGNAL(currentIndexChanged(int)),
            this, SLOT(orderChanged(int)));

    initOrders();
}

void EscortForm::closeForm()
{
    this->parentWidget()->close();
}

//
void EscortForm::fillHeaderModel(QStandardItemModel* headerModel)
{
    QStandardItem* item = new QStandardItem("Должность");
    QFontMetrics fm(item->font());
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 0, item);

    item = new QStandardItem("В/звание");
    item->setData(QSize(fm.width(item->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 1, item);

    item = new QStandardItem("Фамилия");
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 2, item);

    item = new QStandardItem("Имя");
    item->setData(QSize(fm.width(item->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 3, item);

    item = new QStandardItem("Отчество");
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 4, item);

    item = new QStandardItem("Следует с");
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 5, item);

    item = new QStandardItem("Вооружение");

    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 6, item);

    QList<QStandardItem*> list;

    QStandardItem* child = new QStandardItem("Наимен.");
    child->setData(QSize(fm.width(child->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    item->appendColumn(list<<child);
    list.clear();

    child = new QStandardItem("Номер");
    child->setData(QSize(fm.width(child->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    item->appendColumn(list<<child);
    list.clear();
    //
    item = new QStandardItem("Вагон");
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 7, item);

    child = new QStandardItem("Назнач.");
    child->setData(QSize(fm.width(child->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    item->appendColumn(list<<child);
    list.clear();

    child = new QStandardItem("Номер");
    child->setData(QSize(fm.width(child->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    item->appendColumn(list<<child);
    list.clear();

    item = new QStandardItem("Примечание");
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 8, item);
}

void EscortForm::addEscort()
{
    m_model->addEmptyRow();
}

void EscortForm::removeEscort()
{
    QModelIndex index = tvEscort->currentIndex();
    if (!index.isValid())
        return;   
    m_model->removeEscort(index);
}
//
void EscortForm::cancelEscort()
{
    m_model->load();
}
//
void EscortForm::save()
{
    if (!m_model->save())
        qDebug()<<"ERROR";
}

void EscortForm::orderChanged(int index)
{
    int order_id = cbOrder->itemData(index).toInt();
    m_model->changeOrder(order_id);
}

void EscortForm::initOrders()
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
