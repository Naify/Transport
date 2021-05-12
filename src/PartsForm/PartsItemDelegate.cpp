#include "PartsForm/PartsItemDelegate.h"
#include "PartsForm/PartsModel.h"

#include <QComboBox>
#include <QLineEdit>
#include <QDebug>
#include <QTextEdit>
#include <QSpinBox>

PartsItemDelegate::PartsItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}
//
PartsItemDelegate::~PartsItemDelegate()
{

}
//
//
QWidget* PartsItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/,
                      const QModelIndex& index) const
{
//    switch(index.data(PartsModel::TypeRole).toInt())
//    {
//    case PartsModel::Name:
//    {
//        QComboBox* editor = new QComboBox(parent);
//        editor->setEditable(true);
//        editor->setInsertPolicy(QComboBox::NoInsert);
//        editor->addItem("Блок автоматики");

//        editor->setCurrentIndex(editor->findText(index.data().toString()));
//        return editor;
//    }
//    case PartsModel::Index:
//    {
//        QComboBox* editor = new QComboBox(parent);
//        editor->setEditable(true);
//        editor->setInsertPolicy(QComboBox::NoInsert);
//        editor->addItem("ТБА-7777");

//        editor->setCurrentIndex(editor->findText(index.data().toString()));
//        return editor;
//    }
//    case PartsModel::Num:
//        break;
//    case PartsModel::Grif:
//    {
//        QComboBox* editor = new QComboBox(parent);
//        editor->setEditable(true);
//        editor->setInsertPolicy(QComboBox::NoInsert);
//        editor->addItem("нс", 0);
//        editor->addItem("с", 1);
//        editor->addItem("сс", 2);

//        editor->setCurrentIndex(editor->findData(index.data(PartsModel::GrifRole).toInt()));
//        return editor;
//    }
//    case PartsModel::Code:
//        break;
//    case PartsModel::Seal:
//    case PartsModel::SealCount:
//    case PartsModel::Comment:
//    {
//        QTextEdit* editor = new QTextEdit(parent);
//        editor->setText(index.data().toString());
//        return editor;
//    }
//    case PartsModel::PlaceCount:
//    {
//        QSpinBox* editor = new QSpinBox(parent);
//        editor->setMinimum(1);
//        editor->setMaximum(99);

//        editor->setValue(index.data(Qt::DisplayRole).toInt());
//        return editor;
//    }
//    case PartsModel::CarNum:
//        break;
//    }

    QLineEdit* editor = new QLineEdit(parent);
    editor->setText(index.data(Qt::DisplayRole).toString());
    return editor;
}
//
void PartsItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                  const QModelIndex& index) const
{
    QFontMetrics fm(editor->font());

//    switch(index.data(PartsModel::TypeRole).toInt())
//    {
//    case PartsModel::Name:
//    case PartsModel::Index:
//    {
//        QComboBox* cb = qobject_cast<QComboBox*>(editor);
//        model->setData(index, cb->currentText(), Qt::DisplayRole);
//        model->setData(index, QSize(fm.width(cb->currentText())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
//        break;
//    }
//    case PartsModel::PlaceCount:
//    {
//        QSpinBox* sb = qobject_cast<QSpinBox*>(editor);
//        model->setData(index, QString::number(sb->value()), Qt::DisplayRole);
//        model->setData(index, QSize(fm.width(index.data().toString())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
//        break;
//    }
//    case PartsModel::Seal:
//    case PartsModel::SealCount:
//    case PartsModel::Comment:
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
//    case PartsModel::Grif:
//    {
//        QComboBox* cb = qobject_cast<QComboBox*>(editor);
//        model->setData(index, cb->currentText(), Qt::DisplayRole);
//        model->setData(index, cb->itemData(cb->currentIndex()).toInt(), PartsModel::GrifRole);
//        model->setData(index, QSize(fm.width(cb->currentText())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
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

    model->setData(index.sibling(index.row(), 0), true, PartsModel::ChangedRole);
}
//
