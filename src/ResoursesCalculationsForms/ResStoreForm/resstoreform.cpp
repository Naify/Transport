#include "resstoreform.h"
#include "ui_resstoreform.h"

#include "QStandardItem"
#include "QMessageBox"
#include "QDebug"
#include "CarsStore/carsidelegate.h"
#include "CarsStore/carsmodel.h"
#include "EngTechStore/engtechidelegate.h"
#include "EngTechStore/engtechmodel.h"
#include "RailsStore/railsidelegate.h"
#include "RailsStore/railsmodel.h"
#include "SuppMachinesStore/suppmachinesidelegate.h"
#include "SuppMachinesStore/supportmachinesmodel.h"
#include "TzuStore/tzuidelegate.h"
#include "TzuStore/tzuskmodel.h"

ResStoreForm::ResStoreForm(Form formType, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResStoreForm),
    formType(formType)
{
    ui->setupUi(this);
    init();
}

ResStoreForm::~ResStoreForm()
{
    switch (formType)
    {
    case Rails:
        delete railsM;
        break;
    case Cars:
        delete carsM;
        break;
    case TzuSk:
        delete tzuM;
        break;
    case EngTech:
        delete engM;
        break;
    case SuppMach:
        delete suppM;
        break;
    }
    delete ui;
}

void ResStoreForm::init()
{
    setModel();
    setDelegate();

    ui->tv->resizeColumnsToContents();
    ui->tv->verticalHeader()->setVisible(false);
#if QT_VERSION >= 0x050000
    ui->tv->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
#else
    ui->tv->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#endif
//    ui->tv->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    connect(ui->pbAdd, SIGNAL(released()), this, SLOT(add()));
    connect(ui->pbDel, SIGNAL(released()), this, SLOT(remove()));
    connect(ui->pbCancel, SIGNAL(released()), this, SLOT(cancel()));
    connect(ui->pbSave, SIGNAL(released()), this, SLOT(save()));
    connect(ui->pbExit, SIGNAL(released()), this, SLOT(close()));
}

void ResStoreForm::setModel()
{
    switch (formType)
    {
    case Rails:
        railsM = new RailsModel(ui->tv);
        railsM->setColumnCount(14);
        railsM->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
        railsM->setHorizontalHeaderItem(1, new QStandardItem(QString("Обозначение")));
        railsM->setHorizontalHeaderItem(2, new QStandardItem(QString("Заводской\nномер")));
        railsM->setHorizontalHeaderItem(3, new QStandardItem(QString("Общесетевой\nномер")));
        railsM->setHorizontalHeaderItem(4, new QStandardItem(QString("Год\nпостройки")));
        railsM->setHorizontalHeaderItem(5, new QStandardItem(QString("Даты посл\nЕТР")));
        railsM->setHorizontalHeaderItem(6, new QStandardItem(QString("Даты посл\nДР")));
        railsM->setHorizontalHeaderItem(7, new QStandardItem(QString("Даты посл\nЗР")));
        railsM->setHorizontalHeaderItem(8, new QStandardItem(QString("Готовность")));
        railsM->setHorizontalHeaderItem(9, new QStandardItem(QString("Даты очередного\nЕТР")));
        railsM->setHorizontalHeaderItem(10, new QStandardItem(QString("Даты очередного\nДР")));
        railsM->setHorizontalHeaderItem(11, new QStandardItem(QString("Даты очередного\nЗР")));
        railsM->setHorizontalHeaderItem(12, new QStandardItem(QString("Тип установленной\nохран системы")));
        railsM->setHorizontalHeaderItem(13, new QStandardItem(QString("Отсутствует")));
        ui->tv->setModel(railsM);
        break;
    case Cars:
        carsM = new CarsModel(ui->tv);
        carsM->setColumnCount(10);
        carsM->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
        carsM->setHorizontalHeaderItem(1, new QStandardItem(QString("Обозначение")));
        carsM->setHorizontalHeaderItem(2, new QStandardItem(QString("Заводской\nномер")));
        carsM->setHorizontalHeaderItem(3, new QStandardItem(QString("Военный\nномер")));
        carsM->setHorizontalHeaderItem(4, new QStandardItem(QString("Год\nвыпуска")));
        carsM->setHorizontalHeaderItem(5, new QStandardItem(QString("Даты последнего\nпереосвидетельствования")));
        carsM->setHorizontalHeaderItem(6, new QStandardItem(QString("Готовность")));
        carsM->setHorizontalHeaderItem(7, new QStandardItem(QString("Даты очередного\nосвидетельствования")));
        carsM->setHorizontalHeaderItem(8, new QStandardItem(QString("Примечание")));
        carsM->setHorizontalHeaderItem(9, new QStandardItem(QString("Отсутствует")));
        ui->tv->setModel(carsM);
        break;
    case TzuSk:
        tzuM = new TzuSkModel(ui->tv);
        tzuM->setColumnCount(8);
        tzuM->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
        tzuM->setHorizontalHeaderItem(1, new QStandardItem(QString("Наименование")));
        tzuM->setHorizontalHeaderItem(2, new QStandardItem(QString("Обозначение")));
        tzuM->setHorizontalHeaderItem(3, new QStandardItem(QString("Заводской\nномер")));
        tzuM->setHorizontalHeaderItem(4, new QStandardItem(QString("Год\nвыпуска")));
        tzuM->setHorizontalHeaderItem(5, new QStandardItem(QString("Готовность")));
        tzuM->setHorizontalHeaderItem(6, new QStandardItem(QString("Примечание")));
        tzuM->setHorizontalHeaderItem(7, new QStandardItem(QString("Отсутствует")));
        ui->tv->setModel(tzuM);
        break;
    case EngTech:
        engM = new EngTechModel(ui->tv);
        engM->setColumnCount(10);
        engM->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
        engM->setHorizontalHeaderItem(1, new QStandardItem(QString("Наименование")));
        engM->setHorizontalHeaderItem(2, new QStandardItem(QString("Обозначение")));
        engM->setHorizontalHeaderItem(3, new QStandardItem(QString("Заводской\nномер")));
        engM->setHorizontalHeaderItem(4, new QStandardItem(QString("Регистрационный\nномер")));
        engM->setHorizontalHeaderItem(5, new QStandardItem(QString("Военный\nномер")));
        engM->setHorizontalHeaderItem(6, new QStandardItem(QString("Год\nвыпуска")));
        engM->setHorizontalHeaderItem(7, new QStandardItem(QString("Даты последнего\nпереосвидетельствования")));
        engM->setHorizontalHeaderItem(8, new QStandardItem(QString("Готовность")));
        engM->setHorizontalHeaderItem(9, new QStandardItem(QString("Даты очередного\nосвидетельствования")));
        ui->tv->setModel(engM);
        break;
    case SuppMach:
        suppM = new SupportMachinesModel(ui->tv);
        suppM->setColumnCount(7);
        suppM->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
        suppM->setHorizontalHeaderItem(1, new QStandardItem(QString("Наименование")));
        suppM->setHorizontalHeaderItem(2, new QStandardItem(QString("Обозначение")));
        suppM->setHorizontalHeaderItem(3, new QStandardItem(QString("Заводской\nномер")));
        suppM->setHorizontalHeaderItem(4, new QStandardItem(QString("Военный\nномер")));
        suppM->setHorizontalHeaderItem(5, new QStandardItem(QString("Год\nвыпуска")));
        suppM->setHorizontalHeaderItem(6, new QStandardItem(QString("Готовность")));
        ui->tv->setModel(suppM);
        break;
    }
}

