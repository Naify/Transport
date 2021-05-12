#include "OrderForm/orderform.h"
#include "ui_orderform.h"
#include "global_defs.h"

#include <QtGlobal>
#include <OdsInterface>
#include <QMessageBox>
#include <OdsTemplateManager>
#include <CatalogManager.h>
#include <QFileDialog>

OrderForm::OrderForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderForm)
{
    ui->setupUi(this);

    ui->tableWidget_journal->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_journal->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_order->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_order->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableWidget_journal->verticalHeader()->setVisible(false);
    ui->tableWidget_order->verticalHeader()->setVisible(false);

    ui->tableWidget_order->setColumnWidth(0, 70);
    ui->tableWidget_order->setColumnWidth(3, 120);
    ui->tableWidget_order->setColumnWidth(5, 70);
    ui->tableWidget_order->setColumnWidth(6, 50);
    ui->tableWidget_order->setColumnWidth(7, 95);

#if QT_VERSION >= 0x050000
    ui->tableWidget_order->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_order->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableWidget_order->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->tableWidget_order->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);
    ui->tableWidget_order->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Fixed);
    ui->tableWidget_order->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Fixed);
#else
    ui->tableWidget_order->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidget_order->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    ui->tableWidget_order->horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);
    ui->tableWidget_order->horizontalHeader()->setResizeMode(5, QHeaderView::Fixed);
    ui->tableWidget_order->horizontalHeader()->setResizeMode(6, QHeaderView::Fixed);
    ui->tableWidget_order->horizontalHeader()->setResizeMode(7, QHeaderView::Fixed);
#endif

    ui->comboBox_transport->addItem(" ");
    ui->comboBox_transport->addItem("ЖД");
    ui->comboBox_transport->addItem("авто");
    ui->comboBox_transport->addItem("морской");
    ui->comboBox_transport->addItem("авиа");

    ui->comboBox_responsib->addItem(" ", -1);
    IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
    IRS::Catalog::ptr catalog_divs = catalMgr.getCatalogByName("Классификатор воинских частей");

    if (catalog_divs) {
        IRS::CatalogEntry::ptrList divs_list = catalog_divs->entries();
        for (IRS::CatalogEntry::ptrList::iterator it = divs_list.begin(); it != divs_list.end(); ++it){
            IRS::CatalogEntry::ptr div = (*it);
            QString sender_string = div->stringValue("Условное наименование");
            ui->comboBox_responsib->addItem(sender_string, div->id());
        }
    } else {
        emit(msgToStatus("Не удалось получить словарь 'Классификатор воинских частей'"));
    }

    ui->dateEdit->setDate(QDate::currentDate());

    connect(ui->tableWidget_journal, SIGNAL(cellClicked(int,int)), SLOT(journalClicked(int, int)));
    connect(ui->pushButton_exit, SIGNAL(released()), SLOT(closeForm()));
    connect(ui->pushButton_add_order, SIGNAL(released()), SLOT(addOrder()));
    connect(ui->pushButton_content, SIGNAL(released()), SLOT(addContentTable()));
    connect(ui->pushButton_save, SIGNAL(released()), SLOT(addContent()));
    connect(ui->pushButton_del, SIGNAL(released()), SLOT(delContentTable()));
    connect(ui->pushButton_print, SIGNAL(released()), SLOT(makeWord()));


    renewTableJournal();
}

int OrderForm::current_order_id = 0;
int OrderForm::row_count = 0;

OrderForm::~OrderForm()
{
    delete ui;
}

void OrderForm::closeForm()
{
    this->parentWidget()->close();
}

