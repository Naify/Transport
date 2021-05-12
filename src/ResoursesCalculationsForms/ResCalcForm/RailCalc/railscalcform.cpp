#include "railscalcform.h"
#include "ui_railscalcform.h"
#include "global_defs.h"

#include "QComboBox"
#include "OdsInterface"
#include "QMessageBox"

RailsCalcForm::RailsCalcForm(int transitId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RailsCalcForm),
    currentTrId(transitId)
{
    ui->setupUi(this);
    init();
}

RailsCalcForm::~RailsCalcForm()
{
    delete ui;
}

void RailsCalcForm::init()
{
    model = new QStandardItemModel(this);
    ui->tv->setModel(model);

    initTv();
    initOther();

    load();

    connect(ui->pbCalc, SIGNAL(released()), this, SLOT(calc()));
    connect(ui->pbSave, SIGNAL(released()), this, SLOT(save()));
    connect(ui->pbExit, SIGNAL(released()), this, SLOT(close()));
}

void RailsCalcForm::initTv()
{
    model->clear();

    model->setHorizontalHeaderItem(0,new QStandardItem(QString("№")));
    model->setHorizontalHeaderItem(1,new QStandardItem(QString("Кол-во СИ")));
    model->setHorizontalHeaderItem(2,new QStandardItem(QString("Тип ТЗУ")));
    model->setHorizontalHeaderItem(3,new QStandardItem(QString("Кол-во ТЗУ")));
    model->setHorizontalHeaderItem(4,new QStandardItem(QString("Тип вагона")));
    model->setHorizontalHeaderItem(5,new QStandardItem(QString("Кол-во вагонов")));

    ui->tv->resizeColumnsToContents();    
    ui->tv->verticalHeader()->setVisible(false);
#if QT_VERSION >= 0x050000
    ui->tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tv->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
#else
    ui->tv->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tv->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#endif
    ui->tv->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tv->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tv->setSelectionMode(QAbstractItemView::SingleSelection);
}

void RailsCalcForm::initOther()
{
    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();
    ODS::IObject trIobj = ioMan.getIObjectById(currentTrId);
    kp = trIobj.getStringAttr("Категория");
    ODS::IObject cargoIobj = ioMan.getIObjectById(trIobj.getIntAttr("Содержание распоряжения"));
    ODS::IObject cargoNameIobj =ioMan.getIObjectById(cargoIobj.getIntAttr("Наименование груза"));
    QString cargoName = cargoNameIobj.getStringAttr("Наименование");

    ui->leSiType->setText(cargoName);
    siTypeId = cargoNameIobj.id();
    siCount = cargoIobj.getIntAttr("Кол-во");
    ui->leSiCount->setText(QString::number(siCount));

    ui->leSiType->setReadOnly(true);
    ui->leSiCount->setReadOnly(true);

    ui->cbContainerType->addItem("В ТЗУ(СК)", QVariant(1));
    ui->cbContainerType->addItem("Без ТЗУ(СК)", QVariant(0));

    ui->cbRailsTerm->addItem("Зима", QVariant(0));
    ui->cbRailsTerm->addItem("Лето", QVariant(1));
}

