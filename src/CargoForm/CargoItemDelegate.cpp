#include "CargoForm/CargoItemDelegate.h"
#include "CargoForm/CargoModel.h"

#include <QComboBox>
#include <QLineEdit>
#include <QDebug>
#include <QTextEdit>
#include <CatalogManager.h>

CargoItemDelegate::CargoItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}
//
CargoItemDelegate::~CargoItemDelegate()
{

}
//
//
QWidget* CargoItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/,
                      const QModelIndex& index) const
{
    switch(index.data(CargoModel::TypeRole).toInt())
    {
    case CargoModel::Name:
    {
        QComboBox* editor = new QComboBox(parent);
        editor->setEditable(true);
        editor->setInsertPolicy(QComboBox::NoInsert);

        IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
        IRS::Catalog::ptr catalog_divs = catalMgr.getCatalogByName("Классификатор грузов");

        if (catalog_divs)
        {
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
    case CargoModel::NomNum:
        break;
    case CargoModel::Num:
        break;
    case CargoModel::Container:
        break;
    case CargoModel::SG:
    {
        QComboBox* editor = new QComboBox(parent);
        editor->setEditable(true);
        editor->setInsertPolicy(QComboBox::NoInsert);
        editor->addItem(QString(" "));
        editor->addItem(QString("5"));
        editor->addItem(QString("5-0"));
        editor->addItem(QString("5-H"));
        editor->addItem(QString("5-П"));

        editor->setCurrentIndex(editor->findText(index.data().toString()));
        return editor;
    }
    case CargoModel::Block:
    {
        QComboBox* editor = new QComboBox(parent);
        editor->setEditable(true);
        editor->setInsertPolicy(QComboBox::NoInsert);
        editor->addItem(QString(" "));
        editor->addItem(QString("9(T)"));
        editor->addItem(QString("9(Б)"));
        editor->addItem(QString("9(Б)-ФА"));
        editor->addItem(QString("9(Б)-ДА"));

        editor->setCurrentIndex(editor->findText(index.data().toString()));
        return editor;
    }
    case CargoModel::ContainerSeal:
        break;
    case CargoModel::TzuType:
    {
        QComboBox* editor = new QComboBox(parent);
        editor->setEditable(true);
        editor->setInsertPolicy(QComboBox::NoInsert);

        IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
        IRS::Catalog::ptr catalog_divs = catalMgr.getCatalogByName("Классификатор ТЗУ");

        if (catalog_divs)
        {
            IRS::CatalogEntry::ptrList divs_list = catalog_divs->entries();
            for (IRS::CatalogEntry::ptrList::iterator it = divs_list.begin(); it != divs_list.end(); ++it){
                IRS::CatalogEntry::ptr div = (*it);
                editor->addItem(div->stringValue("Тип ТЗУ"),QVariant(div->id()));
            }
        } else {
            QLineEdit* editor = new QLineEdit(parent);
            editor->setText(index.data(Qt::DisplayRole).toString());
            return editor;
        }
        editor->setCurrentIndex(editor->findText(index.data().toString()));
        return editor;
    }
    case CargoModel::TzuNum:
        break;
    case CargoModel::TzuSeal:
        break;
    case CargoModel::CarType:
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
    case CargoModel::CarNum:
        break;
    case CargoModel::CarSeal:
        break;
    }

    QLineEdit* editor = new QLineEdit(parent);
    editor->setText(index.data(Qt::DisplayRole).toString());
    return editor;
}
//
void CargoItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,const QModelIndex& index) const
{
    QFontMetrics fm(editor->font());

    switch(index.data(CargoModel::TypeRole).toInt())
    {
    case CargoModel::Name:
    {
        QComboBox* cb = qobject_cast<QComboBox*>(editor);

        model->setData(index, cb->currentText(), Qt::DisplayRole);
        model->setData(index, QSize(fm.width(cb->currentText())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);

        int entryId =  cb->itemData(cb->currentIndex()).toInt();
        IRS::CatalogManager catalMgr(ODS::OdsInterface::self());
        IRS::Catalog::ptr catalog_divs = catalMgr.getCatalogByName("Классификатор грузов");
        IRS::CatalogEntry::ptr entry = catalog_divs->entryById(entryId);
        QString cipher = entry->stringValue("Шифр тары");
        QModelIndex indexCipher = index.sibling(index.row(),3);
        model->setData(indexCipher, cipher, Qt::DisplayRole);
        model->setData(indexCipher, QSize(fm.width(cipher)+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);

        break;
    }
    case CargoModel::SG:
    case CargoModel::Block:
    case CargoModel::TzuType:
    case CargoModel::CarType:
    {
        QComboBox* cb = qobject_cast<QComboBox*>(editor);
        model->setData(index, cb->currentText(), Qt::DisplayRole);
        model->setData(index, QSize(fm.width(cb->currentText())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
        break;
    }
//    case CargoModel::NomNum:
//    case CargoModel::Num:
//    case CargoModel::Container:
//    case CargoModel::ContainerSeal:
//    case CargoModel::TzuNum:
//    case CargoModel::TzuSeal:
//    case CargoModel::CarNum:
//    case CargoModel::CarSeal:
    default:
    {
        QLineEdit* le = qobject_cast<QLineEdit*>(editor);
        model->setData(index, le->text(), Qt::DisplayRole);
        model->setData(index, QSize(fm.width( le->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
        break;
    }
    }

    model->setData(index.sibling(index.row(), 0), true, CargoModel::ChangedRole);
}
//
