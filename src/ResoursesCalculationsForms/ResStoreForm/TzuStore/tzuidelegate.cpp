#include "tzuidelegate.h"
#include "tzuskmodel.h"

#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <CatalogManager.h>
#include <QDateEdit>

TzuIDelegate::TzuIDelegate(QObject *parent):
    QItemDelegate(parent)
{
}

TzuIDelegate::~TzuIDelegate()
{
}

QWidget *TzuIDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
    switch(index.data(TzuSkModel::TypeRole).toInt())
    {
    case TzuSkModel::Num:
    {
        QSpinBox* spb = new QSpinBox(parent);
        spb->setValue(index.data(Qt::DisplayRole).toInt());
        return spb;
    }
    case TzuSkModel::Designation:
    {
        QComboBox* editor = new QComboBox(parent);
        editor->setEditable(false);
        editor->setInsertPolicy(QComboBox::NoInsert);

        IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
        IRS::Catalog::ptr catalog_divs = catalMgr.getCatalogByName("Классификатор контейнеров");

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
    case TzuSkModel::DateRelease:
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
    case TzuSkModel::Ready:
    {
        QComboBox* editor = new QComboBox(parent);
        editor->setEditable(false);
        editor->setInsertPolicy(QComboBox::NoInsert);
        editor->addItem("Готов");
        editor->addItem("Не готов");
        return editor;
    }
    case TzuSkModel::Away:{
        QComboBox* editor = new QComboBox(parent);
        editor->setEditable(false);
        editor->setInsertPolicy(QComboBox::NoInsert);
        editor->addItem("Присутствует");
        editor->addItem("Отсутствует");        
        return editor;
    }
    }
    QLineEdit* editor = new QLineEdit(parent);
    editor->setText(index.data(Qt::DisplayRole).toString());
    return editor;
}

void TzuIDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QFontMetrics fm(editor->font());

    switch(index.data(TzuSkModel::TypeRole).toInt())
    {
    case TzuSkModel::Num:{
        QSpinBox* spb = qobject_cast<QSpinBox*>(editor);
        model->setData(index, QString::number(spb->value()), Qt::DisplayRole);
        break;
    }
    case TzuSkModel::Designation:{
        QComboBox* cb = qobject_cast<QComboBox*>(editor);
        model->setData(index, cb->currentText(), Qt::DisplayRole);
        model->setData(index, QSize(fm.width(cb->currentText())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
        model->setData(index, cb->itemData(cb->currentIndex()).toInt(), TzuSkModel::DesignatRole);
        break;
    }
    case TzuSkModel::DateRelease:{
        QDateEdit* de = qobject_cast<QDateEdit*>(editor);
        model->setData(index, de->date().toString("dd.MM.yyyy"), Qt::DisplayRole);
        break;
    }
    case TzuSkModel::Away:
    case TzuSkModel::Ready:{
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

    model->setData(index.sibling(index.row(), 0), true, TzuSkModel::ChangedRole);
}
