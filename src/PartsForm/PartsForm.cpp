#include "PartsForm/PartsForm.h"
#include "header_view/ProxyModelWithHeaderModels.h"
#include "PartsForm/PartsModel.h"
#include "PartsForm/PartsItemDelegate.h"

#include <OdsInterface>
#include "global_defs.h"
#include "OrderForm/orderform.h"
#include <QDebug>

PartsForm::PartsForm(int id, ODS::OdsInterface* iface, QWidget *parent) :
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
PartsForm::~PartsForm()
{
    delete m_proxy_header;
    delete m_headerModel;
    delete m_model;
}
//
void PartsForm::init()
{
    HierarchicalHeaderView* hv = new HierarchicalHeaderView(Qt::Horizontal, tvParts);
    tvParts->setHorizontalHeader(hv);

    m_model = new PartsModel(m_order_id, m_iface, tvParts);
    m_model->setColumnCount(11);

    m_headerModel = new QStandardItemModel(tvParts);
    fillHeaderModel(m_headerModel);

    m_proxy_header = new ProxyModelWithHeaderModels(tvParts);

#if QT_VERSION >= 0x050000
    m_proxy_header->setSourceModel(m_model);
#else
    m_proxy_header->setModel(m_model);
#endif
    m_proxy_header->setHorizontalHeaderModel(m_headerModel);

    tvParts->setItemDelegate(new PartsItemDelegate(tvParts));
    tvParts->setModel(m_proxy_header);
    tvParts->resizeColumnsToContents();
    tvParts->verticalHeader()->setVisible(false);
#if QT_VERSION >= 0x050000
    tvParts->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tvParts->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
#else
    tvParts->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    tvParts->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#endif
    //
    connect (pbAdd, SIGNAL(clicked()),
             this, SLOT(addParts()));
    connect (pbDel, SIGNAL(clicked()),
             this, SLOT(removeParts()));
    connect (pbCancel, SIGNAL(clicked()),
             this, SLOT(cancelParts()));
    connect (pbSave, SIGNAL(clicked()),
             this, SLOT(save()));
    connect (pushButton_exit, SIGNAL(clicked()),
             this, SLOT(closeForm()));
    connect(cbOrder, SIGNAL(currentIndexChanged(int)),
            this, SLOT(orderChanged(int)));

    initOrders();
}

void PartsForm::closeForm()
{
    this->parentWidget()->close();
}

//
void PartsForm::fillHeaderModel(QStandardItemModel* headerModel)
{
    QStandardItem* item = new QStandardItem("Наименование");
    QFontMetrics fm(item->font());
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 0, item);

    item = new QStandardItem("Номенклат номер");
    item->setData(QSize(fm.width(item->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 1, item);

    item = new QStandardItem("Обозначение\n(индекс)");
    item->setData(QSize(fm.width("Обозначение")+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 2, item);

    item = new QStandardItem("Зав. №");
    item->setData(QSize(fm.width("Шифр")+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 3, item);

    item = new QStandardItem("Гриф\nсекр.");
    item->setData(QSize(fm.width("секр.")+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 4, item);

    item = new QStandardItem("Шифр тары");
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 5, item);

    item = new QStandardItem("К-во мест");
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 6, item);

    item = new QStandardItem("Оттиски пломб на таре");
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 7, item);

    QList<QStandardItem*> list;

    QStandardItem* child = new QStandardItem("Пломба");
    child->setData(QSize(fm.width(child->text())+30, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    item->appendColumn(list<<child);
    list.clear();

    child = new QStandardItem("Количество");
    child->setData(QSize(fm.width(child->text())+30, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    item->appendColumn(list<<child);
    list.clear();

    item = new QStandardItem("Номер\nвагона");
    item->setData(QSize(fm.width("вагона")+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 8, item);

    item = new QStandardItem("Примечание");
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 9, item);
}
//
void PartsForm::addParts()
{
    m_model->addEmptyRow();
}
//
void PartsForm::removeParts()
{
    QModelIndex index = tvParts->currentIndex();
    if (!index.isValid())
        return;
   // qDebug()<<Q_FUNC_INFO<<index<<index.row();
    m_model->removeParts(index);
}
//
void PartsForm::cancelParts()
{
    m_model->load();
}
//
void PartsForm::save()
{
    if (!m_model->save())
        qDebug()<<"ERROR";
}

void PartsForm::orderChanged(int index)
{
    int order_id = cbOrder->itemData(index).toInt();
    m_model->changeOrder(order_id);
}

void PartsForm::initOrders()
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
