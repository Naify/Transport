#include "ShuttleTaskForm/createshuttletask.h"
#include "ui_createshuttletask.h"
#include <QMessageBox>
#include <staffstructuremanager.h>
#include <CatalogManager.h>
#include <OdsInterface>
#include "header_view/ProxyModelWithHeaderModels.h"
#include "ShuttleTaskForm/ShuttleItemDelegate.h"

CreateShuttleTask::CreateShuttleTask(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateShuttleTask)
{
    ui->setupUi(this);

    //передаю -1 как id марш задания для инициализации! потом поменять при сейве!!!!
    this->model_pa_pnvt_s = new ShuttleModel(-1 ,ODS::OdsInterface::self(), this->ui->tv_pa_pnvt_s);
    this->model_respzone_s = new QStandardItemModel(this);
    this->model_checkpoints_s = new QStandardItemModel(this);

    //передаю -1 как id марш задания для инициализации! потом поменять при сейве!!!!
    this->model_pa_pnvt_r = new ShuttleModel(-1 ,ODS::OdsInterface::self(), this->ui->tv_pa_pnvt_r);
    this->model_respzone_r = new QStandardItemModel(this);
    this->model_checkpoints_r = new QStandardItemModel(this);

    //other form code

    HierarchicalHeaderView* hv = new HierarchicalHeaderView(Qt::Horizontal, this->ui->tv_pa_pnvt_s);
    HierarchicalHeaderView* hv2 = new HierarchicalHeaderView(Qt::Horizontal, this->ui->tv_pa_pnvt_s);
    this->ui->tv_pa_pnvt_s->setHorizontalHeader(hv);
    this->ui->tv_pa_pnvt_r->setHorizontalHeader(hv2);

    this->model_pa_pnvt_s->setColumnCount(4);
    this->model_pa_pnvt_r->setColumnCount(4);

    m_headerModel = new QStandardItemModel(this->ui->tv_pa_pnvt_s);
    m_headerModel2 = new QStandardItemModel(this->ui->tv_pa_pnvt_r);

//    fillHeaderModel(m_headerModel);
//    fillHeaderModel(m_headerModel2);

    m_proxy_header = new ProxyModelWithHeaderModels(this->ui->tv_pa_pnvt_s);
    m_proxy_header2 = new ProxyModelWithHeaderModels(this->ui->tv_pa_pnvt_r);

#if QT_VERSION >= 0x050000
    m_proxy_header->setSourceModel(model_pa_pnvt_s);
    m_proxy_header2->setSourceModel(model_pa_pnvt_r);
#else
    m_proxy_header->setModel(model_pa_pnvt_s);
    m_proxy_header2->setModel(model_pa_pnvt_r);
#endif

    m_proxy_header->setHorizontalHeaderModel(m_headerModel);    
    m_proxy_header2->setHorizontalHeaderModel(m_headerModel2);

    this->ui->tv_pa_pnvt_s->setItemDelegate(new ShuttleItemDelegate(this->ui->tv_pa_pnvt_s));
    this->ui->tv_pa_pnvt_s->setModel(m_proxy_header);
    this->ui->tv_pa_pnvt_s->resizeColumnsToContents();
    this->ui->tv_pa_pnvt_s->verticalHeader()->setVisible(false);
#if QT_VERSION >= 0x050000
    this->ui->tv_pa_pnvt_s->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->ui->tv_pa_pnvt_s->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
#else
    this->ui->tv_pa_pnvt_s->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    this->ui->tv_pa_pnvt_s->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#endif

    this->ui->tv_pa_pnvt_r->setItemDelegate(new ShuttleItemDelegate(this->ui->tv_pa_pnvt_r));
    this->ui->tv_pa_pnvt_r->setModel(m_proxy_header2);
    this->ui->tv_pa_pnvt_r->resizeColumnsToContents();
    this->ui->tv_pa_pnvt_r->verticalHeader()->setVisible(false);
#if QT_VERSION >= 0x050000
    this->ui->tv_pa_pnvt_r->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->ui->tv_pa_pnvt_r->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
#else
    this->ui->tv_pa_pnvt_r->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    this->ui->tv_pa_pnvt_r->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#endif


    //end of the other form code

    this->model_pa_pnvt_s->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
    this->model_pa_pnvt_s->setHorizontalHeaderItem(1, new QStandardItem(QString("Номер ЖДЭ")));
    this->model_pa_pnvt_s->setHorizontalHeaderItem(2, new QStandardItem(QString("Наим. станц. п/а")));
    this->model_pa_pnvt_s->setHorizontalHeaderItem(3, new QStandardItem(QString("ПНВТ")));

    this->model_pa_pnvt_r->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
    this->model_pa_pnvt_r->setHorizontalHeaderItem(1, new QStandardItem(QString("Номер ЖДЭ")));
    this->model_pa_pnvt_r->setHorizontalHeaderItem(2, new QStandardItem(QString("Наим. станц. п/а")));
    this->model_pa_pnvt_r->setHorizontalHeaderItem(3, new QStandardItem(QString("ПНВТ")));

    this->model_respzone_s->setHorizontalHeaderItem(0, new QStandardItem(QString("Зона ответств.")));
    this->model_respzone_s->setHorizontalHeaderItem(1, new QStandardItem(QString("Вход")));
    this->model_respzone_s->setHorizontalHeaderItem(2, new QStandardItem(QString("Выход")));

    this->model_respzone_r->setHorizontalHeaderItem(0, new QStandardItem(QString("Зона ответств.")));
    this->model_respzone_r->setHorizontalHeaderItem(1, new QStandardItem(QString("Вход")));
    this->model_respzone_r->setHorizontalHeaderItem(2, new QStandardItem(QString("Выход")));

    this->model_checkpoints_s->setHorizontalHeaderItem(0, new QStandardItem(QString("Наименование")));
    this->model_checkpoints_s->setHorizontalHeaderItem(1, new QStandardItem(QString("Прибытие")));
    this->model_checkpoints_s->setHorizontalHeaderItem(2, new QStandardItem(QString("Убытие")));
    this->model_checkpoints_s->setHorizontalHeaderItem(3, new QStandardItem(QString("НВТ")));

    this->model_checkpoints_r->setHorizontalHeaderItem(0, new QStandardItem(QString("Наименование")));
    this->model_checkpoints_r->setHorizontalHeaderItem(1, new QStandardItem(QString("Прибытие")));
    this->model_checkpoints_r->setHorizontalHeaderItem(2, new QStandardItem(QString("Убытие")));
    this->model_checkpoints_r->setHorizontalHeaderItem(3, new QStandardItem(QString("НВТ")));

//    this->ui->tv_pa_pnvt_s->setModel(this->model_pa_pnvt_s);
    this->ui->tv_respzone_s->setModel(this->model_respzone_s);
    this->ui->tv_checkpoints_s->setModel(this->model_checkpoints_s);

//    this->ui->tv_pa_pnvt_r->setModel(this->model_pa_pnvt_r);
    this->ui->tv_respzone_r->setModel(this->model_respzone_r);
    this->ui->tv_checkpoints_r->setModel(this->model_checkpoints_r);

//    this->ui->tv_pa_pnvt_s->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
//    this->ui->tv_pa_pnvt_r->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#if QT_VERSION >= 0x050000
    this->ui->tv_pa_pnvt_r->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->ui->tv_pa_pnvt_r->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->ui->tv_checkpoints_s->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->ui->tv_checkpoints_r->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#else
    this->ui->tv_pa_pnvt_r->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    this->ui->tv_pa_pnvt_r->verticalHeader()->setResizeMode(QHeaderView::Stretch);
    this->ui->tv_checkpoints_s->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    this->ui->tv_checkpoints_r->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#endif

    QStringList typeList;
    typeList << "" << "КП-1" << "КП-2" << "КП-3" << "КП-4" << "КП-5";
    this->ui->cb_cat_type_s->addItems(typeList);
    this->ui->cb_cat_type_r->addItems(typeList);

    this->ui->de_mzdate->setDate(QDate::currentDate());
    this->ui->dte_senddate_s->setDate(QDate::currentDate());
    this->ui->dte_senddate_r->setDate(QDate::currentDate());
    this->ui->dte_receivedate_s->setDate(QDate::currentDate());
    this->ui->dte_receivedate_r->setDate(QDate::currentDate());

    IRS::CatalogManager catalogMgr(ODS::OdsInterface::self());
    IRS::Catalog::ptr catalog_units = catalogMgr.getCatalogByName("Классификатор воинских частей");

    this->ui->cb_sender_s->addItem("");
    ODS::IORecordCursor cb_sender = ODS::OdsInterface::self()->iobjectManager().exec("SELECT DISTINCT \"Отправитель\" FROM \"Контроль перевозок\".\"Содержание таблица\"");
    for (ODS::IORecordCursor::iterator it = cb_sender.begin(); it != cb_sender.end(); ++it)
    {
        ODS::IORecord cur = *it;

        QString name;
        IRS::CatalogEntry::ptr c_entry = catalog_units->entryById(cur.getLink("Отправитель").id());
        if (!c_entry.isNull())
        {
            name = c_entry->stringValue("Условное наименование");
            this->senderMap.insert(name, cur.getLink("Отправитель").id());
            this->ui->cb_sender_s->addItem(name);
        }
    }

    ODS::IObjectCursor rasp_cursor = ODS::OdsInterface::self()->iobjectManager().getIObjectCursor("Контроль перевозок.Распоряжения");

    this->ui->cb_raspor->addItem("");
    for (ODS::IObjectCursor::iterator it = rasp_cursor.begin(); it != rasp_cursor.end(); ++it)
    {
        ODS::IObject cur = *it;
        QString num = cur.getStringAttr("Номер");
        CDate date = cur.getDateTimeAttr("Дата");
        QDate qdate(date.getYear(), date.getMonthNum(), date.getMonthDay());
        num.append(" от " + qdate.toString("dd.MM.yyyy"));

        this->raspMap.insert(num, cur.id());
        this->ui->cb_raspor->addItem(num);
    }

    ODS::IORecordCursor mz_query = ODS::OdsInterface::self()->iobjectManager().exec("SELECT max(\"Контроль перевозок\".\"Маршрутное задание\".\"Номер задания\") as max_num FROM \"Контроль перевозок\".\"Маршрутное задание\"");
    int mz_num = 0;
    for (ODS::IORecordCursor::iterator it = mz_query.begin(); it != mz_query.end(); ++it)
    {
        ODS::IORecord cur = *it;

        mz_num = cur.getIntAttr("max_num");
    }

//    qDebug()<<"mz_num = "<<mz_num;

    if (mz_num == 0) {
        this->ui->sb_mznumber->setValue(1);
    } else {
        this->ui->sb_mznumber->setValue(mz_num+1);
    }

    this->ui->cb_cat_type_s->setVisible(false);
    this->ui->cb_cat_type_r->setVisible(false);
    this->ui->l_cat_s->setVisible(false);
    this->ui->l_cat_r->setVisible(false);
    this->ui->le_transport_type->setVisible(false);
    this->ui->l_transport_type->setVisible(false);
    this->ui->l_no_load_s->setVisible(false);
    this->ui->l_no_load_r->setVisible(false);

    QObject::connect(this->ui->pbSave, SIGNAL(clicked()), this, SLOT(processSave()));
    QObject::connect(this->ui->pbCancel, SIGNAL(clicked()), this, SLOT(closeForm()));
    QObject::connect(this->ui->pb_checkpoints_s_add, SIGNAL(clicked()), this, SLOT(processAddToCheckPoints_S()));
    QObject::connect(this->ui->pb_checkpoints_r_add, SIGNAL(clicked()), this, SLOT(processAddToCheckPoints_R()));
    QObject::connect(this->ui->pb_pathcp_s_add, SIGNAL(clicked()), this, SLOT(processAddToPaPnvt_S()));
    QObject::connect(this->ui->pb_pathcp_r_add, SIGNAL(clicked()), this, SLOT(processAddToPaPnvt_R()));
    QObject::connect(this->ui->pb_respzone_s_add, SIGNAL(clicked()), this, SLOT(processAddToRespZone_S()));
    QObject::connect(this->ui->pb_respzone_r_add, SIGNAL(clicked()), this, SLOT(processAddToRespZone_R()));
    QObject::connect(this->ui->cb_raspor, SIGNAL(currentIndexChanged(QString)), this, SLOT(displayTransportType(QString)));
    QObject::connect(this->ui->cb_sender_s, SIGNAL(currentIndexChanged(QString)), this, SLOT(fill_cb_receiver_s(QString)));
    QObject::connect(this->ui->cb_receiver_s, SIGNAL(currentIndexChanged(QString)), this, SLOT(update_return_le(QString)));
    QObject::connect(this->ui->le_sender_s, SIGNAL(textChanged(QString)), this->ui->le_receiver_r, SLOT(setText(QString)));
    QObject::connect(this->ui->le_receiver_s, SIGNAL(textChanged(QString)), this->ui->le_sender_r, SLOT(setText(QString)));
    QObject::connect(this->ui->pb_newmz, SIGNAL(released()), this, SLOT(clear_form()));
    QObject::connect(this->ui->sb_mznumber, SIGNAL(valueChanged(int)), this, SLOT(changeMz(int)));
}

