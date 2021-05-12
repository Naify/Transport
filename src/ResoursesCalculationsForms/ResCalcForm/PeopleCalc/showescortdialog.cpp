#include "showescortdialog.h"
#include "ui_showescortdialog.h"
#include "showescortdialogmodel.h"
#include "showescortdialogdelegate.h"

#include "OdsInterface"
#include "QMessageBox"
#include "global_defs.h"

showEscortDialog::showEscortDialog(QMap<int, int> people, int trId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::showEscortDialog),
    peopleList(people),
    trId(trId)
{
    ui->setupUi(this);
    init();
}

showEscortDialog::~showEscortDialog()
{
    delete model;
    delete ui;    
}

void showEscortDialog::init()
{
    model = new ShowEscortDialogModel(ui->tv);
    model->setColumnCount(7);

    model->setHorizontalHeaderItem(0,new QStandardItem(QString("Должность")));
    model->setHorizontalHeaderItem(1,new QStandardItem(QString("В/звание")));
    model->setHorizontalHeaderItem(2,new QStandardItem(QString("ФИО")));
    model->setHorizontalHeaderItem(3,new QStandardItem(QString("Вооружение\nНаименование")));
    model->setHorizontalHeaderItem(4,new QStandardItem(QString("Вооружение\nНомер")));
    model->setHorizontalHeaderItem(5,new QStandardItem(QString("Транспорт\nОбозначение")));
    model->setHorizontalHeaderItem(6,new QStandardItem(QString("Транспорт\nНомер")));

    ui->tv->setModel(model);
    ui->tv->setItemDelegate(new ShowEscortDialogDelegate(ui->tv));

    ui->tv->resizeColumnsToContents();
    ui->tv->verticalHeader()->setVisible(false);
#if QT_VERSION >= 0x050000
    ui->tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tv->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#else
    ui->tv->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tv->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#endif
    ui->tv->setSelectionMode(QAbstractItemView::SingleSelection);

    model->load(peopleList, trId);

    connect(ui->pbDel, SIGNAL(released()), this, SLOT(del()));
    connect(ui->pbSave, SIGNAL(released()), this, SLOT(save()));
    connect(ui->pbExit, SIGNAL(released()), this, SLOT(close()));
}

void showEscortDialog::del()
{
    QModelIndex index = ui->tv->currentIndex();

    if (!index.isValid()){
        QMessageBox::warning(0,"Ошибка", "Выберите ряд");
        return;
    } else {
        emit delId(model->item(index.row(),0)->data().toInt());
        model->remove(index);
    }
}

void showEscortDialog::save()
{
    if (!model->save()){
        QMessageBox::critical(this, "Ошибка", "Не удалось сохранить записи");
    } else {
        emit clearNewPeople();
    }
}

void showEscortDialog::close()
{
    QDialog::close();
}