void RailsCalcForm::load()
{
    //загружает уже произведенный расчет если он у перевозки был сохранен

    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();
    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"Перевозка\" ="+QString::number(currentTrId);    
    ODS::IObjectCursor cur =  ioMan.getIObjects("Определение сил и средств.Вагоны определение", filter);

    IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
    IRS::Catalog::ptr catalogTzuNames = catalMgr.getCatalogByName("Классификатор контейнеров");
    if (catalogTzuNames.isNull()){
        QMessageBox::critical(0,"Ошибка", "Не найден классификатор"
                              "\"Классификатор контейнеров\""
                              " или не заданы права");
        return;
    }

    IRS::Catalog::ptr catalogRailNames = catalMgr.getCatalogByName("Классификатор вагонов по типам");
    if (catalogRailNames.isNull()){
        QMessageBox::critical(0,"Ошибка", "Не найден классификатор"
                              "\"Нормы загрузки вагонов без контейнеров\""
                              " или не заданы права");
        return;
    }

    if (cur.size()){
        for (ODS::IObjectCursor::iterator it = cur.begin(); it != cur.end(); ++it){
            ODS::IObject iobj = *it;

            bool railTerm = iobj.getBoolAttr("Температурный режим");
            bool containerType = iobj.getBoolAttr("Средства защиты");
            ui->cbRailsTerm->setCurrentIndex(ui->cbRailsTerm->findData(railTerm));
            ui->cbContainerType->setCurrentIndex(ui->cbContainerType->findData(containerType));
            ui->leAllRailCount->setText(QString::number(iobj.getIntAttr("Количество вагонов")));

            filter.whereCondition = "\"Определение\" ="+QString::number(iobj.id());
            filter.orderCondition = "\"Порядковый номер\" DESC";
            ODS::IObjectCursor cur2 =  ioMan.getIObjects("Определение сил и средств.Вагоны определение таблица", filter);

            for (ODS::IObjectCursor::iterator it = cur2.begin(); it != cur2.end(); ++it){
                ODS::IObject iobjTable = *it;

                makeRow(iobjTable.getIntAttr("Количество СИ"),
                        catalogTzuNames->entryById(iobjTable.getIntAttr("Тип контейнера"))->stringValue("Наименование"),
                        iobjTable.getIntAttr("Тип контейнера"),
                        iobjTable.getIntAttr("Количество контейнеров"),
                        catalogRailNames->entryById(iobjTable.getIntAttr("Тип вагона"))->stringValue("Тип вагона"),
                        iobjTable.getIntAttr("Тип вагона"),
                        iobjTable.getIntAttr("Количество вагонов"));
            }
        }
    }
}

void RailsCalcForm::calc()
{
    initTv();

    if (ui->leSiCount->text() == "" || ui->leSiType->text() == ""){
        QMessageBox::warning(0,"Ошибка", "Нет данных в полях 'Тип СИ' или 'Количество СИ'");
        return;
    }

    ui->leAllRailCount->setText("0");

    int containerType = ui->cbContainerType->itemData(ui->cbContainerType->currentIndex()).toInt();

    int RailCount = 0;

    if (!containerType){
        RailCount = calcWithoutContainer();
    } else  {
        RailCount = calcWithContainer();
    }

    if (RailCount){
        if (kp == "КП-1")
            RailCount += 9;
        else
            RailCount += 5;

        ui->leAllRailCount->setText(QString::number(RailCount));
    }
}