CreateShuttleTask::~CreateShuttleTask()
{
    delete this->ui;

    delete this->model_pa_pnvt_s;
    delete this->model_respzone_s;
    delete this->model_checkpoints_s;

    delete this->model_pa_pnvt_r;
    delete this->model_respzone_r;
    delete this->model_checkpoints_r;
}

void CreateShuttleTask::closeForm()
{
    this->parentWidget()->close();
}

void CreateShuttleTask::db_error_add()
{
    QMessageBox().information(this, "Ошибка", "Ошибка добавления маршрутного задания");
}

void CreateShuttleTask::fields_not_filled()
{
    QMessageBox().information(this, "Ошибка", "Не заполнены поля");
}

void CreateShuttleTask::processSave()
{
    ODS::OdsInterface::self()->dbManager().transaction();

    ODS::OdsInterface::self()->syncManager().syncTransaction();

    bool updFlag = true;
    ODS::IObject toSave_s, toSave_r;

    ODS::IObjectQueryFilter filter_mz;
    filter_mz.whereCondition = "\"Номер задания\" = " + QString::number(this->ui->sb_mznumber->value());
    ODS::IObjectCursor cur_mz = ODS::OdsInterface::self()->iobjectManager().getIObjectCursor("Контроль перевозок.Маршрутное задание", filter_mz);

    if (cur_mz.size() == 0) {

        toSave_s = ODS::OdsInterface::self()->iobjectManager().createIObject("Контроль перевозок.Маршрутное задание");
        toSave_r = ODS::OdsInterface::self()->iobjectManager().createIObject("Контроль перевозок.Маршрутное задание");

        toSave_s.save(ODS::IObject::NotRecursiveSave);
        toSave_r.save(ODS::IObject::NotRecursiveSave);
        updFlag = false;

    } else {

        for (ODS::IObjectCursor::iterator it = cur_mz.begin(); it != cur_mz.end(); ++it)
        {
            ODS::IObject iobj = *it;

            if (iobj.getIntAttr("Направление") == 1){
                toSave_s = iobj;
            } else {
                toSave_r = iobj;
            }
        }
    }

    ODS::Group adminGroup = ODS::OdsInterface::self()->subjectManager().getGroup("Администраторы Контроль перевозок");
    ODS::Group userGroup = ODS::OdsInterface::self()->subjectManager().getGroup("Пользователи Контроль перевозок");

    qDebug()<<"колво до"<<this->model_pa_pnvt_s->rowCount();

    this->model_pa_pnvt_s->setId(toSave_s.id());

    if (this->model_pa_pnvt_s->rowCount() > 0)
    {
        for (int i = 0; i < this->model_pa_pnvt_s->rowCount(); i++)
        {
            if (this->model_pa_pnvt_s->data(this->model_pa_pnvt_s->index(i,0)).toInt() > 0 &&
                this->model_pa_pnvt_s->data(this->model_pa_pnvt_s->index(i,1)).toString().count() > 0)
            {
                ODS::IObject db_save_pathcp_s;
                if (!updFlag || this->model_pa_pnvt_s->data(this->model_pa_pnvt_s->index(i,0), Qt::UserRole).isNull()){
                    db_save_pathcp_s = ODS::OdsInterface::self()->iobjectManager().createIObject("Контроль перевозок.Станции ПА и ПНВТ");
                    db_save_pathcp_s.setAttr("Марш задание", toSave_s);
                } else {
                    db_save_pathcp_s = ODS::OdsInterface::self()->iobjectManager().getIObjectById(this->model_pa_pnvt_s->data(this->model_pa_pnvt_s->index(i,0), Qt::UserRole).toInt());
                }
                int s_num = this->model_pa_pnvt_s->data(this->model_pa_pnvt_s->index(i,0)).toInt();
                db_save_pathcp_s.setAttr("Порядковый номер", s_num);
                QString num_gd = this->model_pa_pnvt_s->data(this->model_pa_pnvt_s->index(i,1)).toString();
                db_save_pathcp_s.setAttr("Номер ЖДЭ", num_gd);
                QString pa = this->model_pa_pnvt_s->data(this->model_pa_pnvt_s->index(i,2)).toString();
                db_save_pathcp_s.setAttr("Станция ПА", pa);
                QString pnvt = this->model_pa_pnvt_s->data(this->model_pa_pnvt_s->index(i,3)).toString();
                db_save_pathcp_s.setAttr("ПНВТ", pnvt);
                db_save_pathcp_s.update(ODS::IObject::NotRecursiveUpdate);                
                adminGroup.setRightsToIObject(db_save_pathcp_s, ODS::Right::All);
                userGroup.setRightsToIObject(db_save_pathcp_s, ODS::Right::Read);
            }
            else
            {
                this->fields_not_filled();
                ODS::OdsInterface::self()->dbManager().rollback();
                return;
            }
        }
    }
    else
    {
        this->fields_not_filled();
        ODS::OdsInterface::self()->dbManager().rollback();
        return;
    }

    if (this->model_pa_pnvt_r->rowCount() > 0)
    {
        for (int i = 0; i < this->model_pa_pnvt_r->rowCount(); i++)
        {
            if (this->model_pa_pnvt_r->data(this->model_pa_pnvt_r->index(i,0)).toInt() > 0 &&
                this->model_pa_pnvt_r->data(this->model_pa_pnvt_r->index(i,1)).toString().count() > 0)
            {
                ODS::IObject db_save_pathcp_r;
                if (!updFlag || this->model_pa_pnvt_r->data(this->model_pa_pnvt_r->index(i,0), Qt::UserRole).isNull()){
                    db_save_pathcp_r = ODS::OdsInterface::self()->iobjectManager().createIObject("Контроль перевозок.Станции ПА и ПНВТ");
                    db_save_pathcp_r.setAttr("Марш задание", toSave_r);
                } else {
                    db_save_pathcp_r = ODS::OdsInterface::self()->iobjectManager().getIObjectById(this->model_pa_pnvt_r->data(this->model_pa_pnvt_r->index(i,0), Qt::UserRole).toInt());
                }
                int s_num = this->model_pa_pnvt_r->data(this->model_pa_pnvt_r->index(i,0)).toInt();
                db_save_pathcp_r.setAttr("Порядковый номер", s_num);
                QString num_gd = this->model_pa_pnvt_r->data(this->model_pa_pnvt_r->index(i,1)).toString();
                db_save_pathcp_r.setAttr("Номер ЖДЭ", num_gd);
                QString pa = this->model_pa_pnvt_r->data(this->model_pa_pnvt_r->index(i,2)).toString();
                db_save_pathcp_r.setAttr("Станция ПА", pa);
                QString pnvt = this->model_pa_pnvt_r->data(this->model_pa_pnvt_r->index(i,3)).toString();
                db_save_pathcp_r.setAttr("ПНВТ", pnvt);
                db_save_pathcp_r.update(ODS::IObject::NotRecursiveUpdate);                
                adminGroup.setRightsToIObject(db_save_pathcp_r, ODS::Right::All);
                userGroup.setRightsToIObject(db_save_pathcp_r, ODS::Right::Read);
            }
            else
            {
                this->fields_not_filled();
                ODS::OdsInterface::self()->dbManager().rollback();
                return;
            }
        }
    }
    else
    {
        this->fields_not_filled();
        ODS::OdsInterface::self()->dbManager().rollback();
        return;
    }

    if (this->model_respzone_s->rowCount() > 0)
    {
        for (int i = 0; i < this->model_respzone_s->rowCount(); i++)
        {
            if (this->model_respzone_s->data(this->model_respzone_s->index(i,0)).toString().count() > 0 &&
                (this->model_respzone_s->data(this->model_respzone_s->index(i,1)).toDateTime().toMSecsSinceEpoch() <=
                this->model_respzone_s->data(this->model_respzone_s->index(i,2)).toDateTime().toMSecsSinceEpoch()))
            {
                ODS::IObject db_save_respzone_s;
                if (!updFlag || this->model_respzone_s->data(this->model_respzone_s->index(i,0), Qt::UserRole).isNull()){
                    db_save_respzone_s = ODS::OdsInterface::self()->iobjectManager().createIObject("Контроль перевозок.Зоны ответственности");
                    db_save_respzone_s.setAttr("Марш задание", toSave_s);
                } else {
                    db_save_respzone_s = ODS::OdsInterface::self()->iobjectManager().getIObjectById(this->model_respzone_s->data(this->model_respzone_s->index(i,0), Qt::UserRole).toInt());
                }
                QString obj_name = this->model_respzone_s->data(this->model_respzone_s->index(i,0)).toString();
                db_save_respzone_s.setAttr("Наименование", obj_name);
                QDateTime d_in = this->model_respzone_s->data(this->model_respzone_s->index(i,1)).toDateTime();
                CDate din;
                time_t t_in = d_in.toTime_t();
                din.setDateTime(t_in);
                db_save_respzone_s.setAttr("Вход", din);
                QDateTime d_out = this->model_respzone_s->data(this->model_respzone_s->index(i,2)).toDateTime();
                CDate dout;
                time_t t_out = d_out.toTime_t();
                dout.setDateTime(t_out);
                db_save_respzone_s.setAttr("Выход", dout);
                db_save_respzone_s.update(ODS::IObject::NotRecursiveUpdate);
                adminGroup.setRightsToIObject(db_save_respzone_s, ODS::Right::All);
                userGroup.setRightsToIObject(db_save_respzone_s, ODS::Right::Read);
            }
            else
            {
                this->fields_not_filled();
                ODS::OdsInterface::self()->dbManager().rollback();
                return;
            }
        }
    }
    else
    {
        this->fields_not_filled();
        ODS::OdsInterface::self()->dbManager().rollback();
        return;
    }

    if (this->model_respzone_r->rowCount() > 0)
    {
        for (int i = 0; i < this->model_respzone_r->rowCount(); i++)
        {
            if (this->model_respzone_r->data(this->model_respzone_r->index(i,0)).toString().count() > 0 &&
                (this->model_respzone_r->data(this->model_respzone_r->index(i,1)).toDateTime().toMSecsSinceEpoch() <=
                this->model_respzone_r->data(this->model_respzone_r->index(i,2)).toDateTime().toMSecsSinceEpoch()))
            {
                ODS::IObject db_save_respzone_r;
                if (!updFlag || this->model_respzone_r->data(this->model_respzone_r->index(i,0), Qt::UserRole).isNull()){
                    db_save_respzone_r = ODS::OdsInterface::self()->iobjectManager().createIObject("Контроль перевозок.Зоны ответственности");
                    db_save_respzone_r.setAttr("Марш задание", toSave_r);
                } else {
                    db_save_respzone_r = ODS::OdsInterface::self()->iobjectManager().getIObjectById(this->model_respzone_r->data(this->model_respzone_r->index(i,0), Qt::UserRole).toInt());
                }
                QString obj_name = this->model_respzone_r->data(this->model_respzone_r->index(i,0)).toString();
                db_save_respzone_r.setAttr("Наименование", obj_name);
                QDateTime d_in = this->model_respzone_r->data(this->model_respzone_r->index(i,1)).toDateTime();
                CDate din;
                time_t t_in = d_in.toTime_t();
                din.setDateTime(t_in);
                db_save_respzone_r.setAttr("Вход", din);
                QDateTime d_out = this->model_respzone_r->data(this->model_respzone_r->index(i,2)).toDateTime();
                CDate dout;
                time_t t_out = d_out.toTime_t();
                dout.setDateTime(t_out);
                db_save_respzone_r.setAttr("Выход", dout);
                db_save_respzone_r.update(ODS::IObject::NotRecursiveUpdate);                
                adminGroup.setRightsToIObject(db_save_respzone_r, ODS::Right::All);
                userGroup.setRightsToIObject(db_save_respzone_r, ODS::Right::Read);
            }
            else
            {
                this->fields_not_filled();
                ODS::OdsInterface::self()->dbManager().rollback();
                return;
            }
        }
    }
    else
    {
        this->fields_not_filled();
        ODS::OdsInterface::self()->dbManager().rollback();
        return;
    }

    if (this->model_checkpoints_s->rowCount() > 0)
    {
        for (int i = 0; i < this->model_checkpoints_s->rowCount(); i++)
        {
            if (this->model_checkpoints_s->data(this->model_checkpoints_s->index(i,0)).toString().count() > 0 &&
//                this->model_checkpoints_s->data(this->model_checkpoints_s->index(i,3)).toString().count() > 0 &&
                (this->model_checkpoints_s->data(this->model_checkpoints_s->index(i,1)).toDateTime().toMSecsSinceEpoch() <=
                this->model_checkpoints_s->data(this->model_checkpoints_s->index(i,2)).toDateTime().toMSecsSinceEpoch()))
            {
                ODS::IObject db_save_checkpoints_s;
                if (!updFlag || this->model_checkpoints_s->data(this->model_checkpoints_s->index(i,0), Qt::UserRole).isNull()){
                    db_save_checkpoints_s = ODS::OdsInterface::self()->iobjectManager().createIObject("Контроль перевозок.Контрольные точки");
                    db_save_checkpoints_s.setAttr("Марш задание", toSave_s);
                } else {
                    db_save_checkpoints_s = ODS::OdsInterface::self()->iobjectManager().getIObjectById(this->model_checkpoints_s->data(this->model_checkpoints_s->index(i,0), Qt::UserRole).toInt());
                }
                QString obj_name = this->model_checkpoints_s->data(this->model_checkpoints_s->index(i,0)).toString();
                db_save_checkpoints_s.setAttr("Наименование", obj_name);
                QDateTime d_in = this->model_checkpoints_s->data(this->model_checkpoints_s->index(i,1)).toDateTime();
                CDate din;
                time_t t_in = d_in.toTime_t();
                din.setDateTime(t_in);
                db_save_checkpoints_s.setAttr("Прибытие", din);
                QDateTime d_out = this->model_checkpoints_s->data(this->model_checkpoints_s->index(i,2)).toDateTime();
                CDate dout;
                time_t t_out = d_out.toTime_t();
                dout.setDateTime(t_out);
                db_save_checkpoints_s.setAttr("Убытие", dout);
                QString nvt_s = this->model_checkpoints_s->data(this->model_checkpoints_s->index(i,3)).toString();
                db_save_checkpoints_s.setAttr("НВТ", nvt_s);
                db_save_checkpoints_s.update(ODS::IObject::NotRecursiveUpdate);
                adminGroup.setRightsToIObject(db_save_checkpoints_s, ODS::Right::All);
                userGroup.setRightsToIObject(db_save_checkpoints_s, ODS::Right::Read);
            }
            else
            {
                this->fields_not_filled();
                ODS::OdsInterface::self()->dbManager().rollback();
                return;
            }
        }
    }
    else
    {
        this->fields_not_filled();
        ODS::OdsInterface::self()->dbManager().rollback();
        return;
    }

    if (this->model_checkpoints_r->rowCount() > 0)
    {
        for (int i = 0; i < this->model_checkpoints_r->rowCount(); i++)
        {
            if (this->model_checkpoints_r->data(this->model_checkpoints_r->index(i,0)).toString().count() > 0 &&
//                this->model_checkpoints_r->data(this->model_checkpoints_r->index(i,3)).toString().count() > 0 &&
                (this->model_checkpoints_r->data(this->model_checkpoints_r->index(i,1)).toDateTime().toMSecsSinceEpoch() <=
                this->model_checkpoints_r->data(this->model_checkpoints_r->index(i,2)).toDateTime().toMSecsSinceEpoch()))
            {
                ODS::IObject db_save_checkpoints_r;
                if (!updFlag || this->model_checkpoints_r->data(this->model_checkpoints_r->index(i,0), Qt::UserRole).isNull()){
                    db_save_checkpoints_r = ODS::OdsInterface::self()->iobjectManager().createIObject("Контроль перевозок.Контрольные точки");
                    db_save_checkpoints_r.setAttr("Марш задание", toSave_r);
                } else {
                    db_save_checkpoints_r = ODS::OdsInterface::self()->iobjectManager().getIObjectById(this->model_checkpoints_r->data(this->model_checkpoints_r->index(i,0), Qt::UserRole).toInt());
                }
                QString obj_name = this->model_checkpoints_r->data(this->model_checkpoints_r->index(i,0)).toString();
                db_save_checkpoints_r.setAttr("Наименование", obj_name);
                QDateTime d_in = this->model_checkpoints_r->data(this->model_checkpoints_r->index(i,1)).toDateTime();
                CDate din;
                time_t t_in = d_in.toTime_t();
                din.setDateTime(t_in);
                db_save_checkpoints_r.setAttr("Прибытие", din);
                QDateTime d_out = this->model_checkpoints_r->data(this->model_checkpoints_r->index(i,2)).toDateTime();
                CDate dout;
                time_t t_out = d_out.toTime_t();
                dout.setDateTime(t_out);
                db_save_checkpoints_r.setAttr("Убытие", dout);
                QString nvt_r = this->model_checkpoints_r->data(this->model_checkpoints_r->index(i,3)).toString();
                db_save_checkpoints_r.setAttr("НВТ", nvt_r);
                db_save_checkpoints_r.update(ODS::IObject::NotRecursiveUpdate);               
                adminGroup.setRightsToIObject(db_save_checkpoints_r, ODS::Right::All);
                userGroup.setRightsToIObject(db_save_checkpoints_r, ODS::Right::Read);
            }
            else
            {
                this->fields_not_filled();
                ODS::OdsInterface::self()->dbManager().rollback();
                return;
            }
        }
    }
    else
    {
        this->fields_not_filled();
        ODS::OdsInterface::self()->dbManager().rollback();
        return;
    }

    if (!this->ui->sb_mznumber->value() ||
        !this->ui->cb_raspor->currentText().count() ||
        !this->ui->cb_object_name->currentText().count() ||
        !this->ui->cb_sender_s->currentText().count() ||
        !this->ui->le_cb_sender_r->text().count() ||        
        !this->ui->cb_receiver_s->currentText().count() ||
        !this->ui->le_cb_receiver_r->text().count() ||        
        !this->ui->cb_cat_type_s->currentText().count() ||
        !this->ui->cb_cat_type_r->currentText().count())
    {
        this->fields_not_filled();
        ODS::OdsInterface::self()->dbManager().rollback();
        return;
    }

    if (this->ui->le_transport_type->isVisible()  && this->ui->le_transport_type->text().count())
    {
        toSave_r.setAttr("Категория", this->ui->le_transport_type->text());
        toSave_s.setAttr("Категория", this->ui->le_transport_type->text());
    }

    toSave_r.setAttr("Категория", this->ui->cb_cat_type_r->currentText());
    toSave_s.setAttr("Категория", this->ui->cb_cat_type_s->currentText());

    int y,m,d;
    this->ui->de_mzdate->date().getDate(&d,&m,&y);
    CDate mzad_date;
    mzad_date.setDate(y,m,d);

    ODS::IObject raspObj = ODS::OdsInterface::self()->iobjectManager().getIObjectById(this->raspMap.value(this->ui->cb_raspor->currentText()));

    if (!(toSave_s.isValid() &&
        toSave_s.setAttr("Выполнен", false) &&
        toSave_s.setAttr("Номер задания", this->ui->sb_mznumber->value()) &&
        toSave_s.setAttr("Дата марш задания", mzad_date) &&
        toSave_s.setAttr("Направление", 1) &&
        toSave_s.setAttr("Количество грузов", this->load_count_send) &&
        toSave_s.setAttr("Наименование", this->ui->cb_object_name->currentText()) &&
        raspObj.isValid() &&
        toSave_s.setAttr("Распоряжение", raspObj)))
    {
        ODS::OdsInterface::self()->dbManager().rollback();
        this->db_error_add();
        return;
    }

    if (!(toSave_r.isValid() &&
        toSave_r.setAttr("Выполнен", false) &&
        toSave_r.setAttr("Номер задания", this->ui->sb_mznumber->value()) &&
        toSave_r.setAttr("Дата марш задания", mzad_date) &&
        toSave_r.setAttr("Направление", 2) &&
        toSave_r.setAttr("Количество грузов", this->load_count_return) &&
        toSave_r.setAttr("Наименование", this->ui->cb_object_name->currentText()) &&
        raspObj.isValid() &&
        toSave_r.setAttr("Распоряжение", raspObj)))
    {
        ODS::OdsInterface::self()->dbManager().rollback();
        this->db_error_add();
        return;
    }

    int sender_s_id = this->senderMap.value(this->ui->cb_sender_s->currentText());
    int receiver_s_id = this->receiverMap.value(this->ui->cb_receiver_s->currentText());

    ODS::IObjectQueryFilter filter_s;
    filter_s.whereCondition = "\"Отправитель\" = " + QString::number(sender_s_id) + " AND \"Получатель\" = " + QString::number(receiver_s_id);
    ODS::IObjectCursor send = ODS::OdsInterface::self()->iobjectManager().getIObjectCursor("Контроль перевозок.Содержание таблица", filter_s);
    for (ODS::IObjectCursor::iterator it = send.begin(); it != send.end(); ++it)
    {
        ODS::IObject cur = *it;
        cur.setAttr("Марш задание", toSave_s);
        cur.update(ODS::IObject::NotRecursiveUpdate);
    }

    ODS::IObjectQueryFilter filter_r;
    filter_r.whereCondition = "\"Отправитель\" = " + QString::number(receiver_s_id) + " AND \"Получатель\" = " + QString::number(sender_s_id);
    ODS::IObjectCursor ret = ODS::OdsInterface::self()->iobjectManager().getIObjectCursor("Контроль перевозок.Содержание таблица", filter_r);
    for (ODS::IObjectCursor::iterator it = ret.begin(); it != ret.end(); ++it)
    {
        ODS::IObject cur = *it;
        cur.setAttr("Марш задание", toSave_r);
        cur.update(ODS::IObject::NotRecursiveUpdate);
    }

    if ((this->ui->dte_senddate_s->dateTime().toMSecsSinceEpoch() <= this->ui->dte_receivedate_s->dateTime().toMSecsSinceEpoch()) &&
        (this->ui->dte_senddate_r->dateTime().toMSecsSinceEpoch() <= this->ui->dte_receivedate_r->dateTime().toMSecsSinceEpoch()))
    {
        QDateTime d_in_r = this->ui->dte_senddate_r->dateTime();
        CDate din_r;
        time_t t_in_r = d_in_r.toTime_t();
        din_r.setDateTime(t_in_r);
        toSave_r.setAttr("ДатаВремя отправки", din_r);

        QDateTime d_out_r = this->ui->dte_receivedate_r->dateTime();
        CDate dout_r;
        time_t t_out_r = d_out_r.toTime_t();
        dout_r.setDateTime(t_out_r);
        toSave_r.setAttr("ДатаВремя возврата", dout_r);

        ODS::IObject snd_r = ODS::OdsInterface::self()->iobjectManager().getIObjectById(this->senderMap.value(this->ui->le_cb_sender_r->text()));
        toSave_r.setAttr("Отправитель задание", snd_r);
        toSave_r.setAttr("Станция отправки", this->ui->le_cb_sender_r->text());
        ODS::IObject rcv_r = ODS::OdsInterface::self()->iobjectManager().getIObjectById(this->senderMap.value(this->ui->le_cb_receiver_r->text()));
        toSave_r.setAttr("Получатель задание", rcv_r);
        toSave_r.setAttr("Станция прибытия", this->ui->le_cb_receiver_r->text());
        toSave_r.setAttr("Транспорт", ODS::OdsInterface::self()->iobjectManager().getIObjectById(this->objectNameMap.value(this->ui->cb_object_name->currentText())));

        QDateTime d_in_s = this->ui->dte_senddate_s->dateTime();
        CDate din_s;
        time_t t_in_s = d_in_s.toTime_t();
        din_s.setDateTime(t_in_s);
        toSave_s.setAttr("ДатаВремя отправки", din_s);

        QDateTime d_out_s = this->ui->dte_receivedate_s->dateTime();
        CDate dout_s;
        time_t t_out_s = d_out_s.toTime_t();
        dout_s.setDateTime(t_out_s);
        toSave_s.setAttr("ДатаВремя возврата", dout_s);

        ODS::IObject snd_s = ODS::OdsInterface::self()->iobjectManager().getIObjectById(this->senderMap.value(this->ui->cb_sender_s->currentText()));
        toSave_s.setAttr("Отправитель задание", snd_s);
        toSave_s.setAttr("Станция отправки", this->ui->cb_sender_s->currentText());
        ODS::IObject rcv_s = ODS::OdsInterface::self()->iobjectManager().getIObjectById(this->senderMap.value(this->ui->cb_receiver_s->currentText()));
        toSave_s.setAttr("Получатель задание", rcv_s);
        toSave_s.setAttr("Станция прибытия", this->ui->cb_receiver_s->currentText());
        toSave_s.setAttr("Транспорт", ODS::OdsInterface::self()->iobjectManager().getIObjectById(this->objectNameMap.value(this->ui->cb_object_name->currentText())));
    }
    else
    {
        this->fields_not_filled();
        ODS::OdsInterface::self()->dbManager().rollback();
        return;
    }

    toSave_s.update(ODS::IObject::NotRecursiveUpdate);
    adminGroup.setRightsToIObject(toSave_s, ODS::Right::All);
    userGroup.setRightsToIObject(toSave_s, ODS::Right::Read);
    toSave_r.update(ODS::IObject::NotRecursiveUpdate);
    adminGroup.setRightsToIObject(toSave_r, ODS::Right::All);
    userGroup.setRightsToIObject(toSave_r, ODS::Right::Read);

    ODS::OdsInterface::self()->dbManager().commit();
    if (updFlag==true){
        emit( msgToStatus("Дорожка обновлена") );
    } else {
        emit( msgToStatus("Дорожка сохранена") );
    }
//    this->parentWidget()->close();
}

