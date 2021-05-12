#include "claimform.h"
#include "ui_claimform.h"

#include "claimdelegate.h"
#include "OdsInterface"
#include "CatalogManager"
#include "QMessageBox"
#include "global_defs.h"
#include "OdsTemplateManager"
#include "QFileDialog"

QString ClaimForm::trType = "";

ClaimForm::ClaimForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClaimForm),
    model(0)
{
    ui->setupUi(this);
    //Форма заявки на спец перевозки
    init();
}

ClaimForm::~ClaimForm()
{
    delete model;
    delete ui;
}

void ClaimForm::init()
{
    initTv();
    initOther();

    connect(ui->lwProjects, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(loadProj(QListWidgetItem*)));
    connect(ui->pbNewProject, SIGNAL(released()), this, SLOT(newProj()));
    connect(ui->pbSave, SIGNAL(released()), this, SLOT(save()));
    connect(ui->pbExit, SIGNAL(released()), this, SLOT(close()));
    connect(ui->pbPrint, SIGNAL(released()), this, SLOT(print()));
    connect(ui->te, SIGNAL(cursorPositionChanged()), this, SLOT(clearTe()));
    connect(ui->cbTransportType, SIGNAL(currentIndexChanged(QString)), this, SLOT(trTypeChange(QString)));
    connect(ui->pbAdd, SIGNAL(released()), this, SLOT(add()));
    connect(ui->pbDel, SIGNAL(released()), this, SLOT(del()));

    trTypeChange("жд");
}

void ClaimForm::initTv()
{
    model = new ClaimModel(projectId, ui->tv);

    model->setColumnCount(14);
    model->setHorizontalHeaderItem(0,new QStandardItem(QString("№")));
    model->setHorizontalHeaderItem(1,new QStandardItem(QString("Отправитель")));
    model->setHorizontalHeaderItem(2,new QStandardItem(QString("Станция\n(пункт,порт,аэродром,)\nотправления")));
    model->setHorizontalHeaderItem(3,new QStandardItem(QString("Дата и время\nподачи вагонов")));
    model->setHorizontalHeaderItem(4,new QStandardItem(QString("Тип вагонов(автомобилей,\nморского/речного/воздушного\nсудна)")));
    model->setHorizontalHeaderItem(5,new QStandardItem(QString("Количество")));
    model->setHorizontalHeaderItem(6,new QStandardItem(QString("Номера вагонов(автомобилей,\nморского/речного/воздушного\nсудна)")));
    model->setHorizontalHeaderItem(7,new QStandardItem(QString("Дата и время\nотправления")));
    model->setHorizontalHeaderItem(8,new QStandardItem(QString("Станция и дорога\nназначения(пункт, порт,\nаэродром)")));
    model->setHorizontalHeaderItem(9,new QStandardItem(QString("Получатель")));
    model->setHorizontalHeaderItem(10,new QStandardItem(QString("Офицеры")));
    model->setHorizontalHeaderItem(11,new QStandardItem(QString("Прапорщики")));
    model->setHorizontalHeaderItem(12,new QStandardItem(QString("Сержанты")));
    model->setHorizontalHeaderItem(13,new QStandardItem(QString("Солдаты")));

    ui->tv->setModel(model);

    ui->tv->setItemDelegate(new ClaimDelegate(ui->tv));

    ui->tv->resizeColumnsToContents();
    ui->tv->verticalHeader()->setVisible(false);
#if QT_VERSION >= 0x050000
    ui->tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tv->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
#else
    ui->tv->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tv->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#endif
}

