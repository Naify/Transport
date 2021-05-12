#include "personitemdelegate.h"
#include "personadmissionmodel.h"

#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <CatalogManager.h>
#include <QDateEdit>

PersonItemDelegate::PersonItemDelegate(QObject *parent):
    QItemDelegate(parent)
{
}

PersonItemDelegate::~PersonItemDelegate()
{
}

QWidget *PersonItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
    switch(index.data(PersonAdmissionModel::TypeRole).toInt())
    {
    case PersonAdmissionModel::Num:
    {
        QSpinBox* spb = new QSpinBox(parent);
        spb->setValue(index.data(Qt::DisplayRole).toInt());
        return spb;
    }
    case PersonAdmissionModel::Rank:
    {
        QComboBox* editor = new QComboBox(parent);
        editor->setEditable(true);
        editor->setInsertPolicy(QComboBox::NoInsert);

        IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
        IRS::Catalog::ptr catalog_divs = catalMgr.getCatalogByName("Классификатор воинских званий");

        if (catalog_divs) {
            IRS::CatalogEntry::ptrList divs_list = catalog_divs->entries();
            for (IRS::CatalogEntry::ptrList::iterator it = divs_list.begin(); it != divs_list.end(); ++it){
                IRS::CatalogEntry::ptr div = (*it);
                editor->addItem(div->stringValue("Наименование"),QVariant(div->id()));
            }
        } else {
            QLineEdit* editor = new QLineEdit(parent);
            editor->setText(index.data(Qt::DisplayRole).toString());
            return editor;
        }
        editor->setCurrentIndex(editor->findText(index.data().toString()));
        return editor;
    }
    case PersonAdmissionModel::Post:
    {
        QComboBox* editor = new QComboBox(parent);
        editor->setEditable(true);
        editor->setInsertPolicy(QComboBox::NoInsert);

        IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
        IRS::Catalog::ptr catalog_divs = catalMgr.getCatalogByName("Классификатор должностей");

        if (catalog_divs) {
            IRS::CatalogEntry::ptrList divs_list = catalog_divs->entries();
            for (IRS::CatalogEntry::ptrList::iterator it = divs_list.begin(); it != divs_list.end(); ++it){
                IRS::CatalogEntry::ptr div = (*it);
                editor->addItem(div->stringValue("Наименование"),QVariant(div->id()));
            }
        } else {
            QLineEdit* editor = new QLineEdit(parent);
            editor->setText(index.data(Qt::DisplayRole).toString());
            return editor;
        }
        editor->setCurrentIndex(editor->findText(index.data().toString()));
        return editor;
    }
    case PersonAdmissionModel::Fio:
        break;
    case PersonAdmissionModel::Date:
    {
        QDateEdit* de = new QDateEdit(parent);
        de->setDisplayFormat("dd.MM.yyyy");
        de->setCalendarPopup(true);
        if (index.data(Qt::DisplayRole).toString() == ""){
            de->setDate(QDate::currentDate());
        } else {
            de->setDate(QDate::fromString(index.data(Qt::DisplayRole).toString(),"dd.MM.yyyy"));
        }
        return de;
    }
    }
    QLineEdit* editor = new QLineEdit(parent);
    editor->setText(index.data(Qt::DisplayRole).toString());
    return editor;
}

void PersonItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QFontMetrics fm(editor->font());

    switch(index.data(PersonAdmissionModel::TypeRole).toInt())
    {
    case PersonAdmissionModel::Num:{
        QSpinBox* spb = qobject_cast<QSpinBox*>(editor);
        model->setData(index, QString::number(spb->value()), Qt::DisplayRole);
        break;
    }
    case PersonAdmissionModel::Rank:
    case PersonAdmissionModel::Post:{
        QComboBox* cb = qobject_cast<QComboBox*>(editor);
        model->setData(index, cb->currentText(), Qt::DisplayRole);
        model->setData(index, QSize(fm.width(cb->currentText())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
        model->setData(index, cb->itemData(cb->currentIndex()).toInt(), PersonAdmissionModel::IrsIdRole);
        break;
    }
    case PersonAdmissionModel::Date:{
        QDateEdit* de = qobject_cast<QDateEdit*>(editor);
        model->setData(index, de->date().toString("dd.MM.yyyy"), Qt::DisplayRole);
        break;
    }
    default:{
    QLineEdit* le = qobject_cast<QLineEdit*>(editor);
    model->setData(index, le->text(), Qt::DisplayRole);
    model->setData(index, QSize(fm.width( le->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    break;
    }
    }

    model->setData(index.sibling(index.row(), 0), true, PersonAdmissionModel::ChangedRole);
}