void CreateShuttleTask::processAddToCheckPoints_S()
{
    QDateTime date;
    date.setDate(QDate::currentDate());
    QStandardItem *item = new QStandardItem();
    item->setData(date, Qt::DisplayRole);
    QStandardItem *item2 = new QStandardItem();
    item2->setData(date, Qt::DisplayRole);
    QList<QStandardItem*> list;
    list << new QStandardItem() << item << item2;
    this->model_checkpoints_s->insertRow(this->model_checkpoints_s->rowCount()-1, list);
}

void CreateShuttleTask::processAddToCheckPoints_R()
{
    QDateTime date;
    date.setDate(QDate::currentDate());
    QStandardItem *item = new QStandardItem();
    item->setData(date, Qt::DisplayRole);
    QStandardItem *item2 = new QStandardItem();
    item2->setData(date, Qt::DisplayRole);
    QList<QStandardItem*> list;
    list << new QStandardItem() << item << item2;
    this->model_checkpoints_r->insertRow(this->model_checkpoints_r->rowCount()-1, list);
}

void CreateShuttleTask::processAddToPaPnvt_S()
{
    this->model_pa_pnvt_s->addEmptyRow();
}

void CreateShuttleTask::processAddToPaPnvt_R()
{
    this->model_pa_pnvt_r->addEmptyRow();
}