int RailsCalcForm::calcWithoutContainer()
{
    int railType = ui->cbRailsTerm->itemData(ui->cbRailsTerm->currentIndex()).toInt();

    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();
    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"Готовность\" = 'TRUE' AND \"Отсутствует\" = 'FALSE'";
    ODS::IObjectCursor cur =  ioMan.getIObjects("Определение сил и средств.Спец вагоны", filter);

    if (!cur.size()){
        QMessageBox::warning(0,"Внимание","Не найдены готовые и присутствующие вагоны");
        return 0;
    }

    //получение типов имеющихся вагонов и их количество
    QList<int> railsWeHave;
    QHash<int,int> railsAndCount;
    for (ODS::IObjectCursor::iterator it = cur.begin(); it != cur.end(); ++it){
        ODS::IObject iobj = *it;
        int railId = iobj.getIntAttr("Обозначение");
        QString railName = ioMan.getIObjectById(railId).getStringAttr("Тип вагона");
        if (!railType){
            if (!railsWeHave.contains(railId) && railName != "ВГ-124"){
                railsWeHave.append(railId);
                railsAndCount.insert(railId, 1);
            } else if (railsWeHave.contains(railId) && railName != "ВГ-124") {
                int count = railsAndCount.value(railId);
                railsAndCount.insert(railId, count+1);
            }
        } else {
            if (!railsWeHave.contains(railId)){
                railsWeHave.append(railId);
                railsAndCount.insert(railId, 1);
            } else if (railsWeHave.contains(railId)) {
                int count = railsAndCount.value(railId);
                railsAndCount.insert(railId, count+1);
            }
        }
    }

    IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
    IRS::Catalog::ptr catalogNorms = catalMgr.getCatalogByName("Нормы загрузки вагонов без контейнеров");
    if (catalogNorms.isNull()){
        QMessageBox::critical(0,"Ошибка", "Не найден классификатор"
                              "\"Нормы загрузки вагонов без контейнеров\""
                              " или не заданы права");
        return 0;
    }
    if (!catalogNorms->createCache()){
        QMessageBox::critical(0,"Ошибка", "Неудачное кэширование классификатора"
                              "\"Нормы загрузки вагонов без контейнеров\"");
        return 0;
    }
//        if (!catalogNorms->createIndex(QStringList()<<"Тип СИ"<<"Тип вагона")){
//            QMessageBox::critical(0,"Ошибка", "Неудачное индексирование классификатора"
//                                  "\"Нормы загрузки вагонов без контейнеров\"");
//            return;
//        }
    QHash<QString, QString> filterIrs;
    filterIrs.insert("Тип СИ", QString::number(siTypeId));

    //создаем список из id вагонов и их вместимости
    QList<int> capacityInNorms;
    QHash<int, int> railsIdsCapacity;
    for (QList<int>::iterator it = railsWeHave.begin(); it != railsWeHave.end(); ++it){
        int railId = *it;

        filterIrs.insert("Тип вагона", QString::number(railId));
        IRS::CatalogEntry::ptrList list = catalogNorms->entries(filterIrs);
        if (list.isEmpty()){
            continue;
        }
        int capacity = searchForValueIRS(list);
        capacityInNorms.append(capacity);
        railsIdsCapacity.insert(capacity, railId);
    }

    if (railsIdsCapacity.isEmpty()){
        QMessageBox::critical(0,"Ошибка", "В нормах загрузки "
                              "\"Нормы загрузки вагонов без контейнеров\"не найдены "
                              "данные по загрузке\n заданным типом СИ \""+ui->leSiType->text()+
                              "\" и доступными типами вагонов");
        return 0;
    }

    qStableSort(capacityInNorms.begin(), capacityInNorms.end(), qGreater<int>());

    IRS::Catalog::ptr catalog = catalMgr.getCatalogByName("Классификатор вагонов по типам");
    if (catalog.isNull()){
        QMessageBox::critical(0,"Ошибка", "Не найден классификатор"
                              "\"Нормы загрузки вагонов без контейнеров\""
                              " или не заданы права");
        return 0;
    }

    //начинаем считать
    int resultRails = 0;
    int siRemain = siCount;
    for (QList<int>::iterator it = capacityInNorms.begin(); it != capacityInNorms.end(); ++it){
        int capacity = *it;

        int railsNeeded = siRemain/capacity;
        int remain = siRemain%capacity;

        int railsThatCapacity = railsAndCount.value(railsIdsCapacity.value(capacity));

        if (remain){
             railsNeeded += 1;
        }

        IRS::CatalogEntry::ptr entry = catalog->entryById(railsIdsCapacity.value(capacity));

        if (railsNeeded <= railsThatCapacity){
            //если хвататет вагонов

            makeRow(siRemain, QString("-"), 0, 0, entry->stringValue("Тип вагона"), 0, railsNeeded);
            resultRails += railsNeeded;
            siRemain = siRemain - capacity*railsNeeded ;

            //если нет остатка то возвращаем кол-во вагонов всего и выходим
            //если есть то еще одна итерация и грузим остаток
            if (siRemain <= 0){
                return resultRails;
            }
        } else {
            //если не хватает вагонов

            makeRow(capacity*railsThatCapacity, QString("-"), 0, 0, entry->stringValue("Тип вагона"), 0, railsThatCapacity);
            resultRails += railsThatCapacity;
            siRemain = siRemain - capacity*railsThatCapacity;
        }
    }

    if (siRemain>0){
        initTv();
        QMessageBox::warning(0,"Внимание", "Не хвататет готовых вагонов");
        return 0;
    }
    return 0;
}

