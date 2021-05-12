#include "EscortForm/EscortItemDelegate.h"
#include "EscortForm/EscortModel.h"

#include <QComboBox>
#include <QLineEdit>
#include <QDebug>
#include <QTextEdit>
#include <OdsInterface>
#include <CatalogManager.h>

EscortItemDelegate::EscortItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}
//
EscortItemDelegate::~EscortItemDelegate()
{

}
//
//
QWidget* EscortItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/,
                      const QModelIndex& index) const
{
    switch(index.data(EscortModel::TypeRole).toInt())
    {
        case EscortModel::Post:
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
        }
        case EscortModel::Rank:
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
        case EscortModel::LastName:
        case EscortModel::FirstName:
        case EscortModel::MidName:
            break;
        case EscortModel::With:
        {
//            QComboBox* editor = new QComboBox(parent);
//            editor->setEditable(true);
//            editor->setInsertPolicy(QComboBox::NoInsert);

//            const QAbstractItemModel* model = index.model();
//            for (int i = 0; i<model->rowCount(); i++)
//            {
//                QModelIndex modelIndex = model->index(i, 0);
//                QModelIndex modelNameIndex = model->index(i, 2);

//                if (!modelNameIndex.data().toString().isEmpty() && modelIndex.row() != index.row())
//                    editor->addItem(QString("%1 %2")
//                                    .arg(modelIndex.data().toString())
//                                    .arg(modelNameIndex.data().toString()), modelIndex.data(EscortModel::IdRole).toInt());
//            }

//            editor->setCurrentIndex(editor->findText(index.data().toString()));
//            return editor;
            break;
        }
        case EscortModel::WeaponName:
        case EscortModel::WeaponNum:
        {
//            QTextEdit* editor = new QTextEdit(parent);
//            editor->setText(index.data().toString());
//            return editor;
            break;
        }
        case EscortModel::CarType:
        {
            QComboBox* editor = new QComboBox(parent);
            editor->setEditable(true);
            editor->setInsertPolicy(QComboBox::NoInsert);

            QString querySQL = "SELECT DISTINCT \"Транспортные средства\".\"Вагоны\".\"Назначение\" as \"Назначение\" FROM \"Транспортные средства\".\"Вагоны\"";

            ODS::IORecordCursor rec_cursor = ODS::OdsInterface::self()->iobjectManager().exec(querySQL);
            for (ODS::IORecordCursor::iterator it = rec_cursor.begin(); it != rec_cursor.end(); ++it) {
                ODS::IORecord iobj = *it;

                QString numb = iobj.getStringAttr("Назначение");
                editor->addItem(numb);
            }

            editor->setCurrentIndex(editor->findText(index.data().toString()));
            return editor;
        }
        case EscortModel::CarNum:
        {
            QComboBox* editor = new QComboBox(parent);
            editor->setEditable(true);
            editor->setInsertPolicy(QComboBox::NoInsert);

            QString querySQL = "SELECT DISTINCT \"Транспортные средства\".\"Вагоны\".\"Регистр. номер\" as \"РН\" FROM \"Транспортные средства\".\"Вагоны\"";

            ODS::IORecordCursor rec_cursor = ODS::OdsInterface::self()->iobjectManager().exec(querySQL);
            for (ODS::IORecordCursor::iterator it = rec_cursor.begin(); it != rec_cursor.end(); ++it) {
                ODS::IORecord iobj = *it;

                QString numb = iobj.getStringAttr("РН");
                editor->addItem(numb);
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
void EscortItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                  const QModelIndex& index) const
{
    QFontMetrics fm(editor->font());

    switch(index.data(EscortModel::TypeRole).toInt())
    {
        case EscortModel::Post:
        case EscortModel::Rank:
        case EscortModel::CarType:
        case EscortModel::CarNum:
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

    model->setData(index.sibling(index.row(), 0), true, EscortModel::ChangedRole);
}