void CreateShuttleTask::processAddToRespZone_S()
{
    QDateTime date;
    date.setDate(QDate::currentDate());
    QStandardItem *item = new QStandardItem();
    item->setData(date, Qt::DisplayRole);
    QStandardItem *item2 = new QStandardItem();
    item2->setData(date, Qt::DisplayRole);
    this->model_respzone_s->setItem(this->model_respzone_s->rowCount(), 1, item);
    this->model_respzone_s->setItem(this->model_respzone_s->rowCount()-1, 2, item2);
}

void CreateShuttleTask::processAddToRespZone_R()
{
    QDateTime date;
    date.setDate(QDate::currentDate());
    QStandardItem *item = new QStandardItem();
    item->setData(date, Qt::DisplayRole);
    QStandardItem *item2 = new QStandardItem();
    item2->setData(date, Qt::DisplayRole);
    this->model_respzone_r->setItem(this->model_respzone_r->rowCount(), 1, item);
    this->model_respzone_r->setItem(this->model_respzone_r->rowCount()-1, 2, item2);
}

void CreateShuttleTask::displayTransportType(const QString &item)
{
    ODS::IObject rasp = ODS::OdsInterface::self()->iobjectManager().getIObjectById(this->raspMap.value(item));
    ODS::IObject sod = rasp.getLink("Содержание");
    ODS::IObject sod_obj = ODS::OdsInterface::self()->iobjectManager().getIObjectById(sod.id());

    QString transport_type = sod_obj.getStringAttr("Вид транспорта");
    if (transport_type.count() > 0)
    {
        this->ui->le_transport_type->setText(transport_type);
        this->ui->le_transport_type->setVisible(true);
        this->ui->l_transport_type->setVisible(true);

        STAFF::StaffStructureManager staffMgr(ODS::OdsInterface::self());

        if (transport_type == "ЖД")
        {
            this->ui->cb_object_name->clear();
            STAFF::DivisionType::ptr tr_type = staffMgr.getDivisionTypeByName("Ж");

            STAFF::Division::ptrList list = staffMgr.getDivisionsByType(tr_type);

            this->ui->cb_object_name->addItem("");
            for (STAFF::Division::ptrList::iterator it = list.begin(); it != list.end(); ++it)
            {
                STAFF::Division::ptr cur = *it;
                this->objectNameMap.insert(cur->name(), cur->id());
                this->ui->cb_object_name->addItem(cur->name());
            }

            this->ui->cb_cat_type_s->setVisible(true);
            this->ui->cb_cat_type_r->setVisible(true);
            this->ui->l_cat_s->setVisible(true);
            this->ui->l_cat_r->setVisible(true);
            this->ui->le_transport_type->setText(transport_type);
        } else if (transport_type == "авто")
        {            
            this->ui->cb_object_name->clear();
            STAFF::DivisionType::ptr tr_type = staffMgr.getDivisionTypeByName("А");

            STAFF::Division::ptrList list = staffMgr.getDivisionsByType(tr_type);

            this->ui->cb_object_name->addItem("");
            for (STAFF::Division::ptrList::iterator it = list.begin(); it != list.end(); ++it)
            {
                STAFF::Division::ptr cur = *it;
                this->objectNameMap.insert(cur->name(), cur->id());
                this->ui->cb_object_name->addItem(cur->name());
            }

            this->ui->cb_cat_type_s->setVisible(true);
            this->ui->cb_cat_type_r->setVisible(true);
            this->ui->l_cat_s->setVisible(true);
            this->ui->l_cat_r->setVisible(true);
        }
    }
}