void ClaimForm::initOther()
{
    ui->te->setTextColor(Qt::gray);
    ui->te->setText("(Дополнительные условия)");

    ui->lwProjects->setSelectionMode(QAbstractItemView::SingleSelection);
    ODS::IObjectQueryFilter filter;
    filter.orderCondition = "\"Наименование проекта\" ASC";
    ODS::IObjectCursor cursor = ODS::OdsInterface::self()->iobjectManager().getIObjectCursor("Контроль перевозок.Заявки",filter);

    ODS::IObjectCursor::iterator it = cursor.begin();
    while (it != cursor.end())
    {
        ODS::IObject obj = *it;
        QListWidgetItem* item = new QListWidgetItem(obj.getStringAttr("Наименование проекта"),ui->lwProjects);
        item->setData(6,obj.id());
        ++it;
    }

    QStringList list;
    list<<"жд"<<"авто"<<"авиа"<<"морской";
    ui->cbTransportType->addItems(list);

    list.clear();
    list<<"КП-1"<<"КП-2"<<"КП-3"<<"КП-4";
    ui->cbTransportCategory->addItems(list);

    IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
    IRS::Catalog::ptr catalog = catalMgr.getCatalogByName("Классификатор аварийных карточек");
    if (catalog) {
        IRS::CatalogEntry::ptrList entries = catalog->entries();
        for (IRS::CatalogEntry::ptrList::iterator it = entries.begin(); it != entries.end(); ++it){
            IRS::CatalogEntry::ptr entry = (*it);
            ui->cbEmergencyCard->addItem(entry->stringValue("Номер"),QVariant(entry->id()));
        }
    } else {
        QMessageBox::critical(0,"Ошибка","Не найден классификатор аварийных карточек");
    }

    catalog = catalMgr.getCatalogByName("Классификатор воинских частей");
    if (catalog) {
        IRS::CatalogEntry::ptrList entries = catalog->entries();
        for (IRS::CatalogEntry::ptrList::iterator it = entries.begin(); it != entries.end(); ++it){
            IRS::CatalogEntry::ptr entry = (*it);
            ui->cbTransportResponsible->addItem(entry->stringValue("Условное наименование"),QVariant(entry->id()));
        }
    } else {
        QMessageBox::critical(0,"Ошибка","Не найден классификатор воинских частей");
    }

    catalog = catalMgr.getCatalogByName("Классификатор предприятий промышленности");
    if (catalog) {
        IRS::CatalogEntry::ptrList entries = catalog->entries();
        for (IRS::CatalogEntry::ptrList::iterator it = entries.begin(); it != entries.end(); ++it){
            IRS::CatalogEntry::ptr entry = (*it);
            ui->cbTransportResponsible->addItem(entry->stringValue("Сокращенное наименование"),QVariant(entry->id()));
        }
    } else {
        QMessageBox::critical(0,"Ошибка","Не найден классификатор предприятий промышленности");
    }

    ui->de->setCalendarPopup(true);
    ui->de->setDate(QDate::currentDate());
}

void ClaimForm::clearForm()
{
    ui->cbTransportType->setCurrentIndex(0);
    ui->cbTransportCategory->setCurrentIndex(0);
    ui->cbEmergencyCard->setCurrentIndex(0);
    ui->cbTransportResponsible->setCurrentIndex(0);
    ui->leNumber->clear();
    ui->de->setDate(QDate::currentDate());
    ui->leSign->clear();
    ui->te->setTextColor(Qt::gray);
    ui->te->setText("(Дополнительные условия)");
    model->clear();
    model->setColumnCount(14);
    model->setHorizontalHeaderItem(0,new QStandardItem(QString("№")));
    model->setHorizontalHeaderItem(1,new QStandardItem(QString("Отправитель")));
    model->setHorizontalHeaderItem(2,new QStandardItem(QString("Станция\nотправления")));
    model->setHorizontalHeaderItem(3,new QStandardItem(QString("Дата и время\nподачи вагонов")));
    model->setHorizontalHeaderItem(4,new QStandardItem(QString("Тип вагонов\n(автомобилей)")));
    model->setHorizontalHeaderItem(5,new QStandardItem(QString("Количество")));
    model->setHorizontalHeaderItem(6,new QStandardItem(QString("Номера вагонов\n(автомобилей)")));
    model->setHorizontalHeaderItem(7,new QStandardItem(QString("Дата и время\nотправления")));
    model->setHorizontalHeaderItem(8,new QStandardItem(QString("Станция и дорога\nназначения")));
    model->setHorizontalHeaderItem(9,new QStandardItem(QString("Получатель")));
    model->setHorizontalHeaderItem(10,new QStandardItem(QString("Офицеры")));
    model->setHorizontalHeaderItem(11,new QStandardItem(QString("Прапорщики")));
    model->setHorizontalHeaderItem(12,new QStandardItem(QString("Сержанты")));
    model->setHorizontalHeaderItem(13,new QStandardItem(QString("Солдаты")));
}

void ClaimForm::clearTe()
{
    if (ui->te->toPlainText() == "(Дополнительные условия)"){
        ui->te->clear();
        ui->te->setTextColor(Qt::black);
    }
}