void ResStoreForm::setDelegate()
{
    switch (formType)
    {
    case Rails:
        ui->tv->setItemDelegate(new RailsIDelegate(ui->tv));
        break;
    case Cars:
        ui->tv->setItemDelegate(new CarsIDelegate(ui->tv));
        break;
    case TzuSk:
        ui->tv->setItemDelegate(new TzuIDelegate(ui->tv));
        break;
    case EngTech:
        ui->tv->setItemDelegate(new EngTechIDelegate(ui->tv));
        break;
    case SuppMach:
        ui->tv->setItemDelegate(new SuppMachinesIDelegate(ui->tv));
        break;
    }
}

void ResStoreForm::add()
{
    switch (formType)
    {
    case Rails:
        railsM->addEmptyRow();
        break;
    case Cars:
        carsM->addEmptyRow();
        break;
    case TzuSk:
        tzuM->addEmptyRow();
        break;
    case EngTech:
        engM->addEmptyRow();
        break;
    case SuppMach:
        suppM->addEmptyRow();
        break;
    }
}

void ResStoreForm::remove()
{
    QModelIndex index = ui->tv->currentIndex();
    if (!index.isValid())
        return;
    switch (formType)
    {
    case Rails:
        railsM->remove(index);
        break;
    case Cars:
        carsM->remove(index);
        break;
    case TzuSk:
        tzuM->remove(index);
        break;
    case EngTech:
        engM->remove(index);
        break;
    case SuppMach:
        suppM->remove(index);
        break;
    }
}

void ResStoreForm::cancel()
{
    switch (formType)
    {
    case Rails:
        railsM->load();
        break;
    case Cars:
        carsM->load();
        break;
    case TzuSk:
        tzuM->load();
        break;
    case EngTech:
        engM->load();
        break;
    case SuppMach:
        suppM->load();
        break;
    }
}

void ResStoreForm::save()
{
    switch (formType)
    {
    case Rails:
        if (!railsM->save()){
            qDebug()<<"Error saving"<<Q_FUNC_INFO;
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить записи");
        }
        break;
    case Cars:
        if (!carsM->save()){
            qDebug()<<"Error saving"<<Q_FUNC_INFO;
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить записи");
        }
        break;
    case TzuSk:
        if (!tzuM->save()){
            qDebug()<<"Error saving"<<Q_FUNC_INFO;
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить записи");
        }
        break;
    case EngTech:
        if (!engM->save()){
            qDebug()<<"Error saving"<<Q_FUNC_INFO;
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить записи");
        }
        break;
    case SuppMach:
        if (!suppM->save()){
            qDebug()<<"Error saving"<<Q_FUNC_INFO;
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить записи");
        }
        break;
    }
}

void ResStoreForm::close()
{
    this->parentWidget()->close();
}