void CreateShuttleTask::fill_cb_receiver_s(const QString &item)
{
    IRS::CatalogManager catalogMgr(ODS::OdsInterface::self());
    IRS::Catalog::ptr catalog_units = catalogMgr.getCatalogByName("Классификатор воинских частей");

    this->ui->cb_receiver_s->addItem("");
    ODS::IORecordCursor cb_receiver = ODS::OdsInterface::self()->iobjectManager().exec("SELECT DISTINCT \"Получатель\" FROM \"Контроль перевозок\".\"Содержание таблица\"");
    for (ODS::IORecordCursor::iterator it = cb_receiver.begin(); it != cb_receiver.end(); ++it)
    {
        ODS::IORecord cur = *it;

        QString name;
        IRS::CatalogEntry::ptr c_entry = catalog_units->entryById(cur.getLink("Получатель").id());
        if (!c_entry.isNull() && (name != item))
        {
            name = c_entry->stringValue("Условное наименование");
            this->receiverMap.insert(name, cur.getLink("Получатель").id());
            this->ui->cb_receiver_s->addItem(name);
        }
    }
}

void CreateShuttleTask::update_return_le(const QString &item)
{
    this->ui->le_cb_sender_r->setText(item);
    this->ui->le_cb_receiver_r->setText(this->ui->cb_sender_s->currentText());

    int sender_s_id = this->senderMap.value(this->ui->cb_sender_s->currentText());
    int receiver_s_id = this->receiverMap.value(this->ui->cb_receiver_s->currentText());

    ODS::IORecordCursor send = ODS::OdsInterface::self()->iobjectManager().exec("SELECT * FROM \"Контроль перевозок\".\"Содержание таблица\" WHERE \"Отправитель\" = " + QString::number(sender_s_id) + " AND \"Получатель\" = " + QString::number(receiver_s_id));
    this->load_count_send = send.size();
    ODS::IORecordCursor ret = ODS::OdsInterface::self()->iobjectManager().exec("SELECT * FROM \"Контроль перевозок\".\"Содержание таблица\" WHERE \"Отправитель\" = " + QString::number(receiver_s_id) + " AND \"Получатель\" = " + QString::number(sender_s_id));
    this->load_count_return = ret.size();

    QDateTime date_r;
    date_r.setDate(QDate::currentDate());
    QStandardItem *itm_r = new QStandardItem();
    itm_r->setData(date_r, Qt::DisplayRole);
    QStandardItem *itm2_r = new QStandardItem();
    itm2_r->setData(date_r, Qt::DisplayRole);
    this->model_checkpoints_r->setItem(this->model_checkpoints_r->rowCount(), 1, itm_r);
    this->model_checkpoints_r->setItem(this->model_checkpoints_r->rowCount()-1, 2, itm2_r);

    QDateTime date_s;
    date_s.setDate(QDate::currentDate());
    QStandardItem *itm_s = new QStandardItem();
    itm_s->setData(date_s, Qt::DisplayRole);
    QStandardItem *itm2_s = new QStandardItem();
    itm2_s->setData(date_s, Qt::DisplayRole);
    this->model_checkpoints_s->setItem(this->model_checkpoints_s->rowCount(), 1, itm_s);
    this->model_checkpoints_s->setItem(this->model_checkpoints_s->rowCount()-1, 2, itm2_s);

    if (this->model_checkpoints_s->rowCount() == 2)
    {
        QStandardItem *item_b_s = new QStandardItem();
        item_b_s->setData(this->ui->cb_sender_s->currentText(), Qt::DisplayRole);
        QStandardItem *item_e_s = new QStandardItem();
        item_e_s->setData(this->ui->cb_receiver_s->currentText(), Qt::DisplayRole);

        QStandardItem *item_b_r = new QStandardItem();
        item_b_r->setData(this->ui->cb_sender_s->currentText(), Qt::DisplayRole);
        QStandardItem *item_e_r = new QStandardItem();
        item_e_r->setData(this->ui->cb_receiver_s->currentText(), Qt::DisplayRole);

        this->model_checkpoints_s->setItem(0, 0, item_b_s);
        this->model_checkpoints_s->setItem(1, 0, item_e_s);

        this->model_checkpoints_r->setItem(0, 0, item_e_r);
        this->model_checkpoints_r->setItem(1, 0, item_b_r);
    }

    if (this->load_count_send)
    {
        this->ui->l_no_load_s->setVisible(false);
    }
    else
    {
        this->ui->l_no_load_s->setVisible(true);
    }

    if (this->load_count_return)
    {
        this->ui->l_no_load_r->setVisible(false);
    }
    else
    {
        this->ui->l_no_load_r->setVisible(true);
    }
}