void ClaimForm::loadProj(QListWidgetItem* lwItem)
{
    clearForm();
    projectId = lwItem->data(6).toInt();

    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();
    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"io_id\" ="+QString::number(projectId);
    ODS::IObjectCursor cur = ioMan.getIObjectCursor("Контроль перевозок.Заявки",filter);

    for (ODS::IObjectCursor::iterator It = cur.begin(); It != cur.end(); ++It){
        ODS::IObject iobj = (*It);

        qDebug()<<iobj.getStringAttr("Вид транспорта");
        ui->cbTransportType->setCurrentIndex(ui->cbTransportType->findText(iobj.getStringAttr("Вид транспорта")));
        trTypeChange(iobj.getStringAttr("Вид транспорта"));
        ui->cbTransportCategory->setCurrentIndex(ui->cbTransportCategory->findText(iobj.getStringAttr("Категория перевозки")));
        ui->cbEmergencyCard->setCurrentIndex(ui->cbEmergencyCard->findData(iobj.getIntAttr("Аварийная карточка")));
        ui->cbTransportResponsible->setCurrentIndex(ui->cbTransportResponsible->findData(iobj.getIntAttr("Ответственный")));
        ui->leNumber->setText(iobj.getStringAttr("Номер заявки"));
        ui->leSign->setText(iobj.getStringAttr("Подпись"));
        ui->te->setText(iobj.getStringAttr("Доп инфо"));
        QDateTime dt;
        dt.setTime_t(iobj.getDateTimeAttr("Дата заявки").getDateTime());
        ui->de->setDate(dt.date());
    }

    model->changeProj(projectId);
    model->load();

}

void ClaimForm::newProj()
{
    int listItemsCount = ui->lwProjects->count()+1;
    QListWidgetItem* item = new QListWidgetItem("Проект"+QString::number(listItemsCount),ui->lwProjects);
    item->setData(6,0);
    projectId = 0;
    clearForm();
    model->changeProj(0);

}

void ClaimForm::add()
{
    model->addEmptyRow();
}

void ClaimForm::del()
{
    QModelIndex index = ui->tv->currentIndex();
    if (!index.isValid())
        return;
    model->remove(index);
}

void ClaimForm::save()
{
    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();
    ODS::OdsInterface::self()->dbManager().transaction();
    ODS::OdsInterface::self()->syncManager().syncTransaction();

    if (projectId == 0){

        ODS::IObject iobj = ioMan.createIObject("Контроль перевозок.Заявки");
        if (ui->te->toPlainText() == "(Дополнительные условия)"){
            iobj.setAttr("Доп инфо", "");
        } else {
            iobj.setAttr("Доп инфо", ui->te->toPlainText());
        }
        iobj.setAttr("Наименование проекта", ui->lwProjects->currentItem()->text());
        iobj.setAttr("Вид транспорта", ui->cbTransportType->currentText());
        iobj.setAttr("Категория перевозки", ui->cbTransportCategory->currentText());
        iobj.setAttr("Аварийная карточка", ui->cbEmergencyCard->itemData(ui->cbEmergencyCard->currentIndex()).toInt());
        iobj.setAttr("Ответственный", ui->cbTransportResponsible->itemData(ui->cbTransportResponsible->currentIndex()).toInt());
        iobj.setAttr("Подпись", ui->leSign->text());
        iobj.setAttr("Номер заявки", ui->leNumber->text());
        QDateTime datet;
        datet.setDate(ui->de->date());
        CDate cdate;
        cdate.setDateTime(datet.toTime_t());
        iobj.setAttr("Дата заявки", cdate);

        if (!iobj.save(ODS::IObject::NotRecursiveSave)){
            ODS::OdsInterface::self()->dbManager().rollback();
            QMessageBox::critical(0, "Ошибка", "Неудачная поппытка созранения записи");
            qDebug()<<"Ошибка при сохраниении данных в бд";
            return;
        } else {
            if (setIObjRights(iobj)) {
                ODS::OdsInterface::self()->dbManager().commit();
                msgToStatus("Заявка создана");
                projectId = iobj.id();
            }
        }

    } else {

        ODS::IObject iobj = ioMan.getIObjectById(projectId);
        if (ui->te->toPlainText() == "(Дополнительные условия)"){
            iobj.setAttr("Доп инфо", "");
        } else {
            iobj.setAttr("Доп инфо", ui->te->toPlainText());
        }
        iobj.setAttr("Наименование проекта", ui->lwProjects->currentItem()->text());
        iobj.setAttr("Вид транспорта", ui->cbTransportType->currentText());
        iobj.setAttr("Категория перевозки", ui->cbTransportCategory->currentText());
        iobj.setAttr("Аварийная карточка", ui->cbEmergencyCard->itemData(ui->cbEmergencyCard->currentIndex()).toInt());
        iobj.setAttr("Ответственный", ui->cbTransportResponsible->itemData(ui->cbTransportResponsible->currentIndex()).toInt());
        iobj.setAttr("Подпись", ui->leSign->text());
        iobj.setAttr("Номер заявки", ui->leNumber->text());
        QDateTime datet;
        datet.setDate(ui->de->date());
        CDate cdate;
        cdate.setDateTime(datet.toTime_t());
        iobj.setAttr("Дата заявки", cdate);

        if (!iobj.update(ODS::IObject::NotRecursiveUpdate)){
            ODS::OdsInterface::self()->dbManager().rollback();
            QMessageBox::critical(0, "Ошибка", "Неудачная попытка обновления записи");
            qDebug()<<"Ошибка при обновлении данных в бд";
            return;
        } else {
            msgToStatus("Заявка обновлена");
        }

    }

    model->changeProj(projectId);
    if (!model->save()){
        QMessageBox::critical(this, "Ошибка", "Не удалось сохранить записи");
    }
}

