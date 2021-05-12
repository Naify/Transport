#include "AutoForm/AutoTransportItemDelegate.h"
#include "AutoForm/AutoTransportModel.h"

#include <QComboBox>
#include <QLineEdit>
#include <QDebug>
#include <QSpinBox>
#include <CatalogManager.h>

AutoTransportItemDelegate::AutoTransportItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}
//
AutoTransportItemDelegate::~AutoTransportItemDelegate()
{

}
//
//
QWidget* AutoTransportItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/,
                      const QModelIndex& index) const
{
    switch(index.data(AutoTransportModel::TypeRole).toInt())
    {
        case AutoTransportModel::Post:
            {
            QComboBox* editor = new QComboBox(parent);
            editor->setEditable(true);
            editor->setInsertPolicy(QComboBox::NoInsert);

            IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
            IRS::Catalog::ptr catalog_divs = catalMgr.getCatalogByName("Классификатор должностей в составе команд сопровождения");

            if (catalog_divs)
            {
                IRS::CatalogEntry::ptrList divs_list = catalog_divs->entries();
                for (IRS::CatalogEntry::ptrList::iterator it = divs_list.begin(); it != divs_list.end(); ++it){
                    IRS::CatalogEntry::ptr div = (*it);
                    editor->addItem(div->stringValue("Сокращенное обозначение"),QVariant(div->id()));
                }
            } else {
                QLineEdit* editor = new QLineEdit(parent);
                editor->setText(index.data(Qt::DisplayRole).toString());
                return editor;
            }

            editor->setCurrentIndex(editor->findText(index.data().toString()));
            return editor;
                break;
            }
        case AutoTransportModel::Rank:
            {
                QComboBox* editor = new QComboBox(parent);
                editor->setEditable(true);
                editor->setInsertPolicy(QComboBox::NoInsert);

                IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
                IRS::Catalog::ptr catalog_divs = catalMgr.getCatalogByName("Классификатор воинских званий");

                if (catalog_divs)
                {
                    IRS::CatalogEntry::ptrList divs_list = catalog_divs->entries();
                    for (IRS::CatalogEntry::ptrList::iterator it = divs_list.begin(); it != divs_list.end(); ++it){
                        IRS::CatalogEntry::ptr div = (*it);
                        editor->addItem(div->stringValue("Наименование"),QVariant(div->id()));
                    }
                } else {
                    return new QComboBox();
                }
                editor->setCurrentIndex(editor->findText(index.data().toString()));
                return editor;
                break;
            }
        case AutoTransportModel::Mission:
        {
            QComboBox* editor = new QComboBox(parent);
            editor->setEditable(true);
            editor->setInsertPolicy(QComboBox::NoInsert);

            IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
            IRS::Catalog::ptr catalog_divs = catalMgr.getCatalogByName("Классификатор автомобильной техники");

            if (catalog_divs)
            {
                IRS::CatalogEntry::ptrList divs_list = catalog_divs->entries();
                for (IRS::CatalogEntry::ptrList::iterator it = divs_list.begin(); it != divs_list.end(); ++it){
                    IRS::CatalogEntry::ptr div = (*it);
                    editor->addItem(div->stringValue("Назначение"),QVariant(div->id()));
                }
            } else {
                QLineEdit* editor = new QLineEdit(parent);
                editor->setText(index.data(Qt::DisplayRole).toString());
                return editor;
            }
            editor->setCurrentIndex(editor->findText(index.data().toString()));
            return editor;
        }
        case AutoTransportModel::CarType:
        {
            QComboBox* editor = new QComboBox(parent);
            editor->setEditable(true);
            editor->setInsertPolicy(QComboBox::NoInsert);

            IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
            IRS::Catalog::ptr catalog_divs = catalMgr.getCatalogByName("Классификатор автомобильной техники");

            if (catalog_divs)
            {
                IRS::CatalogEntry::ptrList divs_list = catalog_divs->entries();
                for (IRS::CatalogEntry::ptrList::iterator it = divs_list.begin(); it != divs_list.end(); ++it){
                    IRS::CatalogEntry::ptr div = (*it);
                    editor->addItem(div->stringValue("Марка"),QVariant(div->id()));
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

void AutoTransportItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                  const QModelIndex& index) const
{
    QFontMetrics fm(editor->font());

    switch(index.data(AutoTransportModel::TypeRole).toInt())
    {
        case AutoTransportModel::Rank:
        case AutoTransportModel::Post:
        case AutoTransportModel::CarType:
        case AutoTransportModel::Mission:
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

    model->setData(index.sibling(index.row(), 0), true, AutoTransportModel::ChangedRole);
}
//