void OrderForm::renewTableJournal()
{
    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();

    ODS::IObjectCursor cursor =  ioMan.getIObjects(JOURNAL_TYPE);

    ui->tableWidget_journal->setRowCount(0);

    for (ODS::IObjectCursor::iterator iobjIt = cursor.begin(); iobjIt != cursor.end(); ++iobjIt ){
        ODS::IObject iobj = (*iobjIt);

        ui->tableWidget_journal->insertRow(ui->tableWidget_journal->rowCount());

        ui->tableWidget_journal->setItem(ui->tableWidget_journal->rowCount() - 1, 0, new QTableWidgetItem);
        QString number = iobj.getStringAttr(NUMBER_ATTR);
        ui->tableWidget_journal->item(ui->tableWidget_journal->rowCount() - 1, 0)->setText(number);
        ui->tableWidget_journal->item(ui->tableWidget_journal->rowCount() - 1, 0)->setData(Qt::UserRole, iobj.id());

        ui->tableWidget_journal->setItem(ui->tableWidget_journal->rowCount() - 1, 1, new QTableWidgetItem);
        QDateTime datetime;
        datetime.setTime_t(iobj.getDateTimeAttr(DATE_ATTR).getDateTime());
        ui->tableWidget_journal->item(ui->tableWidget_journal->rowCount() - 1, 1)->setText(datetime.toString("dd.MM.yyyy"));
    }

    ui->tableWidget_journal->setEditTriggers(QAbstractItemView::NoEditTriggers);

//    if (!ui->tableWidget_journal->size().isEmpty())
//        ui->tableWidget_journal->selectRow(0);
}

void OrderForm::renewTableOrders()
{    
    ui->tableWidget_order->setRowCount(0);

    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();

    ODS::IObjectQueryFilter filter;
    filter.whereCondition = QString ("%1 = %2").arg(ORDER_ATTR, QString::number(current_order_id));
    filter.orderCondition = "\""+NUMBER_P_P_ATTR+"\" ASC";

    ODS::IObjectCursor cursor =  ioMan.getIObjects(ORDER_TABLE_TYPE, filter);

//    if (cursor.size() == 0) qDebug()<<"Не найдено содержание таблицы распоряжения";

    for (ODS::IObjectCursor::iterator iobjIt = cursor.begin(); iobjIt != cursor.end(); ++iobjIt ){
        ODS::IObject iobj = (*iobjIt);

        ui->tableWidget_order->insertRow(ui->tableWidget_order->rowCount());

        int new_row = ui->tableWidget_order->rowCount() - 1;

        ui->tableWidget_order->setItem(new_row,0, new QTableWidgetItem());
        ui->tableWidget_order->item(new_row,0)->setText(QString::number(iobj.getIntAttr(NUMBER_P_P_ATTR)));
        ui->tableWidget_order->item(new_row,0)->setData(Qt::UserRole, iobj.id());

        ui->tableWidget_order->setItem(new_row,1, new QTableWidgetItem());
        int sender = iobj.getIntAttr(SENDER_ATTR);
        QString sender_string = getSenderName(sender);
        ui->tableWidget_order->item(new_row,1)->setText(sender_string);

        ui->tableWidget_order->setItem(new_row,2, new QTableWidgetItem());
        sender = iobj.getIntAttr(RECEIVER_ATTR);
        QString string_receiver = getSenderName(sender);
        ui->tableWidget_order->item(new_row,2)->setText(string_receiver);

        ui->tableWidget_order->setItem(new_row,3, new QTableWidgetItem());
        QDateTime datetime;
        datetime.setTime_t(iobj.getDateTimeAttr(DATE_SEND_ATTR).getDateTime());
        ui->tableWidget_order->item(new_row,3)->setText(datetime.toString(DATE_FORMAT));

        ui->tableWidget_order->setItem(new_row,4, new QTableWidgetItem());
        int cargo = iobj.getIntAttr(CARGO_NAME_ATTR);
        QString string_cargo = getCargoName(cargo);
        ui->tableWidget_order->item(new_row,4)->setText(string_cargo);

        ui->tableWidget_order->setItem(new_row,5, new QTableWidgetItem());
        ui->tableWidget_order->item(new_row,5)->setText(iobj.getStringAttr(AMOUNT_ATTR));

        ui->tableWidget_order->setItem(new_row,6, new QTableWidgetItem());
        ui->tableWidget_order->item(new_row,6)->setText(iobj.getStringAttr(READYNESS_STATUS_ATTR));

        ui->tableWidget_order->setItem(new_row,7, new QTableWidgetItem());
        ui->tableWidget_order->item(new_row,7)->setText(iobj.getStringAttr(BLOCK_ATTR));
    }

    ui->tableWidget_order->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //---------------------------------------------------------------------------

    int index = ui->comboBox_responsib->findText(QString(" "));
    ui->comboBox_responsib->setCurrentIndex(index);
    index = ui->comboBox_transport->findText(QString(" "));
    ui->comboBox_transport->setCurrentIndex(index);
    ui->textEdit_spec_orders->setText("");

    ODS::IObject order_iobj = ioMan.getIObjectById(current_order_id);
    int content_iobj_id = 0;
    content_iobj_id = order_iobj.getIntAttr(CONTENT_ATTR);

    ODS::IObject iobj = ioMan.getIObjectById(content_iobj_id);

//    if (content_iobj_id == 0) qDebug()<<"Не найдено содержание распоряжения";

   index = ui->comboBox_transport->findText(iobj.getStringAttr(TYPE_TRANSPORT_ATTR));
   ui->comboBox_transport->setCurrentIndex(index);

    int responsib = iobj.getIntAttr(RESPONSIBLE_ATTR);
    index = ui->comboBox_responsib->findData(responsib);
    ui->comboBox_responsib->setCurrentIndex(index);

    ui->textEdit_spec_orders->setText(iobj.getStringAttr(SPECIAL_ORDERS_ATTR));

    row_count = ui->tableWidget_order->rowCount();
}

