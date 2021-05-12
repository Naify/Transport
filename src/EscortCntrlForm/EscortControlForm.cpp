#include "EscortCntrlForm/EscortControlForm.h"
#include "header_view/ProxyModelWithHeaderModels.h"
#include "EscortCntrlForm/EscortControlModel.h"
#include "EscortCntrlForm/EscortControlItemDelegate.h"

#include <OdsInterface>
#include <QDebug>

EscortControlForm::EscortControlForm(int id, ODS::OdsInterface* iface, QWidget *parent) :
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
EscortControlForm::~EscortControlForm()
{
    delete m_proxy_header;
    delete m_headerModel;
    delete m_model;
}
//
void EscortControlForm::init()
{
    HierarchicalHeaderView* hv = new HierarchicalHeaderView(Qt::Horizontal, tvEscortControl);
    tvEscortControl->setHorizontalHeader(hv);

    m_model = new EscortControlModel(m_iface, tvEscortControl);
    m_model->setColumnCount(11);

    m_headerModel = new QStandardItemModel(tvEscortControl);
    fillHeaderModel(m_headerModel);

    m_proxy_header = new ProxyModelWithHeaderModels(tvEscortControl);

#if QT_VERSION >= 0x050000
    m_proxy_header->setSourceModel(m_model);
#else
    m_proxy_header->setModel(m_model);
#endif
    m_proxy_header->setHorizontalHeaderModel(m_headerModel);

    tvEscortControl->setItemDelegate(new EscortControlItemDelegate(tvEscortControl));
    tvEscortControl->setModel(m_proxy_header);
    tvEscortControl->resizeColumnsToContents();
    tvEscortControl->verticalHeader()->setVisible(false);
#if QT_VERSION >= 0x050000
    tvEscortControl->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tvEscortControl->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
#else
    tvEscortControl->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    tvEscortControl->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#endif

    tvEscortControl->hideColumn(0);
    tvEscortControl->hideColumn(5);

    //
//    connect (pbAdd, SIGNAL(clicked()),
//             this, SLOT(addEscortControl()));
//    connect (pbDel, SIGNAL(clicked()),
//             this, SLOT(removeEscortControl()));
//    connect (pbCancel, SIGNAL(clicked()),
//             this, SLOT(cancelEscortControl()));
//    connect (pbSave, SIGNAL(clicked()),
//             this, SLOT(save()));
}
//
void EscortControlForm::fillHeaderModel(QStandardItemModel* headerModel)
{
    QStandardItem* item = new QStandardItem("№№\nп/п");
    QFontMetrics fm(item->font());
    item->setData(QSize(fm.width("№№")+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 0, item);

    item = new QStandardItem("Дата");
    item->setData(QSize(fm.width(item->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 1, item);

    item = new QStandardItem("В/звание");
    item->setData(QSize(fm.width(item->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 2, item);

    item = new QStandardItem("Фамилия, инициалы");
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 3, item);

    item = new QStandardItem("Пункты командировки");
    item->setData(QSize(fm.width(item->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 4, item);

    item = new QStandardItem("Цель командировки");
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 5, item);

    item = new QStandardItem("Убытие");
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 6, item);

    QList<QStandardItem*> list;

    QStandardItem* child = new QStandardItem("План");
    child->setData(QSize(fm.width(child->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    item->appendColumn(list<<child);
    list.clear();

    child = new QStandardItem("Факт");
    child->setData(QSize(fm.width(child->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    item->appendColumn(list<<child);
    list.clear();

    item = new QStandardItem("Прибытие");
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 7, item);
    //
    child = new QStandardItem("План");
    child->setData(QSize(fm.width(child->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    item->appendColumn(list<<child);
    list.clear();

    child = new QStandardItem("Факт");
    child->setData(QSize(fm.width(child->text())+20, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    item->appendColumn(list<<child);
    list.clear();

    item = new QStandardItem("Примечание");
    item->setData(QSize(fm.width(item->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    headerModel->setItem(0, 8, item);
}
//

