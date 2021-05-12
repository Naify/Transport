#include "railsidelegate.h"
#include "railsmodel.h"

#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <CatalogManager.h>
#include <QDateEdit>

RailsIDelegate::RailsIDelegate(QObject *parent):
    QItemDelegate(parent)
{
}

RailsIDelegate::~RailsIDelegate()
{
}

QWidget *RailsIDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
    switch(index.data(RailsModel::TypeRole).toInt())
    {
    case RailsModel::Num:
    {
        QSpinBox* spb = new QSpinBox(parent);
        spb->setValue(index.data(Qt::DisplayRole).toInt());
        return spb;
    }
    case RailsModel::Designation:
    {
        QComboBox* editor = new QComboBox(parent);
        editor->setEditable(false);
        editor->setInsertPolicy(QComboBox::NoInsert);

        IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
        IRS::Catalog::ptr catalog_divs = catalMgr.getCatalogByName("Классификатор вагонов по типам");

        if (catalog_divs) {
            IRS::CatalogEntry::ptrList divs_list = catalog_divs->entries();
            for (IRS::CatalogEntry::ptrList::iterator it = divs_list.begin(); it != divs_list.end(); ++it){
                IRS::CatalogEntry::ptr div = (*it);
                editor->addItem(div->stringValue("Тип вагона"),QVariant(div->id()));
            }
        } else {
            QLineEdit* editor = new QLineEdit(parent);
            editor->setText(index.data(Qt::DisplayRole).toString());
            return editor;
        }
        editor->setCurrentIndex(editor->findText(index.data().toString()));
//        editor->view()->setMinimumWidth(editor->minimumSizeHint().width());
        return editor;
    }
    case RailsModel::DateRelease:
    case RailsModel::DateLastEtr:
    case RailsModel::DateLastDr:
    case RailsModel::DateLastZr:
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
    case RailsModel::Ready:
    {
        QComboBox* editor = new QComboBox(parent);
        editor->setEditable(false);
        editor->setInsertPolicy(QComboBox::NoInsert);
        editor->addItem("Готов");
        editor->addItem("Не готов");
        return editor;
    }
    case RailsModel::DateNextEtr:
    case RailsModel::DateNextDr:
    case RailsModel::DateNextZr:{
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
    case RailsModel::SecurityType:
        break;
    case RailsModel::Away:{
        QComboBox* editor = new QComboBox(parent);
        editor->setEditable(false);
        editor->setInsertPolicy(QComboBox::NoInsert);
        editor->addItem("Отсутствует");
        editor->addItem("Присутствует");
        return editor;
    }
    }
    QLineEdit* editor = new QLineEdit(parent);
    editor->setText(index.data(Qt::DisplayRole).toString());
    return editor;
}

void RailsIDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QFontMetrics fm(editor->font());

    switch(index.data(RailsModel::TypeRole).toInt())
    {
    case RailsModel::Num:{
        QSpinBox* spb = qobject_cast<QSpinBox*>(editor);
        model->setData(index, QString::number(spb->value()), Qt::DisplayRole);
        break;
    }
    case RailsModel::Designation:{
        QComboBox* cb = qobject_cast<QComboBox*>(editor);
        model->setData(index, cb->currentText(), Qt::DisplayRole);
        model->setData(index, QSize(fm.width(cb->currentText())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
        model->setData(index, cb->itemData(cb->currentIndex()).toInt(), RailsModel::DesignatRole);
        break;
    }
    case RailsModel::DateRelease:
    case RailsModel::DateLastEtr:
    case RailsModel::DateLastDr:
    case RailsModel::DateLastZr:
    case RailsModel::DateNextEtr:
    case RailsModel::DateNextDr:
    case RailsModel::DateNextZr:{
        QDateEdit* de = qobject_cast<QDateEdit*>(editor);
        model->setData(index, de->date().toString("dd.MM.yyyy"), Qt::DisplayRole);
        break;
    }
    case RailsModel::Ready:
    case RailsModel::Away:{
        QComboBox* cb = qobject_cast<QComboBox*>(editor);
        model->setData(index, cb->currentText(), Qt::DisplayRole);
        model->setData(index, QSize(fm.width(cb->currentText())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
        break;
    }
    default:{
        QLineEdit* le = qobject_cast<QLineEdit*>(editor);
        model->setData(index, le->text(), Qt::DisplayRole);
        model->setData(index, QSize(fm.width( le->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
        break;
    }
    }

    model->setData(index.sibling(index.row(), 0), true, RailsModel::ChangedRole);
}