void OrderForm::journalClicked(int row, int column)
{
    (void)column;
    bool ok;
    int id = ui->tableWidget_journal->item(row, 0)->data(Qt::UserRole).toInt(&ok);
    if (ok) {
        current_order_id = id;
        renewTableOrders();
//        ui->pushButton_print->setEnabled(true);
    } else {
        emit(msgToStatus("Не удалось получить id распоряжения"));
    }
}

void OrderForm::addOrder() {

    if (ui->lineEdit_number->text() == ""){
        QMessageBox::critical(0,"Ошибка", "Заполните поле с номером нового распоряжения");
    } else if (ui->dateEdit->text() == ""){
        QMessageBox::critical(0,"Ошибка", "Заполните поле с датой нового распоряжения");
    } else {

        ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();

        ODS::OdsInterface::self()->dbManager().transaction();
        ODS::OdsInterface::self()->syncManager().syncTransaction();

        ODS::IObject iobj = ioMan.createIObject(JOURNAL_TYPE);

        if (!iobj.setAttr(NUMBER_ATTR, ui->lineEdit_number->text())){
            emit(msgToStatus("Не удалось задать занчение атрибута "+NUMBER_ATTR));
            return;
        }

        QDateTime dateTime = ui->dateEdit->dateTime();
        CDate change_date;
        change_date.setDateTime(dateTime.toTime_t());
        if (!iobj.setAttr(DATE_ATTR, change_date)){
            emit(msgToStatus("Не удалось задать занчение атрибута "+DATE_ATTR));
            return;
        }

        if  (iobj.save(ODS::IObject::NotRecursiveSave)) {
            if (setIObjRights(iobj)) {
                ODS::OdsInterface::self()->dbManager().commit();
                renewTableJournal();
                ui->lineEdit_number->clear();
                ui->dateEdit->setSpecialValueText( " ");
                ui->dateEdit->setDate( QDate::fromString("01/01/2000", "dd/MM/yyyy"));
            } else {
                ODS::OdsInterface::self()->dbManager().rollback();
            }
        } else {
            ODS::OdsInterface::self()->dbManager().rollback();
            emit(msgToStatus("Не удалось создать распоряжение"));
        }

        ui->dateEdit->setDate(QDate::currentDate());

    }
}

void OrderForm::addContentTable()
{
    emit( openAddForm() );
}

