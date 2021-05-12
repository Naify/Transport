#include "OrderForm/addcontentform.h"
#include "ui_addcontentform.h"
#include "global_defs.h"
#include "OrderForm/orderform.h"

#include <QComboBox>
#include <QLineEdit>
#include <QDateEdit>
#include <OdsInterface>
#include <QMessageBox>
#include <CatalogManager.h>

AddContentForm::AddContentForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddContentForm)
{
    ui->setupUi(this);

    ui->tableWidget->verticalHeader()->setVisible(false);

    ui->tableWidget->setColumnWidth(0, 70);
    ui->tableWidget->setColumnWidth(3, 120);
    ui->tableWidget->setColumnWidth(5, 70);
    ui->tableWidget->setColumnWidth(6, 50);
    ui->tableWidget->setColumnWidth(7, 90);

#if QT_VERSION >= 0x050000
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Fixed);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#else
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setResizeMode(5, QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setResizeMode(6, QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setResizeMode(7, QHeaderView::Fixed);
    ui->tableWidget->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#endif

    ui->tableWidget->setItem(0,0, new QTableWidgetItem());
    ui->tableWidget->item(0,0)->setText(QString::number(OrderForm::get_row_count()+1));
    ui->tableWidget->setItem(0,5, new QTableWidgetItem());

    QComboBox * combo_box_vc_enterprise = new QComboBox(this);

    QComboBox * combo_box_vc_enterprise2 = new QComboBox(this);

    IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
    IRS::Catalog::ptr catalog_divs = catalMgr.getCatalogByName("Классификатор воинских частей");

    if (catalog_divs)
    {
        IRS::CatalogEntry::ptrList divs_list = catalog_divs->entries();
        for (IRS::CatalogEntry::ptrList::iterator it = divs_list.begin(); it != divs_list.end(); ++it){
            IRS::CatalogEntry::ptr div = (*it);
            combo_box_vc_enterprise->addItem(div->stringValue("Условное наименование"),QVariant(div->id()));
            combo_box_vc_enterprise2->addItem(div->stringValue("Условное наименование"), QVariant(div->id()));
        }
    } else {
        emit(msgToStatus("Не удалось получить словарь 'Классификатор воинских частей'"));
    }
    ui->tableWidget->setCellWidget(0, 1, combo_box_vc_enterprise);
    ui->tableWidget->setCellWidget(0, 2, combo_box_vc_enterprise2);

    QDateEdit * dateEdit = new QDateEdit(this);
    dateEdit->setDisplayFormat("dd.MM.yyyy");
    dateEdit->setCalendarPopup(true);
    dateEdit->setDate(QDate::currentDate());
    ui->tableWidget->setCellWidget(0, 3, dateEdit);

    QComboBox * combo_box_cargo = new QComboBox(this);

    IRS::Catalog::ptr catalog_units = catalMgr.getCatalogByName("Классификатор грузов");

    if (!catalog_units.isNull())
    {
        IRS::CatalogEntry::ptrList unit_list = catalog_units->entries();
        for (IRS::CatalogEntry::ptrList::iterator it = unit_list.begin(); it != unit_list.end(); ++it){
            IRS::CatalogEntry::ptr unit = (*it);
            combo_box_cargo->addItem(unit->stringValue("Наименование"),QVariant(unit->id()));
        }
    } else {
        emit(msgToStatus("Не удалось получить словарь 'Классификатор грузов'"));
    }
    ui->tableWidget->setCellWidget(0, 4, combo_box_cargo);

    QComboBox * combo_ready = new QComboBox(this);
    combo_ready->setEditable(true);
    combo_ready->lineEdit()->setReadOnly(true);
    combo_ready->lineEdit()->setAlignment(Qt::AlignHCenter);
    combo_ready->addItem(QString(" "));
    combo_ready->addItem(QString("5"));
    combo_ready->addItem(QString("5-0"));
    combo_ready->addItem(QString("5-H"));
    combo_ready->addItem(QString("5-П"));
    ui->tableWidget->setCellWidget(0, 6, combo_ready);

    QComboBox * combo_bloc = new QComboBox(this);
    combo_bloc->setEditable(true);
    combo_bloc->lineEdit()->setReadOnly(true);
    combo_bloc->lineEdit()->setAlignment(Qt::AlignHCenter);
    combo_bloc->addItem(QString(" "));
    combo_bloc->addItem(QString("9(T)"));
    combo_bloc->addItem(QString("9(Б)"));
    combo_bloc->addItem(QString("9(Б)-ФА"));
    combo_bloc->addItem(QString("9(Б)-ДА"));
    ui->tableWidget->setCellWidget(0, 7, combo_bloc);

    connect(ui->pushButton_exit, SIGNAL(released()), this, SLOT(closeForm()));
    connect(ui->pushButton_save, SIGNAL(released()), this, SLOT(handleSaveButton()));
}

AddContentForm::~AddContentForm()
{
    delete ui;
}

void AddContentForm::closeForm()
{
    this->parentWidget()->close();
}

void AddContentForm::handleSaveButton()
{
    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();

    ODS::OdsInterface::self()->dbManager().transaction();

    ODS::OdsInterface::self()->syncManager().syncTransaction();

    ODS::IObject iobj= ioMan.createIObject(ORDER_TABLE_TYPE);


    if (ui->tableWidget->item(0, 0)->text() == ""){
        QMessageBox::warning(0,"Ошибка", "Введите значение в поле "+NUMBER_P_P_ATTR);
        return;
    }
    bool ok;
    int num = ui->tableWidget->item(0, 0)->text().toInt(&ok);
    if (!ok){
        QMessageBox::warning(0,"Ошибка", "Введите целое число в поле"+NUMBER_P_P_ATTR);
        return;
    }

    if (!iobj.setAttr(NUMBER_P_P_ATTR, num)){
        emit(msgToStatus("Не удалось задать значение для аттрибута "+NUMBER_P_P_ATTR));
        return;
    }

    QComboBox *combo_sender = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(0, 1));
    int id = combo_sender->itemData(combo_sender->currentIndex()).toInt();

    if (!iobj.setAttr(SENDER_ATTR, id)) {
        emit(msgToStatus("Не удалось задать значение для аттрибута "+SENDER_ATTR));
        return;
    }

    QComboBox *combo_receiver = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(0, 2));
    id = combo_receiver->itemData(combo_receiver->currentIndex()).toInt();

    if (!iobj.setAttr(RECEIVER_ATTR, id)){
        emit(msgToStatus("Не удалось задать значение для аттрибута "+RECEIVER_ATTR));
        return;
    }

    QDateEdit * dateTimeEdit = qobject_cast<QDateEdit*>(ui->tableWidget->cellWidget(0, 3));
    if (dateTimeEdit)
    {
        QDateTime dateTime = dateTimeEdit->dateTime();
        CDate change_date;
        if (!change_date.setDateTime(dateTime.toTime_t()))
            qDebug()<<"cdate not set";
        if (!iobj.setAttr(DATE_SEND_ATTR, change_date)){
            emit(msgToStatus("Не удалось задать значение для аттрибута "+DATE_SEND_ATTR));
            return;
        }

    }

    QComboBox *combo_cargo = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(0, 4));
    id = combo_cargo->itemData(combo_cargo->currentIndex()).toInt();

    //костыль пока нет разметки или ОШС
