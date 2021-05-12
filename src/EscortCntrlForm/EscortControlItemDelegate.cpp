#include "EscortCntrlForm/EscortControlItemDelegate.h"
#include "EscortCntrlForm/EscortControlModel.h"

#include <QDateEdit>
#include <QDate>
#include <QLineEdit>
#include <QDebug>
#include <QTextEdit>

EscortControlItemDelegate::EscortControlItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}
//
EscortControlItemDelegate::~EscortControlItemDelegate()
{

}
//
//
QWidget* EscortControlItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/,
                      const QModelIndex& index) const
{
//    switch(index.data(EscortControlModel::TypeRole).toInt())
//    {
//    case EscortControlModel::In:
//    case EscortControlModel::Out:
//    {
//        QDateEdit* editor = new QDateEdit(parent);
//        editor->setDisplayFormat("dd.MM.yy");
//        editor->setCalendarPopup(true);
//        editor->setDate(QDate::fromString(index.data().toString(), "dd.MM.yy"));
//        return editor;
//    }
//    case EscortControlModel::Comment:
//    {
//        QTextEdit* editor = new QTextEdit(parent);
//        editor->setText(index.data().toString());
//        return editor;
//    }
//    }

    QLineEdit* editor = new QLineEdit(parent);
    editor->setText(index.data(Qt::DisplayRole).toString());
    return editor;
}
//
void EscortControlItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                  const QModelIndex& index) const
{
    QFontMetrics fm(editor->font());

//    switch(index.data(EscortControlModel::TypeRole).toInt())
//    {
//    case EscortControlModel::In:
//    case EscortControlModel::Out:
//    {
//        QDateEdit* de = qobject_cast<QDateEdit*>(editor);
//        model->setData(index, de->date().toString(), Qt::DisplayRole);
//        model->setData(index, QSize(fm.width(de->date().toString())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
//        break;
//    }
//    case EscortControlModel::Comment:
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

    ((EscortControlModel*)model)->save(index);
}