void OrderForm::addContent()
{
    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();

    if (current_order_id == 0) {
        QMessageBox::critical(0,"Ошибка", "Выберите распоряжение для сохранения в него информации");
        return;
    }

    ODS::IObject order_iobj = ioMan.getIObjectById(current_order_id);
    int content_iobj_id = 0;
    content_iobj_id = order_iobj.getIntAttr(CONTENT_ATTR);

    ODS::OdsInterface::self()->dbManager().transaction();

    ODS::OdsInterface::self()->syncManager().syncTransaction();

    ODS::IObject content_iobj;
    if (content_iobj_id == 0){
        content_iobj = ioMan.createIObject(ORDER_TYPE);
    } else {
        content_iobj = ioMan.getIObjectById(content_iobj_id);
    }

    QString transport = ui->comboBox_transport->itemText(ui->comboBox_transport->currentIndex());
    content_iobj.setAttr(TYPE_TRANSPORT_ATTR,transport);

    int responsible = ui->comboBox_responsib->itemData(ui->comboBox_responsib->currentIndex()).toInt();
    content_iobj.setAttr(RESPONSIBLE_ATTR,responsible);

    QString spec_orders = ui->textEdit_spec_orders->toPlainText();
    content_iobj.setAttr(SPECIAL_ORDERS_ATTR,spec_orders);

    if (content_iobj_id == 0){
        if (!content_iobj.save(ODS::IObject::NotRecursiveSave)){
            ODS::OdsInterface::self()->dbManager().rollback();
            emit(msgToStatus("Ошибка при сохраниении данных в бд"));
        } else {
            order_iobj.setAttr(CONTENT_ATTR, content_iobj.id());
            if (!order_iobj.update(ODS::IObject::NotRecursiveUpdate)){
                ODS::OdsInterface::self()->dbManager().rollback();
                emit(msgToStatus("Ошибка при сохраниении данных в бд"));
            } else {
                if (setIObjRights(content_iobj)) {
                    ODS::OdsInterface::self()->dbManager().commit();
                    emit(msgToStatus("Данные сохранены"));
                } else {
                    ODS::OdsInterface::self()->dbManager().rollback();
                    emit(msgToStatus("Ошибка при задании прав"));
                }
            }
        }
    } else {
        if (!content_iobj.update(ODS::IObject::NotRecursiveUpdate)){
            ODS::OdsInterface::self()->dbManager().rollback();
            emit(msgToStatus("Ошибка при сохраниении данных в бд"));
        }
        else {
            ODS::OdsInterface::self()->dbManager().commit();
            emit(msgToStatus("Данные обновлены"));
        }
    }
}

void OrderForm::delContentTable()
{
    QList<QTableWidgetItem*> items_list = ui->tableWidget_order->selectedItems();

    if ( items_list.size() == 0)
    {
        QMessageBox::information(0, "Внимание","Выберите ряд");
        return;
    }
    else
    {
        int id = items_list.at(0)->data( Qt::UserRole ).toInt();

        ODS::OdsInterface::self()->dbManager().transaction();
        ODS::OdsInterface::self()->syncManager().syncTransaction();

        if ( !ODS::OdsInterface::self()->iobjectManager().deleteIObjectById( id ) )
        {
            ODS::OdsInterface::self()->dbManager().rollback();
            emit(msgToStatus("Ошибка при удалении данных из бд"));
        } else {
            int numb = items_list.at(0)->text().toInt();
            ui->tableWidget_order->removeRow( items_list.at(0)->row() );
            row_count = ui->tableWidget_order->rowCount();

            if (row_count != 0){
                for (int i = 0; i < row_count; ++i){
                    QTableWidgetItem * item = ui->tableWidget_order->item(i,0);
                    int new_number = item->text().toInt() - 1;
                    if (item->text().toInt() > numb){
                        item->setText(QString::number(new_number));

                        int change_id = item->data(Qt::UserRole).toInt();
                        ODS::IObject iobj = ODS::OdsInterface::self()->iobjectManager().getIObjectById(change_id);
                        iobj.setAttr(NUMBER_P_P_ATTR,new_number);
                        iobj.update(ODS::IObject::NotRecursiveUpdate);
                        ODS::OdsInterface::self()->dbManager().commit();
                    };
                }
            }
        }
    }
}

QString OrderForm::getSenderName(int sender){

    QString sender_string;

    IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
    IRS::Catalog::ptr catalog_divs = catalMgr.getCatalogByName("Классификатор воинских частей");

    if (catalog_divs)
    {
        IRS::CatalogEntry::ptrList divs_list = catalog_divs->entries();
        for (IRS::CatalogEntry::ptrList::iterator it = divs_list.begin(); it != divs_list.end(); ++it){
            IRS::CatalogEntry::ptr div = (*it);
            if ( div->id() == sender){
                sender_string = div->stringValue("Условное наименование");
            }
        }
    } else {
        emit(msgToStatus("Не удалось получить словарь 'Классификатор воинских частей'"));
    }

    return sender_string;
}

