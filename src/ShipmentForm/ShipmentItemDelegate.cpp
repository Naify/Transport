#include "ShipmentForm/ShipmentItemDelegate.h"
#include "ShipmentForm/ShipmentModel.h"

#include <QComboBox>
#include <QLineEdit>
#include <QDebug>
#include <QTextEdit>

ShipmentItemDelegate::ShipmentItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}
//
ShipmentItemDelegate::~ShipmentItemDelegate()
{

}
//
//
QWidget* ShipmentItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/,
                      const QModelIndex& index) const
{
//    switch(index.data(CargoModel::TypeRole).toInt())
//    {
//    case CargoModel::Name:
//    {
//        QComboBox* editor = new QComboBox(parent);
//        editor->setEditable(true);
//        editor->setInsertPolicy(QComboBox::NoInsert);
//        editor->addItem("ВН175-01");

//        editor->setCurrentIndex(editor->findText(index.data().toString()));
//        return editor;
//    }
//    case CargoModel::Num:
//        break;
//    case CargoModel::Container:
//    {
//        QComboBox* editor = new QComboBox(parent);
//        editor->setEditable(true);
//        editor->setInsertPolicy(QComboBox::NoInsert);
//        editor->addItem("ББ837");

//        editor->setCurrentIndex(editor->findText(index.data().toString()));
//        return editor;
//    }
//    case CargoModel::SG:
//        break;
//    case CargoModel::Block:
//    case CargoModel::ContainerSeal:
//    case CargoModel::TzuSeal:
//    case CargoModel::CarSeal:
//    {
//        QTextEdit* editor = new QTextEdit(parent);
//        editor->setText(index.data().toString());
//        return editor;
//    }
//    case CargoModel::TzuType:
//    {
//        QComboBox* editor = new QComboBox(parent);
//        editor->setEditable(true);
//        editor->setInsertPolicy(QComboBox::NoInsert);
//        editor->addItem("УЗ-872");

//        editor->setCurrentIndex(editor->findText(index.data().toString()));
//        return editor;
//    }
//    case CargoModel::TzuNum:
//        break;
//    case CargoModel::CarType:
//    {
//        QComboBox* editor = new QComboBox(parent);
//        editor->setEditable(true);
//        editor->setInsertPolicy(QComboBox::NoInsert);
//        editor->addItem("В60М2");

//        editor->setCurrentIndex(editor->findText(index.data().toString()));
//        return editor;
//    }
//    case CargoModel::CarNum:
//        break;
//    }

    QLineEdit* editor = new QLineEdit(parent);
    editor->setText(index.data(Qt::DisplayRole).toString());
    return editor;
}
//
void ShipmentItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                  const QModelIndex& index) const
{
    QFontMetrics fm(editor->font());

//    switch(index.data(CargoModel::TypeRole).toInt())
//    {
//    case CargoModel::Name:
//    case CargoModel::Container:
//    case CargoModel::TzuType:
//    case CargoModel::CarType:
//    {
//        QComboBox* cb = qobject_cast<QComboBox*>(editor);
//        model->setData(index, cb->currentText(), Qt::DisplayRole);
//        model->setData(index, QSize(fm.width(cb->currentText())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
//        break;
//    }
//    case CargoModel::Block:
//    case CargoModel::ContainerSeal:
//    case CargoModel::TzuSeal:
//    case CargoModel::CarSeal:
//    {
//        QTextEdit* te = qobject_cast<QTextEdit*>(editor);
//        QString text = te->toPlainText();

//        model->setData(index, text, Qt::DisplayRole);

//        QTextDocument* doc = te->document();

//        //qDebug()<<text<<text.count('\n')+1<<doc->textWidth()<<doc->lineCount();

//        QSize size = QSize(doc->size().width(), (text.count('\n')+1)*fm.height()+4);

//        model->setData(index, size, Qt::SizeHintRole);
//        break;
//    }
//    default:
//    {
        QLineEdit* le = qobject_cast<QLineEdit*>(editor);
        model->setData(index, le->text(), Qt::DisplayRole);
        model->setData(index, QSize(fm.width( le->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
//        break;
//    }
//    }

    model->setData(index.sibling(index.row(), 0), true, ShipmentModel::ChangedRole);
}
//
