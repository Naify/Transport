#include "ShuttleTaskForm/ShuttleItemDelegate.h"
#include "ShuttleTaskForm/ShuttleModel.h"

#include <QComboBox>
#include <QLineEdit>
#include <QDebug>
#include <QTextEdit>
#include <OdsInterface>
#include <CatalogManager.h>

ShuttleItemDelegate::ShuttleItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}
//
ShuttleItemDelegate::~ShuttleItemDelegate()
{

}
//
//
QWidget* ShuttleItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/,
                      const QModelIndex& index) const
{
    switch(index.data(ShuttleModel::TypeRole).toInt())
    {
        case ShuttleModel::Pa:
        {
            QComboBox* editor = new QComboBox(parent);
            editor->setEditable(true);
            editor->setInsertPolicy(QComboBox::NoInsert);

            IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
            IRS::Catalog::ptr catalog_divs = catalMgr.getCatalogByName("Классификатор ЖД станций");

            if (catalog_divs)
            {
                IRS::CatalogEntry::ptrList divs_list = catalog_divs->entries();
                for (IRS::CatalogEntry::ptrList::iterator it = divs_list.begin(); it != divs_list.end(); ++it){
                    IRS::CatalogEntry::ptr div = (*it);
                    if (div->value("ПА").toBool())
                    {
                        editor->addItem(div->stringValue("Наименование"),QVariant(div->id()));
                    }
                }
            } else {
                QLineEdit* editor = new QLineEdit(parent);
                editor->setText(index.data(Qt::DisplayRole).toString());
                return editor;
            }

            editor->setCurrentIndex(editor->findText(index.data().toString()));
            return editor;
        }
        case ShuttleModel::Pnvt:
        {
            QComboBox* editor = new QComboBox(parent);
            editor->setEditable(true);
            editor->setInsertPolicy(QComboBox::NoInsert);

            IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
            IRS::Catalog::ptr catalog_divs = catalMgr.getCatalogByName("Классификатор ЖД станций");

            if (catalog_divs)
            {
                IRS::CatalogEntry::ptrList divs_list = catalog_divs->entries();
                for (IRS::CatalogEntry::ptrList::iterator it = divs_list.begin(); it != divs_list.end(); ++it){
                    IRS::CatalogEntry::ptr div = (*it);
                    if (div->value("Вода").toBool() && div->value("Уголь").toBool())
                    {
                        editor->addItem(div->stringValue("Наименование"),QVariant(div->id()));
                    }
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
//
void ShuttleItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                  const QModelIndex& index) const
{
    QFontMetrics fm(editor->font());

    switch(index.data(ShuttleModel::TypeRole).toInt())
    {
        case ShuttleModel::Pa:
        case ShuttleModel::Pnvt:
        {
            QComboBox* cb = qobject_cast<QComboBox*>(editor);
            model->setData(index, cb->currentText(), Qt::DisplayRole);
            model->setData(index, QSize(fm.width(cb->currentText())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
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

    model->setData(index.sibling(index.row(), 0), true, ShuttleModel::ChangedRole);
}