void CreateShuttleTask::clear_form(){
    qDebug()<<Q_FUNC_INFO;
    this->model_pa_pnvt_s->clear();
    this->model_respzone_s->clear();
    this->model_checkpoints_s->clear();
    qDebug()<<"before clear2";


    this->model_pa_pnvt_r->clear();
    this->model_respzone_r->clear();
    this->model_checkpoints_r->clear();

    this->model_pa_pnvt_s->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
    this->model_pa_pnvt_s->setHorizontalHeaderItem(1, new QStandardItem(QString("Номер ЖДЭ")));
    this->model_pa_pnvt_s->setHorizontalHeaderItem(2, new QStandardItem(QString("Наим. станц. п/а")));
    this->model_pa_pnvt_s->setHorizontalHeaderItem(3, new QStandardItem(QString("ПНВТ")));

    this->model_pa_pnvt_r->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
    this->model_pa_pnvt_r->setHorizontalHeaderItem(1, new QStandardItem(QString("Номер ЖДЭ")));
    this->model_pa_pnvt_r->setHorizontalHeaderItem(2, new QStandardItem(QString("Наим. станц. п/а")));
    this->model_pa_pnvt_r->setHorizontalHeaderItem(3, new QStandardItem(QString("ПНВТ")));

    this->model_respzone_s->setHorizontalHeaderItem(0, new QStandardItem(QString("Зона ответств.")));
    this->model_respzone_s->setHorizontalHeaderItem(1, new QStandardItem(QString("Вход")));
    this->model_respzone_s->setHorizontalHeaderItem(2, new QStandardItem(QString("Выход")));

    this->model_respzone_r->setHorizontalHeaderItem(0, new QStandardItem(QString("Зона ответств.")));
    this->model_respzone_r->setHorizontalHeaderItem(1, new QStandardItem(QString("Вход")));
    this->model_respzone_r->setHorizontalHeaderItem(2, new QStandardItem(QString("Выход")));

    this->model_checkpoints_s->setHorizontalHeaderItem(0, new QStandardItem(QString("Наименование")));
    this->model_checkpoints_s->setHorizontalHeaderItem(1, new QStandardItem(QString("Прибытие")));
    this->model_checkpoints_s->setHorizontalHeaderItem(2, new QStandardItem(QString("Убытие")));
    this->model_checkpoints_s->setHorizontalHeaderItem(3, new QStandardItem(QString("НВТ")));

    this->model_checkpoints_r->setHorizontalHeaderItem(0, new QStandardItem(QString("Наименование")));
    this->model_checkpoints_r->setHorizontalHeaderItem(1, new QStandardItem(QString("Прибытие")));
    this->model_checkpoints_r->setHorizontalHeaderItem(2, new QStandardItem(QString("Убытие")));
    this->model_checkpoints_r->setHorizontalHeaderItem(3, new QStandardItem(QString("НВТ")));

    this->ui->de_mzdate->setDate(QDate::currentDate());
    this->ui->dte_senddate_s->setDate(QDate::currentDate());
    this->ui->dte_senddate_r->setDate(QDate::currentDate());
    this->ui->dte_receivedate_s->setDate(QDate::currentDate());
    this->ui->dte_receivedate_r->setDate(QDate::currentDate());

    this->ui->cb_sender_s->disconnect();
    this->ui->cb_sender_s->setCurrentIndex(this->ui->cb_sender_s->findText(""));
    QObject::connect(this->ui->cb_sender_s, SIGNAL(currentIndexChanged(QString)), this, SLOT(fill_cb_receiver_s(QString)));

    ODS::IObjectCursor rasp_cursor = ODS::OdsInterface::self()->iobjectManager().getIObjectCursor("Контроль перевозок.Распоряжения");

    this->ui->cb_raspor->clear();
    this->ui->cb_raspor->addItem("");
    for (ODS::IObjectCursor::iterator it = rasp_cursor.begin(); it != rasp_cursor.end(); ++it)
    {
        ODS::IObject cur = *it;
        QString num = cur.getStringAttr("Номер");
        CDate date = cur.getDateTimeAttr("Дата");
        QDate qdate(date.getYear(), date.getMonthNum(), date.getMonthDay());
        num.append(" от " + qdate.toString("dd.MM.yyyy"));

        this->raspMap.insert(num, cur.id());
        this->ui->cb_raspor->addItem(num);
    }

    this->ui->cb_cat_type_s->setVisible(false);
    this->ui->cb_cat_type_r->setVisible(false);
    this->ui->l_cat_s->setVisible(false);
    this->ui->l_cat_r->setVisible(false);
    this->ui->le_transport_type->setVisible(false);
    this->ui->l_transport_type->setVisible(false);
    this->ui->l_no_load_s->setVisible(false);
    this->ui->l_no_load_r->setVisible(false);

    ODS::IORecordCursor mz_query = ODS::OdsInterface::self()->iobjectManager().exec("SELECT max(\"Контроль перевозок\".\"Маршрутное задание\".\"Номер задания\") as max_num FROM \"Контроль перевозок\".\"Маршрутное задание\"");
    int mz_num = 0;
    for (ODS::IORecordCursor::iterator it = mz_query.begin(); it != mz_query.end(); ++it)
    {
        ODS::IORecord cur = *it;

        mz_num = cur.getIntAttr("max_num");
    }

    if (mz_num == 0) {
        this->ui->sb_mznumber->setValue(1);
    } else {
        this->ui->sb_mznumber->setValue(mz_num+1);
    }

    this->ui->tabWidget->setCurrentIndex(0);

    this->ui->cb_object_name->clear();

    this->ui->le_cb_sender_r->clear();
    this->ui->le_sender_s->clear();
    this->ui->le_sender_r->clear();
    this->ui->cb_receiver_s->disconnect();
    this->ui->cb_receiver_s->clear();
    QObject::connect(this->ui->cb_receiver_s, SIGNAL(currentIndexChanged(QString)), this, SLOT(update_return_le(QString)));
    this->ui->le_receiver_s->clear();
    this->ui->le_cb_receiver_r->clear();
    this->ui->le_receiver_r->clear();
}