void ClaimForm::print()
{
    if (projectId == 0){
        QMessageBox::warning(0, "Внимание", "Выберите проект из списка или сохраните текущий");
        return;
    }

    OdsTemplateManager tmpMan = OdsTemplateManager(ODS::OdsInterface::self());
    bool ok;
    OdsTemplate templ = tmpMan.getTemplate("Заявка на спецперевозку", &ok);

    if (!ok){
        QString str = "Не выполнена загрузка шаблона: Заявка на спецперевозку";
        QMessageBox::critical(0,"Ошибка", str);
        emit msgToStatus(str);
        return;
    }

    if (!templ.isValid()){
        QString str = "Недействительный шаблон: Заявка на спецперевозку";
        QMessageBox::critical(0,"Ошибка", str);
        emit msgToStatus(str);
        return;
    }

    templ.insertText("transport1", ui->cbTransportType->currentText());
    templ.insertText("transport2", ui->cbTransportType->currentText());
    templ.insertText("date", ui->de->date().toString("dd.MM.yyyy"));
    templ.insertText("respon", ui->cbTransportResponsible->currentText());
    templ.insertText("category", ui->cbTransportCategory->currentText());
    templ.insertText("emergency", ui->cbEmergencyCard->currentText());
    templ.insertText("sign", ui->leSign->text());

    WordTable wordTable(0, 14);

    for (int i = 0; i != model->rowCount(); ++i){

        wordTable.appendRow();

        wordTable.addText(wordTable.TRCount(), 1, model->item(i,0)->text());
        wordTable.addText(wordTable.TRCount(), 2, model->item(i,1)->text());
        wordTable.addText(wordTable.TRCount(), 3, model->item(i,2)->text());
        wordTable.addText(wordTable.TRCount(), 4, model->item(i,3)->text());
        wordTable.addText(wordTable.TRCount(), 5, model->item(i,4)->text());
        wordTable.addText(wordTable.TRCount(), 6, model->item(i,5)->text());
        wordTable.addText(wordTable.TRCount(), 7, model->item(i,6)->text());
        wordTable.addText(wordTable.TRCount(), 8, model->item(i,7)->text());
        wordTable.addText(wordTable.TRCount(), 9, model->item(i,8)->text());
        wordTable.addText(wordTable.TRCount(), 10, model->item(i,9)->text());
        wordTable.addText(wordTable.TRCount(), 11, model->item(i,10)->text());
        wordTable.addText(wordTable.TRCount(), 12, model->item(i,11)->text());
        wordTable.addText(wordTable.TRCount(), 13, model->item(i,12)->text());
        wordTable.addText(wordTable.TRCount(), 14, model->item(i,13)->text());
    }

    templ.insertTableData("table", &wordTable);

    QString fileName = QFileDialog::getSaveFileName(0, "Сохранить в файл", "Заявка на спецперевозку", "Docx Files (*.docx)");

    if (fileName.isEmpty()){
        emit msgToStatus("Документ не сохранен");
        return;
    }

    if (!templ.saveTemplate(fileName, WordProcessingDocument::DocX)){
        QMessageBox::critical(0,"Ошибка", "Ошибка при сохранении документа!");
        emit msgToStatus("Ошибка при сохранении документа!");
        return;
    } else {
        emit msgToStatus("Документ сохранен");
    }
}

void ClaimForm::close()
{
    this->parentWidget()->close();
}