QString OrderForm::getCargoName(int cargo){
    QString string_cargo;

    IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
    IRS::Catalog::ptr catalog_units = catalMgr.getCatalogByName("Классификатор грузов");

    if (!catalog_units.isNull())
    {
        IRS::CatalogEntry::ptrList unit_list = catalog_units->entries();
        for (IRS::CatalogEntry::ptrList::iterator it = unit_list.begin(); it != unit_list.end(); ++it){
            IRS::CatalogEntry::ptr unit = (*it);
            if ( unit->id() == cargo){
                string_cargo = unit->stringValue("Наименование");
            }
        }
    } else {
        emit(msgToStatus("Не удалось получить словарь 'Классификатор грузов'"));
    }

    return string_cargo;
}

void OrderForm::makeWord()
{
    OdsTemplateManager tmpMan = OdsTemplateManager(ODS::OdsInterface::self());
    bool ok = false;
    OdsTemplate templ = tmpMan.getTemplate("Распоряжение", &ok);
    if (!ok)
    {
        emit(msgToStatus("Не выполнена загрузка шаблона!"));
        qDebug()<<"Не выполнена загрузка шаблона!";
        return;
    }

    if (!templ.isValid())
    {
        emit(msgToStatus("Недействительный шаблон!"));
        qDebug()<<"Недействительный шаблон!";
        return;
    }

    templ.insertText("trType",ui->comboBox_transport->itemText(ui->comboBox_transport->currentIndex()));
    templ.insertText("respon",ui->comboBox_responsib->itemText(ui->comboBox_responsib->currentIndex()));
    templ.insertText("specOrder",ui->textEdit_spec_orders->toPlainText());
    templ.insertText("sign1", "");
    templ.insertText("sign2", "");

    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();

    ODS::IObjectQueryFilter filter;
    filter.whereCondition = QString ("%1 = %2").arg(ORDER_ATTR, QString::number(current_order_id));
    filter.orderCondition = "\""+NUMBER_P_P_ATTR+"\" ASC";

    ODS::IObjectCursor cursor =  ioMan.getIObjects(ORDER_TABLE_TYPE, filter);

//    WordTable wordTable(cursor, false);
    WordTable wordTable (0, 8);

    for (ODS::IObjectCursor::iterator iobjIt = cursor.begin(); iobjIt != cursor.end(); ++iobjIt){
        ODS::IObject iobj = (*iobjIt);

        wordTable.appendRow();

        wordTable.addText(wordTable.TRCount(), 1, QString::number(iobj.getIntAttr(NUMBER_P_P_ATTR)));

        wordTable.addText(wordTable.TRCount(), 2, getSenderName(iobj.getIntAttr(SENDER_ATTR)));

        wordTable.addText(wordTable.TRCount(), 3, getSenderName(iobj.getIntAttr(RECEIVER_ATTR)));

        CDate date = iobj.getDateTimeAttr(DATE_SEND_ATTR);
        QDateTime dt = QDateTime::fromTime_t(date.getDateTime());

        wordTable.addText(wordTable.TRCount(), 4, dt.date().toString("dd.MM.yy"));

        wordTable.addText(wordTable.TRCount(), 5, getCargoName(iobj.getIntAttr(CARGO_NAME_ATTR)));

        wordTable.addText(wordTable.TRCount(), 6, QString::number(iobj.getIntAttr(AMOUNT_ATTR)));

        wordTable.addText(wordTable.TRCount(), 7, iobj.getStringAttr(READYNESS_STATUS_ATTR));

        wordTable.addText(wordTable.TRCount(), 8, iobj.getStringAttr(BLOCK_ATTR));

    }

    templ.insertTableData("table", &wordTable);

    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить в файл", "Распоряжение", "Docx Files (*.docx)");

    if (fileName == ""){
        emit(msgToStatus("Документ не сохранен"));
        return;
    }

    if (!templ.saveTemplate(fileName, WordProcessingDocument::DocX)) {
        emit(msgToStatus("Ошибка при сохранении!"));
        return;
    } else {
        emit(msgToStatus("Документ сохранен"));
    }

}