void CreateShuttleTask::changeMz(int mzNumb) {

    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"Номер задания\" = '"+QString::number(mzNumb)+"'";
    ODS::IObjectCursor cur = ODS::OdsInterface::self()->iobjectManager().getIObjectCursor("Контроль перевозок.Маршрутное задание", filter);

    for (ODS::IObjectCursor::iterator it = cur.begin(); it != cur.end(); ++it)
    {
        ODS::IObject iobj = *it;

        QDateTime datetime;
        datetime.setTime_t(iobj.getDateTimeAttr("Дата марш задания").getDateTime());
        QDate date = datetime.date();
        this->ui->de_mzdate->setDate(date);

        QString rasp = this->raspMap.key(iobj.getIntAttr("Распоряжение"));
        this->ui->cb_raspor->setCurrentIndex(this->ui->cb_raspor->findText(rasp));

        QString transpObj = this->objectNameMap.key(iobj.getIntAttr("Транспорт"));
        this->ui->cb_object_name->setCurrentIndex(this->ui->cb_object_name->findText(transpObj));

        int direction = iobj.getIntAttr("Направление");
        if ( direction == 1){
            this->ui->cb_cat_type_s->setCurrentIndex(this->ui->cb_cat_type_s->findText(iobj.getStringAttr("Категория")));
//            QString sender = senderMap.key(iobj.getIntAttr("Отправитель задание"));
            this->ui->cb_sender_s->setCurrentIndex(this->ui->cb_sender_s->findText(iobj.getStringAttr("Станция отправки")));
            datetime.setTime_t(iobj.getDateTimeAttr("ДатаВремя отправки").getDateTime());
            this->ui->dte_senddate_s->setDateTime(datetime);
            this->ui->cb_receiver_s->setCurrentIndex(this->ui->cb_receiver_s->findText(iobj.getStringAttr("Станция прибытия")));
            datetime.setTime_t(iobj.getDateTimeAttr("ДатаВремя возврата").getDateTime());
            this->ui->dte_receivedate_s->setDateTime(datetime);

            ODS::IObjectQueryFilter filter2;
            filter2.whereCondition = "\"Марш задание\" = '"+QString::number(iobj.id())+"'";

            this->model_pa_pnvt_s->clear();
            this->model_pa_pnvt_s->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
            this->model_pa_pnvt_s->setHorizontalHeaderItem(1, new QStandardItem(QString("Номер ЖДЭ")));
            this->model_pa_pnvt_s->setHorizontalHeaderItem(2, new QStandardItem(QString("Наим. станц. п/а")));
            this->model_pa_pnvt_s->setHorizontalHeaderItem(3, new QStandardItem(QString("НВТ")));

            ODS::IObjectCursor cur2 = ODS::OdsInterface::self()->iobjectManager().getIObjectCursor("Контроль перевозок.Станции ПА и ПНВТ", filter2);

            for (ODS::IObjectCursor::iterator it2 = cur2.begin(); it2 != cur2.end(); ++it2)
            {
                  ODS::IObject iobjPathCP = *it2;

                  QStandardItem *item = new QStandardItem();
                  item->setData(iobjPathCP.getIntAttr("Порядковый номер"), Qt::DisplayRole);
                  item->setData(iobjPathCP.id(), Qt::UserRole);
                  QStandardItem *item2 = new QStandardItem();
                  item2->setData(iobjPathCP.getStringAttr("Номер ЖДЭ"), Qt::DisplayRole);
                  QStandardItem *item3 = new QStandardItem();
                  item3->setData(iobjPathCP.getStringAttr("Станция ПА"), Qt::DisplayRole);
                  QStandardItem *item4 = new QStandardItem();
                  item4->setData(iobjPathCP.getStringAttr("ПНВТ"), Qt::DisplayRole);
                  QList<QStandardItem*> list;
                  list << item << item2<<item3<<item4;
                  this->model_pa_pnvt_s->appendRow(list);
            }

            this->model_respzone_s->clear();
            this->model_respzone_s->setHorizontalHeaderItem(0, new QStandardItem(QString("Зона ответств.")));
            this->model_respzone_s->setHorizontalHeaderItem(1, new QStandardItem(QString("Вход")));
            this->model_respzone_s->setHorizontalHeaderItem(2, new QStandardItem(QString("Выход")));

            cur2 = ODS::OdsInterface::self()->iobjectManager().getIObjectCursor("Контроль перевозок.Зоны ответственности", filter2);

            for (ODS::IObjectCursor::iterator it2 = cur2.begin(); it2 != cur2.end(); ++it2)
            {
                ODS::IObject iobj = *it2;

                QStandardItem *item = new QStandardItem();
                item->setData(iobj.getStringAttr("Наименование"), Qt::DisplayRole);
                item->setData(iobj.id(), Qt::UserRole);
                QDateTime date;
                date.setTime_t(iobj.getDateTimeAttr("Вход").getDateTime());
                QStandardItem *item2 = new QStandardItem();
                item2->setData(date, Qt::DisplayRole);
                date.setTime_t(iobj.getDateTimeAttr("Выход").getDateTime());
                QStandardItem *item3 = new QStandardItem();
                item3->setData(date, Qt::DisplayRole);
                QList<QStandardItem*> list;
                list << item << item2 << item3;
                this->model_respzone_s->appendRow(list);
            }

            this->model_checkpoints_s->clear();
            this->model_checkpoints_s->setHorizontalHeaderItem(0, new QStandardItem(QString("Наименование")));
            this->model_checkpoints_s->setHorizontalHeaderItem(1, new QStandardItem(QString("Прибытие")));
            this->model_checkpoints_s->setHorizontalHeaderItem(2, new QStandardItem(QString("Убытие")));
            this->model_checkpoints_s->setHorizontalHeaderItem(3, new QStandardItem(QString("НВТ")));

            cur2 = ODS::OdsInterface::self()->iobjectManager().getIObjectCursor("Контроль перевозок.Контрольные точки", filter2);

            for (ODS::IObjectCursor::iterator it2 = cur2.begin(); it2 != cur2.end(); ++it2)
            {
                ODS::IObject iobj = *it2;

                QStandardItem *item = new QStandardItem();
                item->setData(iobj.getStringAttr("Наименование"), Qt::DisplayRole);
                item->setData(iobj.id(), Qt::UserRole);
                QDateTime date;
                date.setTime_t(iobj.getDateTimeAttr("Прибытие").getDateTime());
                QStandardItem *item2 = new QStandardItem();
                item2->setData(date, Qt::DisplayRole);
                date.setTime_t(iobj.getDateTimeAttr("Убытие").getDateTime());
                QStandardItem *item3 = new QStandardItem();
                item3->setData(date, Qt::DisplayRole);
                QStandardItem *item4 = new QStandardItem();
                item4->setData(iobj.getStringAttr("НВТ"), Qt::DisplayRole);
                QList<QStandardItem*> list;
                list << item << item2 << item3 << item4;
                this->model_checkpoints_s->appendRow(list);
            }

        } else if (direction == 2){

            this->ui->cb_cat_type_r->setCurrentIndex(this->ui->cb_cat_type_r->findText(iobj.getStringAttr("Категория")));
//            QString sender = senderMap.key(iobj.getIntAttr("Отправитель задание"));
            this->ui->le_cb_sender_r->setText(iobj.getStringAttr("Станция отправки"));
            datetime.setTime_t(iobj.getDateTimeAttr("ДатаВремя отправки").getDateTime());
            this->ui->dte_senddate_r->setDateTime(datetime);
            this->ui->le_cb_receiver_r->setText(iobj.getStringAttr("Станция прибытия"));
            datetime.setTime_t(iobj.getDateTimeAttr("ДатаВремя возврата").getDateTime());
            this->ui->dte_receivedate_r->setDateTime(datetime);

            ODS::IObjectQueryFilter filter2;
            filter2.whereCondition = "\"Марш задание\" = '"+QString::number(iobj.id())+"'";

            this->model_pa_pnvt_r->clear();
            this->model_pa_pnvt_r->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
            this->model_pa_pnvt_r->setHorizontalHeaderItem(1, new QStandardItem(QString("Номер ЖДЭ")));
            this->model_pa_pnvt_r->setHorizontalHeaderItem(2, new QStandardItem(QString("Наим. станц. п/а")));
            this->model_pa_pnvt_r->setHorizontalHeaderItem(3, new QStandardItem(QString("НВТ")));

            ODS::IObjectCursor cur2 = ODS::OdsInterface::self()->iobjectManager().getIObjectCursor("Контроль перевозок.Станции ПА и ПНВТ", filter2);

            for (ODS::IObjectCursor::iterator it2 = cur2.begin(); it2 != cur2.end(); ++it2)
            {
                  ODS::IObject iobjPathCP = *it2;

                  QStandardItem *item = new QStandardItem();
                  item->setData(iobjPathCP.getIntAttr("Порядковый номер"), Qt::DisplayRole);
                  item->setData(iobjPathCP.id(), Qt::UserRole);
                  QStandardItem *item2 = new QStandardItem();
                  item2->setData(iobjPathCP.getStringAttr("Номер ЖДЭ"), Qt::DisplayRole);
                  QStandardItem *item3 = new QStandardItem();
                  item3->setData(iobjPathCP.getStringAttr("Станция ПА"), Qt::DisplayRole);
                  QStandardItem *item4 = new QStandardItem();
                  item4->setData(iobjPathCP.getStringAttr("ПНВТ"), Qt::DisplayRole);
                  QList<QStandardItem*> list;
                  list << item << item2<<item3<<item4;
                  this->model_pa_pnvt_r->appendRow(list);
            }

            this->model_respzone_r->clear();
            this->model_respzone_r->setHorizontalHeaderItem(0, new QStandardItem(QString("Зона ответств.")));
            this->model_respzone_r->setHorizontalHeaderItem(1, new QStandardItem(QString("Вход")));
            this->model_respzone_r->setHorizontalHeaderItem(2, new QStandardItem(QString("Выход")));

            cur2 = ODS::OdsInterface::self()->iobjectManager().getIObjectCursor("Контроль перевозок.Зоны ответственности", filter2);

            for (ODS::IObjectCursor::iterator it2 = cur2.begin(); it2 != cur2.end(); ++it2)
            {
                ODS::IObject iobj = *it2;

                QStandardItem *item = new QStandardItem();
                item->setData(iobj.getStringAttr("Наименование"), Qt::DisplayRole);
                item->setData(iobj.id(), Qt::UserRole);
                QDateTime date;
                date.setTime_t(iobj.getDateTimeAttr("Вход").getDateTime());
                QStandardItem *item2 = new QStandardItem();
                item2->setData(date, Qt::DisplayRole);
                date.setTime_t(iobj.getDateTimeAttr("Выход").getDateTime());
                QStandardItem *item3 = new QStandardItem();
                item3->setData(date, Qt::DisplayRole);
                QList<QStandardItem*> list;
                list << item << item2 << item3;
                this->model_respzone_r->appendRow(list);
            }

            this->model_checkpoints_r->clear();
            this->model_checkpoints_r->setHorizontalHeaderItem(0, new QStandardItem(QString("Наименование")));
            this->model_checkpoints_r->setHorizontalHeaderItem(1, new QStandardItem(QString("Прибытие")));
            this->model_checkpoints_r->setHorizontalHeaderItem(2, new QStandardItem(QString("Убытие")));
            this->model_checkpoints_r->setHorizontalHeaderItem(3, new QStandardItem(QString("НВТ")));

            cur2 = ODS::OdsInterface::self()->iobjectManager().getIObjectCursor("Контроль перевозок.Контрольные точки", filter2);

            for (ODS::IObjectCursor::iterator it2 = cur2.begin(); it2 != cur2.end(); ++it2)
            {
                ODS::IObject iobj = *it2;

                QStandardItem *item = new QStandardItem();
                item->setData(iobj.getStringAttr("Наименование"), Qt::DisplayRole);
                item->setData(iobj.id(), Qt::UserRole);
                QDateTime date;
                date.setTime_t(iobj.getDateTimeAttr("Прибытие").getDateTime());
                QStandardItem *item2 = new QStandardItem();
                item2->setData(date, Qt::DisplayRole);
                date.setTime_t(iobj.getDateTimeAttr("Убытие").getDateTime());
                QStandardItem *item3 = new QStandardItem();
                item3->setData(date, Qt::DisplayRole);
                QStandardItem *item4 = new QStandardItem();
                item4->setData(iobj.getStringAttr("НВТ"), Qt::DisplayRole);
                QList<QStandardItem*> list;
                list << item << item2 << item3 << item4;
                this->model_checkpoints_r->appendRow(list);
            }
        }
    }
}

//void CreateShuttleTask::setEditors() {

//    QComboBox* editor = new QComboBox();
//    editor->setEditable(true);
//    editor->setInsertPolicy(QComboBox::NoInsert);

//    QString querySQL = "SELECT \"Информационно-справочная подсистема\"."
//            "\"Классификатор ЖД станций\".\"Наименование\" as \"Наименование\" "
//            "FROM \"Информационно-справочная подсистема\".\"Классификатор ЖД станций\" "
//            "WHERE \"Информационно-справочная подсистема\"."
//            "\"Классификатор ЖД станций\".\"ПА\" = true";

//    ODS::IORecordCursor rec_cursor = ODS::OdsInterface::self()->iobjectManager().exec(querySQL);
//    for (ODS::IORecordCursor::iterator it = rec_cursor.begin(); it != rec_cursor.end(); ++it) {
//        ODS::IORecord iobj = *it;

//        QString numb = iobj.getStringAttr("Наименование");
//        editor->addItem(numb);
//    }

////    editor->setCurrentIndex(editor->findText(index.data().toString()));

//    return editor;
//}