int RailsCalcForm::calcWithContainer()
{
    int railType = ui->cbRailsTerm->itemData(ui->cbRailsTerm->currentIndex()).toInt();

    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();
    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"Готовность\" = 'TRUE' AND \"Отсутствует\" = 'FALSE'";
    ODS::IObjectCursor cur =  ioMan.getIObjects("Определение сил и средств.ТЗУ и СК", filter);

    if (!cur.size()){
        QMessageBox::warning(0,"Внимание","Не найдены готовые и присутствующие ТЗУ и СК");
        return 0;
    }

    //получение типов имеющихся тзу и их количество
    QList<int> tzuWeHave;
    QHash<int,int> tzuAndCount;
    for (ODS::IObjectCursor::iterator it = cur.begin(); it != cur.end(); ++it){
        ODS::IObject iobj = *it;
        int tzuId = iobj.getIntAttr("Обозначение");
        if (!tzuWeHave.contains(tzuId)) {
            tzuWeHave.append(tzuId);
            tzuAndCount.insert(tzuId, 1);
        } else if (tzuWeHave.contains(tzuId)) {
            int count = tzuAndCount.value(tzuId);
            tzuAndCount.insert(tzuId, count+1);
        }
    }

    IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
    IRS::Catalog::ptr catalogTzuNorms = catalMgr.getCatalogByName("Нормы загрузки контейнеров");
    if (catalogTzuNorms.isNull()){
        QMessageBox::critical(0,"Ошибка", "Не найден классификатор"
                              "\"Нормы загрузки контейнеров\""
                              " или не заданы права");
        return 0;
    }
    if (!catalogTzuNorms->createCache()){
        QMessageBox::critical(0,"Ошибка", "Неудачное кэширование классификатора"
                              "\"Нормы загрузки контейнеров\"");
        return 0;
    }
//        if (!catalogTzuNorms->createIndex(QStringList()<<"Тип СИ"<<"Тип вагона")){
//            QMessageBox::critical(0,"Ошибка", "Неудачное индексирование классификатора"
//                                  "\"Нормы загрузки контейнеров\"");
//            return;
//        }
    QHash<QString, QString> filterIrs;
    filterIrs.insert("Тип СИ", QString::number(siTypeId));

    //создаем список из id тзу которые есть и их вместимости
    QList<int> capacityInNorms;
    QHash<int, int> tzuIdsCapacity;
    for (QList<int>::iterator it = tzuWeHave.begin(); it != tzuWeHave.end(); ++it){
        int tzuId = *it;

        filterIrs.insert("Тип контейнера", QString::number(tzuId));
        IRS::CatalogEntry::ptrList list = catalogTzuNorms->entries(filterIrs);
        if (list.isEmpty()){
            continue;
        }
        int capacity = searchForValueIRS(list);
        capacityInNorms.append(capacity);
//        qDebug()<<"capacityInNorms"<<capacity;
        tzuIdsCapacity.insert(tzuId, capacity);
//        qDebug()<<"tzuIdsCapacity"<<tzuId<<"capacity"<<capacity;
    }

    if (tzuIdsCapacity.isEmpty()){
        QMessageBox::critical(0,"Ошибка", "В нормах загрузки "
                              "\"Нормы загрузки контейнеров\" не найдены "
                              "данные по загрузке\n заданным типом СИ \""+ui->leSiType->text()+
                              "\" и доступными типами контейнеров");
        return 0;
    }

    QHash<int, int> tzuIdsCapacity2;

    tzuIdsCapacity2 = tzuIdsCapacity;

    qStableSort(capacityInNorms.begin(), capacityInNorms.end(), qGreater<int>());

    //начинаем считать тзу
    QHash<int, int> tzuResult;
    int siRemain = siCount;
    for (QList<int>::iterator it = capacityInNorms.begin(); it != capacityInNorms.end(); ++it){
        int capacity = *it;

        int tzuNeeded = siRemain/capacity;
        int remain = siRemain%capacity;

        int tzuId = tzuIdsCapacity.key(capacity);

        int tzuThatCapacity = tzuAndCount.value(tzuId);

        if (remain){
             tzuNeeded += 1;
        }

        if (tzuNeeded <= tzuThatCapacity){
            //если хвататет тзу

            tzuResult.insert(tzuId, tzuNeeded);
            siRemain = siRemain - capacity*tzuNeeded;

            tzuAndCount.remove(tzuId);
            tzuIdsCapacity.remove(tzuId);
            //если нет остатка то считаем вагоны
            //если есть то еще одна итерация и грузим остаток
            if (siRemain <= 0){
                break;
            }
        } else {
            //если не хватает тзу

            tzuResult.insert(tzuId, tzuThatCapacity);
            siRemain = siRemain - capacity*tzuThatCapacity;
            tzuAndCount.remove(tzuId);
            tzuIdsCapacity.remove(tzuId);
        }
    }

    if (siRemain>0){
        initTv();
        QMessageBox::warning(0,"Внимание", "Не хвататет готовых тзу");
        return 0;
    }

    filter.whereCondition = "\"Готовность\" = 'TRUE' AND \"Отсутствует\" = 'FALSE'";
    cur =  ioMan.getIObjects("Определение сил и средств.Спец вагоны", filter);

    if (!cur.size()){
        QMessageBox::warning(0,"Внимание","Не найдены готовые и присутствующие вагоны");
        return 0;
    }

    //получение типов имеющихся вагонов и их количество
    QList<int> railsWeHave;
    QHash<int,int> railsAndCount;
    for (ODS::IObjectCursor::iterator it = cur.begin(); it != cur.end(); ++it){
        ODS::IObject iobj = *it;
        int railId = iobj.getIntAttr("Обозначение");
        QString railName = ioMan.getIObjectById(railId).getStringAttr("Тип вагона");
        if (!railType){
            if (!railsWeHave.contains(railId) && railName != "ВГ-124"){
                railsWeHave.append(railId);
                railsAndCount.insert(railId, 1);
            } else if (railsWeHave.contains(railId) && railName != "ВГ-124") {
                int count = railsAndCount.value(railId);
                railsAndCount.insert(railId, count+1);
            }
        } else {
            if (!railsWeHave.contains(railId)){
                railsWeHave.append(railId);
                railsAndCount.insert(railId, 1);
            } else if (railsWeHave.contains(railId)) {
                int count = railsAndCount.value(railId);
                railsAndCount.insert(railId, count+1);
            }
        }
    }

    IRS::Catalog::ptr catalog = catalMgr.getCatalogByName("Нормы загрузки вагонов");
    if (catalog.isNull()){
        QMessageBox::critical(0,"Ошибка", "Не найден классификатор"
                              "\"Нормы загрузки вагонов\""
                              " или не заданы права");
        return 0;
    }
    if (!catalog->createCache()){
        QMessageBox::critical(0,"Ошибка", "Неудачное кэширование классификатора"
                              "\"Нормы загрузки вагонов\"");
        return 0;
    }
//        if (!catalog->createIndex(QStringList()<<"Тип контейнера"<<"Тип вагона")){
//            QMessageBox::critical(0,"Ошибка", "Неудачное индексирование классификатора"
//                                  "\"Нормы загрузки вагонов\"");
//            return;
//        }

    IRS::Catalog::ptr catalogTzuNames = catalMgr.getCatalogByName("Классификатор контейнеров");
    if (catalogTzuNames.isNull()){
        QMessageBox::critical(0,"Ошибка", "Не найден классификатор"
                              "\"Классификатор контейнеров\""
                              " или не заданы права");
        return 0;
    }

    IRS::Catalog::ptr catalogRailNames = catalMgr.getCatalogByName("Классификатор вагонов по типам");
    if (catalogRailNames.isNull()){
        QMessageBox::critical(0,"Ошибка", "Не найден классификатор"
                              "\"Нормы загрузки вагонов без контейнеров\""
                              " или не заданы права");
        return 0;
    }

    //начинаем считать вагоны
    int resultRails = 0;
    QHashIterator<int, int> it(tzuResult);
    while (it.hasNext()) {
        it.next();

        int tzuRemain = it.value();
        int tzuTypeId = it.key();

        QHash<QString, QString> filterIrs;
        filterIrs.insert("Тип контейнера", QString::number(tzuTypeId));

        QList<int> railsCapacities;
        QHash<int, int> railsIdsCapacity;

        for (QList<int>::iterator it = railsWeHave.begin(); it != railsWeHave.end(); ++it){
             int railId = *it;

             filterIrs.insert("Тип вагона", QString::number(railId));
             IRS::CatalogEntry::ptrList list = catalog->entries(filterIrs);
             if (list.isEmpty()){
                 continue;
             }
             int capacity = searchForValueIRS(list);
             railsCapacities.append(capacity);
             railsIdsCapacity.insert(capacity, railId);
        }

        if(!railsIdsCapacity.size()){
            IRS::CatalogEntry::ptr entryTzu = catalogTzuNames->entryById(tzuTypeId);
            QMessageBox::critical(0,"Ошибка", "В нормах загрузки "
                                  "\"Нормы загрузки вагонов\" не найдены "
                                  "данные по загрузке\n с типом контейнера "+entryTzu->stringValue("Наименование")+
                                  " и доступными типами вагонов");
            return 0;
        }

        qStableSort(railsCapacities.begin(), railsCapacities.end(), qGreater<int>());
        for (int j = 0; j < railsCapacities.size(); ++j){
              int capacity = railsCapacities.at(j);

              int railsNeeded = tzuRemain/capacity;
              int remain = tzuRemain%capacity;

              int railId = railsIdsCapacity.value(capacity);

              int railsThatCapacity = railsAndCount.value(railId);

              if (remain){
                   railsNeeded += 1;
              }

              IRS::CatalogEntry::ptr entryRail = catalogRailNames->entryById(railId);
              IRS::CatalogEntry::ptr entryTzu = catalogTzuNames->entryById(tzuTypeId);

              if (railsNeeded <= railsThatCapacity){
                  //если хвататет вагонов

                  makeRow(tzuIdsCapacity2.value(tzuTypeId)*tzuRemain, entryTzu->stringValue("Наименование"), tzuTypeId,
                          tzuRemain, entryRail->stringValue("Тип вагона"), railId, railsNeeded);
                  resultRails += railsNeeded;
                  tzuRemain = tzuRemain - capacity*railsNeeded;
                  if (tzuRemain <= 0){
                      break;
                  }
              } else {
                  //если не хватает вагонов

                  makeRow(tzuIdsCapacity2.value(tzuTypeId)*railsThatCapacity*capacity, entryTzu->stringValue("Наименование"), tzuTypeId,
                          railsThatCapacity*capacity, entryRail->stringValue("Тип вагона"), railId, railsThatCapacity);
                  resultRails += railsThatCapacity;
                  tzuRemain = tzuRemain - capacity*railsThatCapacity;
              }

              if (tzuRemain <= 0){
                  break;
              }
         }

         if (tzuRemain>0){
             initTv();
             QMessageBox::warning(0,"Внимание", "Не хвататет готовых вагонов");
             return 0;
         }
    }

    return resultRails;
}