//    if (string == "BH175-01") {
//        fake_id = 1;
//    } else if (string == "BH175-02"){
//        fake_id = 2;
//    } else if (string == "BH175-03"){
//        fake_id = 3;
//    } else if (string == "KM298-01"){
//        fake_id = 4;
//    } else if (string == "KM298-02"){
//        fake_id = 5;
//    } else if (string == "TУ472"){
//        fake_id = 6;
//    } else if (string == "АЛ56"){
//        fake_id = 7;
//    } else if (string == "РБ085-01"){
//        fake_id = 8;
//    } else if (string == "РБ085-02"){
//        fake_id = 9;
//    } else if (string == "РБ085-03"){
//        fake_id = 10;
//    }

    if (!iobj.setAttr(CARGO_NAME_ATTR, id)){
        emit(msgToStatus("Не удалось задать значение для аттрибута "+CARGO_NAME_ATTR));
        return;
    }

    if (ui->tableWidget->item(0, 5)->text() == ""){
        QMessageBox::warning(0,"Ошибка", "Введите значение в поле "+AMOUNT_ATTR);
        return;
    }
    num = ui->tableWidget->item(0, 5)->text().toInt(&ok);
    if (!ok){
        QMessageBox::warning(0,"Ошибка", "Введите целое число в поле"+AMOUNT_ATTR);
        return;
    }
    if (!iobj.setAttr(AMOUNT_ATTR, num)){
        emit(msgToStatus("Не удалось задать значение для аттрибута "+AMOUNT_ATTR));
        return;
    }

    QWidget * item_widget = ui->tableWidget->cellWidget(0, 6);
    QList <QLineEdit *>linedit_list = item_widget->findChildren<QLineEdit *>();
    QLineEdit * lineedit = linedit_list.at(0);
    if (!iobj.setAttr(READYNESS_STATUS_ATTR, lineedit->text())){
        emit(msgToStatus("Не удалось задать значение для аттрибута "+READYNESS_STATUS_ATTR));
        return;
    }

    item_widget = ui->tableWidget->cellWidget(0, 7);
    linedit_list = item_widget->findChildren<QLineEdit *>();
    lineedit = linedit_list.at(0);
    if (!iobj.setAttr(BLOCK_ATTR, lineedit->text())){
        emit(msgToStatus("Не удалось задать значение для аттрибута "+BLOCK_ATTR));
        return;
    }

    if (OrderForm::get_current_order_id()==0){
        QMessageBox::warning(0," ", "Выберите распоряжение из журнала для добавления этого содержания");
        return;
    } else {
        iobj.setAttr(ORDER_ATTR, OrderForm::get_current_order_id());
    }

    if (!iobj.save(ODS::IObject::NotRecursiveSave)) {
        ODS::OdsInterface::self()->dbManager().rollback();
        emit(msgToStatus("Ошибка при сохраниении данных в бд"));
    } else {
        if (setIObjRights(iobj)) {
            ODS::OdsInterface::self()->dbManager().commit();
            this->parentWidget()->close();
            emit ( row_save() );
            emit(msgToStatus("Данные сохранены"));
        } else {
            ODS::OdsInterface::self()->dbManager().rollback();
        }
    }
}
