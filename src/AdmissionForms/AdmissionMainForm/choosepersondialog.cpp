#include "choosepersondialog.h"
#include "ui_choosepersondialog.h"

#include <OdsInterface>

ChoosePersonDialog::ChoosePersonDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChoosePersonDialog)
{
    ui->setupUi(this);

    init();
}

ChoosePersonDialog::~ChoosePersonDialog()
{
    delete ui;
}

void ChoosePersonDialog::init()
{
    ODS::IObjectCursor cursor = ODS::OdsInterface::self()->iobjectManager().getIObjectCursor("Допуска КП.Личный состав");

    ODS::IObjectCursor::iterator it = cursor.begin();
    while (it != cursor.end())
    {
        ODS::IObject obj = *it;
        QString label = obj.getLink("Должность").getStringAttr("Наименование") + " " + obj.getStringAttr("ФИО");
        QListWidgetItem* item = new QListWidgetItem(label, ui->lw);
        item->setFlags(item->flags() | Qt::ItemIsSelectable);
        item->setData(6,obj.id());
        ++it;
    }

    connect(ui->bB, SIGNAL(accepted()), this, SLOT(handleOkButton()));
}

void ChoosePersonDialog::handleOkButton()
{
    int id = ui->lw->currentItem()->data(6).toInt();
    emit(sendId(id));
}