void RailsCalcForm::makeRow(int siCount, QString tzuType, int tzuTypeId, int tzuCount,
                            QString railType, int railTypeId, int railCount)
{
        QList<QStandardItem*> row;
        QStandardItem* item = new QStandardItem;
        item->setText(QString::number(model->rowCount()+1));
        row<<item;
        item = new QStandardItem;
        item->setText(QString::number(siCount));
        row<<item;
        item = new QStandardItem;
        item->setText(tzuType);
        item->setData(tzuTypeId);
        row<<item;
        item = new QStandardItem;
        item->setText(QString::number(tzuCount));
        row<<item;
        item = new QStandardItem;
        item->setText(railType);
        item->setData(railTypeId);
        row<<item;
        item = new QStandardItem;
        item->setText(QString::number(railCount));
        row<<item;

        model->appendRow(row);
}

int RailsCalcForm::searchForValueIRS(IRS::CatalogEntry::ptrList list)
{
    if (list.size() == 0){
            QMessageBox::warning(0,"Ошибка", "В классификаторе норм загрузки не найдена запись");
            return 0;
    } else {
        for (IRS::CatalogEntry::ptrList::iterator it = list.begin(); it != list.end(); ++it){
            IRS::CatalogEntry::ptr div = (*it);
            int result = div->value("НОТО8").toInt();
            if (result != 0 ){
                return result;
            }
            result = div->value("ТО на СК").toInt();
            if (result != 0 ){
                return result;
            }
            result = div->value("ТО на НГ9Т1").toInt();
            if (result != 0 ){
                return result;
            }
            result = div->value("ТО на НГ22В1").toInt();
            if (result != 0 ){
                return result;
            }
            result = div->value("РЭ на НГП2Т1").toInt();
            if (result != 0 ){
                return result;
            }
            result = div->value("Перечень Д30").toInt();
            if (result != 0 ){
                return result;
            }
            result = div->value("Инструкция ИЭ12").toInt();
            if (result != 0 ){
                return result;
            }
            QMessageBox::warning(0,"Ошибка", "В классификаторе норм загрузки\nне выставлено"
                                                             "значение");
            return 0;
        }
    }
    return 0;
}

