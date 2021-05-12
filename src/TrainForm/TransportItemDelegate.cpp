#include "TrainForm/TransportItemDelegate.h"
#include "TrainForm/TransportModel.h"

#include <QComboBox>
#include <QLineEdit>
#include <QDebug>
#include <QSpinBox>
#include <CatalogManager.h>

TransportItemDelegate::TransportItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}
//
TransportItemDelegate::~TransportItemDelegate()
{

}
//
//
QWidget* TransportItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/,
                      const QModelIndex& index) const
{
    switch(index.data(TransportModel::TypeRole).toInt())
    {
        case TransportModel::CarType:
        {
            QComboBox* editor = new QComboBox(parent);
            editor->setEditable(true);
            editor->setInsertPolicy(QComboBox::NoInsert);

            IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
            IRS::Catalog::ptr catalog_divs = catalMgr.getCatalogByName("Классификатор вагонов по типам");

            if (catalog_divs)
            {
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
            return editor;
        }
        case TransportModel::Mission:
        {
            QComboBox* editor = new QComboBox(parent);
            editor->setEditable(true);
            editor->setInsertPolicy(QComboBox::NoInsert);

            IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
            IRS::Catalog::ptr catalog_divs = catalMgr.getCatalogByName("Классификатор вагонов по назначению");

            if (catalog_divs)
            {
                IRS::CatalogEntry::ptrList divs_list = catalog_divs->entries();
                for (IRS::CatalogEntry::ptrList::iterator it = divs_list.begin(); it != divs_list.end(); ++it){
                    IRS::CatalogEntry::ptr div = (*it);
                    editor->addItem(div->stringValue("Обозначение"),QVariant(div->id()));
                }
            } else {
                QLineEdit* editor = new QLineEdit(parent);
                editor->setText(index.data(Qt::DisplayRole).toString());
                return editor;
            }

            editor->setCurrentIndex(editor->findText(index.data().toString()));
            return editor;
        }
        case TransportModel::Flank:
        {
            QComboBox* editor = new QComboBox(parent);
            editor->setEditable(true);
            editor->setInsertPolicy(QComboBox::NoInsert);
            editor->addItem("Лев");
            editor->addItem("Прав");

            editor->setCurrentIndex(editor->findText(index.data().toString()));
            return editor;
        }
        case TransportModel::FlankNum:
        {
            QSpinBox* editor = new QSpinBox(parent);
            editor->setMinimum(1);
            editor->setMaximum(99);

            editor->setValue(index.data(Qt::DisplayRole).toInt());
            return editor;
        }
    }

    QLineEdit* editor = new QLineEdit(parent);
    editor->setText(index.data(Qt::DisplayRole).toString());
    return editor;
}
//
void TransportItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                  const QModelIndex& index) const
{
    QFontMetrics fm(editor->font());

    switch(index.data(TransportModel::TypeRole).toInt())
    {
        case TransportModel::CarType:
        case TransportModel::Mission:
        case TransportModel::Flank:
        {
            QComboBox* cb = qobject_cast<QComboBox*>(editor);
            model->setData(index, cb->currentText(), Qt::DisplayRole);
            model->setData(index, QSize(fm.width(cb->currentText())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
            break;
        }
        case TransportModel::FlankNum:
        {
            QSpinBox* sb = qobject_cast<QSpinBox*>(editor);
            model->setData(index, QString::number(sb->value()), Qt::DisplayRole);
            model->setData(index, QSize(fm.width(index.data().toString())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
            break;
        }
        default:
        {
            QLineEdit* le = qobject_cast<QLineEdit*>(editor);
            model->setData(index, le->text(), Qt::DisplayRole);
            model->setData(index, QSize(fm.width( le->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
            break;
        }
    }

    model->setData(index.sibling(index.row(), 0), true, TransportModel::ChangedRole);
}
//
