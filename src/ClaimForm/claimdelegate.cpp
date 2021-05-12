#include "claimdelegate.h"
#include "claimmodel.h"
#include "claimform.h"

#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <CatalogManager.h>
#include <QDateTimeEdit>

ClaimDelegate::ClaimDelegate(QObject *parent):
    QItemDelegate(parent)
{
}

ClaimDelegate::~ClaimDelegate()
{
}

QWidget *ClaimDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
    switch(index.data(ClaimModel::TypeRole).toInt())
    {
    case ClaimModel::Num:
    case ClaimModel::Count:
    case ClaimModel::Oficers:
    case ClaimModel::Ensignes:
    case ClaimModel::Sergants:
    case ClaimModel::Soldier:
    {
        QSpinBox* spb = new QSpinBox(parent);
        spb->setValue(index.data(Qt::DisplayRole).toInt());
        return spb;
    }
    case ClaimModel::Sender:
    case ClaimModel::Reciever:
    {
        QComboBox* editor = new QComboBox(parent);
        editor->setEditable(true);
        editor->setInsertPolicy(QComboBox::NoInsert);

        IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
        IRS::Catalog::ptr catalog_divs = catalMgr.getCatalogByName("Классификатор воинских частей");

        IRS::CatalogEntry::ptrList divs_list = catalog_divs->entries();
        for (IRS::CatalogEntry::ptrList::iterator it = divs_list.begin(); it != divs_list.end(); ++it){
            IRS::CatalogEntry::ptr div = (*it);
            editor->addItem(div->stringValue("Условное наименование"),QVariant(div->id()));
        }

        catalog_divs = catalMgr.getCatalogByName("Классификатор предприятий промышленности");

        divs_list = catalog_divs->entries();
        for (IRS::CatalogEntry::ptrList::iterator it = divs_list.begin(); it != divs_list.end(); ++it){
            IRS::CatalogEntry::ptr div = (*it);
            editor->addItem(div->stringValue("Сокращенное наименование"),QVariant(div->id()));
        }
        editor->setCurrentIndex(editor->findText(index.data().toString()));
        return editor;
    }
    case ClaimModel::SendStation:
    case ClaimModel::Numbers:
    case ClaimModel::ArriveStation:{
        QLineEdit* editor = new QLineEdit(parent);
        editor->setText(index.data(Qt::DisplayRole).toString());
        return editor;
    }
    case ClaimModel::DateTimeCar:
    case ClaimModel::DateTimeSend:{
        QDateTimeEdit* de = new QDateTimeEdit(parent);
        de->setDisplayFormat("dd.MM.yyyy hh:mm");
        de->setCalendarPopup(true);
        if (index.data(Qt::DisplayRole).toString() == ""){
            de->setDateTime(QDateTime::currentDateTime());
        } else {
            de->setDateTime(QDateTime::fromString(index.data(Qt::DisplayRole).toString(),"dd.MM.yyyy hh:mm"));
        }
        return de;
    }
    case ClaimModel::CarType: {

        IRS::CatalogManager catalMgr(ODS::OdsInterface::self());

        if (ClaimForm::getTrType() == "жд"){
            QComboBox* editor = new QComboBox(parent);
            editor->setEditable(true);
            editor->setInsertPolicy(QComboBox::NoInsert);

            IRS::Catalog::ptr  catalog_divs = catalMgr.getCatalogByName("Классификатор вагонов по типам");
            IRS::CatalogEntry::ptrList divs_list = catalog_divs->entries();
            for (IRS::CatalogEntry::ptrList::iterator it = divs_list.begin(); it != divs_list.end(); ++it){
                IRS::CatalogEntry::ptr div = (*it);
                editor->addItem(div->stringValue("Тип вагона"),QVariant(div->id()));
            }
            editor->setCurrentIndex(editor->findText(index.data().toString()));
            return editor;
        } else if (ClaimForm::getTrType() == "авто"){
            QComboBox* editor = new QComboBox(parent);
            editor->setEditable(true);
            editor->setInsertPolicy(QComboBox::NoInsert);

            IRS::Catalog::ptr catalog_divs = catalMgr.getCatalogByName("Классификатор автомобильной техники");
            IRS::CatalogEntry::ptrList divs_list = catalog_divs->entries();
            for (IRS::CatalogEntry::ptrList::iterator it = divs_list.begin(); it != divs_list.end(); ++it){
                IRS::CatalogEntry::ptr div = (*it);
                editor->addItem(div->stringValue("Наименование"),QVariant(div->id()));
            }
            editor->setCurrentIndex(editor->findText(index.data().toString()));
            return editor;
        } else {
            QLineEdit* editor = new QLineEdit(parent);
            editor->setText(index.data(Qt::DisplayRole).toString());
            return editor;
        }
    }
    }
return 0;
}

void ClaimDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QFontMetrics fm(editor->font());

    switch(index.data(ClaimModel::TypeRole).toInt())
    {
    case ClaimModel::Num:
    case ClaimModel::Count:
    case ClaimModel::Oficers:
    case ClaimModel::Ensignes:
    case ClaimModel::Sergants:
    case ClaimModel::Soldier:{
        QSpinBox* spb = qobject_cast<QSpinBox*>(editor);
        model->setData(index, QString::number(spb->value()), Qt::DisplayRole);
        break;
    }
    case ClaimModel::Sender:
    case ClaimModel::Reciever:{
        QComboBox* cb = qobject_cast<QComboBox*>(editor);
        model->setData(index, cb->currentText(), Qt::DisplayRole);
        model->setData(index, QSize(fm.width(cb->currentText())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
        model->setData(index, cb->itemData(cb->currentIndex()).toInt(), ClaimModel::IrsRole);
        break;
    }
    case ClaimModel::DateTimeCar:
    case ClaimModel::DateTimeSend:{
        QDateTimeEdit* de = qobject_cast<QDateTimeEdit*>(editor);
        model->setData(index, de->dateTime().toString("dd.MM.yyyy hh:mm"), Qt::DisplayRole);
        break;
    }
    case ClaimModel::SendStation:
    case ClaimModel::Numbers:
    case ClaimModel::ArriveStation:{
        QLineEdit* le = qobject_cast<QLineEdit*>(editor);
        model->setData(index, le->text(), Qt::DisplayRole);
        model->setData(index, QSize(fm.width( le->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
        break;
    }
    case ClaimModel::CarType:{
        if (ClaimForm::getTrType() == "жд" || ClaimForm::getTrType() == "авто"){
            QComboBox* cb = qobject_cast<QComboBox*>(editor);
            model->setData(index, cb->currentText(), Qt::DisplayRole);
            model->setData(index, QSize(fm.width(cb->currentText())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
            model->setData(index, cb->itemData(cb->currentIndex()).toInt(), ClaimModel::IrsRole);
            break;
        } else {
            QLineEdit* le = qobject_cast<QLineEdit*>(editor);
            model->setData(index, le->text(), Qt::DisplayRole);
            model->setData(index, QSize(fm.width( le->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
            break;
        }
    }
    }

    model->setData(index.sibling(index.row(), 0), true, ClaimModel::ChangedRole);
}


