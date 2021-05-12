#include "damagedsiitemdelegate.h"
#include "damagedsiadmissionmodel.h"

#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <CatalogManager.h>
#include <QDateEdit>

DamagedSiItemDelegate::DamagedSiItemDelegate(QObject *parent):
    QItemDelegate(parent)
{
}

DamagedSiItemDelegate::~DamagedSiItemDelegate()
{
}

QWidget *DamagedSiItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
    switch(index.data(DamagedSiAdmissionModel::TypeRole).toInt())
    {
    case DamagedSiAdmissionModel::Num:
    {
        QSpinBox* spb = new QSpinBox(parent);
        spb->setValue(index.data(Qt::DisplayRole).toInt());
        return spb;
    }
    case DamagedSiAdmissionModel::Rank:
    case DamagedSiAdmissionModel::Post:
    case DamagedSiAdmissionModel::Fio:
    case DamagedSiAdmissionModel::Date:
    {
        QLineEdit* editor = new QLineEdit(parent);
        editor->setReadOnly(false);
        editor->setText(index.data(Qt::DisplayRole).toString());
        return editor;
    }
    case DamagedSiAdmissionModel::Si:
    {
        QComboBox* editor = new QComboBox(parent);
        editor->setEditable(true);
        editor->setInsertPolicy(QComboBox::NoInsert);

        IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
        IRS::Catalog::ptr catalog_divs = catalMgr.getCatalogByName("Классификатор грузов");

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
    case DamagedSiAdmissionModel::NumAndDate:
    {
        QLineEdit* editor = new QLineEdit(parent);
        editor->setText(index.data(Qt::DisplayRole).toString());
        return editor;
    }
    case DamagedSiAdmissionModel::Place:
    {
        QComboBox* editor = new QComboBox(parent);
        editor->setEditable(true);
        editor->setInsertPolicy(QComboBox::NoInsert);

        IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
        IRS::Catalog::ptr catalog_divs = catalMgr.getCatalogByName("Классификатор воинских частей");

        if (catalog_divs) {
            IRS::CatalogEntry::ptrList divs_list = catalog_divs->entries();
            for (IRS::CatalogEntry::ptrList::iterator it = divs_list.begin(); it != divs_list.end(); ++it){
                IRS::CatalogEntry::ptr div = (*it);
                editor->addItem(div->stringValue("Условное наименование"),QVariant(div->id()));
            }
        } else {
            QLineEdit* editor = new QLineEdit(parent);
            editor->setText(index.data(Qt::DisplayRole).toString());
            return editor;
        }
        editor->setCurrentIndex(editor->findText(index.data().toString()));
        return editor;
    }
    }
    QLineEdit* editor = new QLineEdit(parent);
    editor->setText(index.data(Qt::DisplayRole).toString());
    return editor;
}

void DamagedSiItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QFontMetrics fm(editor->font());

    switch(index.data(DamagedSiAdmissionModel::TypeRole).toInt())
    {
    case DamagedSiAdmissionModel::Num:{
        QSpinBox* spb = qobject_cast<QSpinBox*>(editor);
        model->setData(index, QString::number(spb->value()), Qt::DisplayRole);
        break;
    }
    case DamagedSiAdmissionModel::Rank:
    case DamagedSiAdmissionModel::Post:
    case DamagedSiAdmissionModel::Fio:
    case DamagedSiAdmissionModel::Date:
        break;
    case DamagedSiAdmissionModel::Si:{
        QComboBox* cb = qobject_cast<QComboBox*>(editor);
        model->setData(index, cb->currentText(), Qt::DisplayRole);
        model->setData(index, QSize(fm.width(cb->currentText())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
        model->setData(index, cb->itemData(cb->currentIndex()).toInt(), DamagedSiAdmissionModel::SiIdRole);
        break;
    }
//    case DamagedSiAdmissionModel::NumAndDate:
    case DamagedSiAdmissionModel::Place:{
        QComboBox* cb = qobject_cast<QComboBox*>(editor);
        model->setData(index, cb->currentText(), Qt::DisplayRole);
        model->setData(index, QSize(fm.width(cb->currentText())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
        model->setData(index, cb->itemData(cb->currentIndex()).toInt(), DamagedSiAdmissionModel::PlaceRole);
        break;
    }
    default:
    QLineEdit* le = qobject_cast<QLineEdit*>(editor);
    model->setData(index, le->text(), Qt::DisplayRole);
    model->setData(index, QSize(fm.width( le->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    break;
    }

    model->setData(index.sibling(index.row(), 0), true, DamagedSiAdmissionModel::ChangedRole);
}