void RailsCalcForm::save()
{
    if (model->rowCount() == 0) {
        QMessageBox::warning(0,"Ошибка", "Сначала определите нужное количество средств");
        return;
    }

    //привязывает расчет к перевозке
    ODS::DBManager db = ODS::OdsInterface::self()->dbManager();
    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();
    db.transaction();
    ODS::OdsInterface::self()->syncManager().syncTransaction();

    bool railTerm = ui->cbRailsTerm->itemData(ui->cbRailsTerm->currentIndex()).toBool();
    bool containerType = ui->cbContainerType->itemData(ui->cbContainerType->currentIndex()).toBool();

    ODS::IObjectQueryFilter filter;
    filter.whereCondition = "\"Перевозка\" ="+QString::number(currentTrId);
    ODS::IObjectCursor cur =  ioMan.getIObjects("Определение сил и средств.Вагоны определение", filter);

    if (cur.size()) {

        for (ODS::IObjectCursor::iterator it = cur.begin(); it != cur.end(); ++it){
            ODS::IObject iobj = *it;

            iobj.setAttr("Перевозка", currentTrId);
            iobj.setAttr("Средства защиты", containerType);
            iobj.setAttr("Температурный режим", railTerm);
            iobj.setAttr("Количество вагонов", ui->leAllRailCount->text().toInt());

            if (!iobj.update(ODS::IObject::NotRecursiveUpdate)){
                ODS::OdsInterface::self()->dbManager().rollback();
                QMessageBox::critical(0, "Ошибка", "Неудачная поппытка обновления записи");
                qDebug()<<"Ошибка при сохраниении данных в бд";
                return;
            } else {
                if (setIObjRights(iobj)) {
                    ODS::OdsInterface::self()->dbManager().commit();
                }
            }

            filter.whereCondition = "\"Определение\" ="+QString::number(iobj.id());
            ODS::IObjectCursor cur =  ioMan.getIObjects("Определение сил и средств.Вагоны определение таблица", filter);

            if (cur.size()) {
                for (ODS::IObjectCursor::iterator it = cur.begin(); it != cur.end(); ++it){
                    ODS::IObject iobj = *it;
                    ioMan.deleteIObjectById(iobj.id());
                }
            }

            for (int j = 0; j < model->rowCount(); ++j){
                ODS::IObject iobjTable = ioMan.createIObject("Определение сил и средств.Вагоны определение таблица");
                iobjTable.setAttr("Определение", iobj.id());
                iobjTable.setAttr("Порядковый номер", model->item(j,0)->text().toInt());
                iobjTable.setAttr("Количество СИ", model->item(j,1)->text().toInt());
                iobjTable.setAttr("Тип контейнера", model->item(j,2)->data().toInt());
                iobjTable.setAttr("Количество контейнеров", model->item(j,3)->text().toInt());
                iobjTable.setAttr("Тип вагона", model->item(j,4)->data().toInt());
                iobjTable.setAttr("Количество вагонов", model->item(j,5)->text().toInt());

                if (!iobjTable.save(ODS::IObject::NotRecursiveSave)){
                    ODS::OdsInterface::self()->dbManager().rollback();
                    QMessageBox::critical(0, "Ошибка", "Неудачная попытка сохранения записи");
                    qDebug()<<"Ошибка при сохраниении данных в бд";
                    return;
                } else {
                    if (setIObjRights(iobjTable)) {
                        ODS::OdsInterface::self()->dbManager().commit();
                        msgToStatus("Расчеты обновлены");
                    }
                }
            }
        }

    } else {
        ODS::IObject iobj = ioMan.createIObject("Определение сил и средств.Вагоны определение");

        iobj.setAttr("Перевозка", currentTrId);
        iobj.setAttr("Средства защиты", containerType);
        iobj.setAttr("Температурный режим", railTerm);
        iobj.setAttr("Количество вагонов", ui->leAllRailCount->text().toInt());

        if (!iobj.save(ODS::IObject::NotRecursiveSave)){
            ODS::OdsInterface::self()->dbManager().rollback();
            QMessageBox::critical(0, "Ошибка", "Неудачная поппытка сохранения записи");
            qDebug()<<"Ошибка при сохраниении данных в бд";
            return;
        } else {
            if (setIObjRights(iobj)) {
                ODS::OdsInterface::self()->dbManager().commit();
            }
        }

        filter.whereCondition = "\"Определение\" ="+QString::number(iobj.id());
        ODS::IObjectCursor cur =  ioMan.getIObjects("Определение сил и средств.Вагоны определение таблица", filter);

        if (cur.size()) {
            for (ODS::IObjectCursor::iterator it = cur.begin(); it != cur.end(); ++it){
                ODS::IObject iobj = *it;
                ioMan.deleteIObjectById(iobj.id());
            }
        }

        for (int j = 0; j < model->rowCount(); ++j){
            ODS::IObject iiobjTable = ioMan.createIObject("Определение сил и средств.Вагоны определение таблица");
            iiobjTable.setAttr("Определение", iobj.id());
            iiobjTable.setAttr("Порядковый номер", model->item(j,0)->text().toInt());
            iiobjTable.setAttr("Количество СИ", model->item(j,1)->text().toInt());
            iiobjTable.setAttr("Тип контейнера", model->item(j,2)->data().toInt());
            iiobjTable.setAttr("Количество контейнеров", model->item(j,3)->text().toInt());
            iiobjTable.setAttr("Тип вагона", model->item(j,4)->data().toInt());
            iiobjTable.setAttr("Количество вагонов", model->item(j,5)->text().toInt());

            if (!iiobjTable.save(ODS::IObject::NotRecursiveSave)){
                ODS::OdsInterface::self()->dbManager().rollback();
                QMessageBox::critical(0, "Ошибка", "Неудачная попытка сохранения записи");
                qDebug()<<"Ошибка при сохраниении данных в бд";
                return;
            } else {
                if (setIObjRights(iiobjTable)) {
                    ODS::OdsInterface::self()->dbManager().commit();
                    msgToStatus("Расчеты созданы");
                }
            }
        }
    }
}

void RailsCalcForm::close()
{
    this->parentWidget()->close();
}
