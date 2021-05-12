#include "showescortdialogdelegate.h"
#include "showescortdialogmodel.h"

#include <QLineEdit>

ShowEscortDialogDelegate::ShowEscortDialogDelegate(QObject *parent):
     QItemDelegate(parent)
{
}

ShowEscortDialogDelegate::~ShowEscortDialogDelegate()
{

}

QWidget *ShowEscortDialogDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
    QLineEdit* editor = new QLineEdit(parent);
    editor->setText(index.data(Qt::DisplayRole).toString());
    return editor;
}

void ShowEscortDialogDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QFontMetrics fm(editor->font());

    switch(index.data(ShowEscortDialogModel::TypeRole).toInt())
    {
    default:{
        QLineEdit* le = qobject_cast<QLineEdit*>(editor);
        model->setData(index, le->text(), Qt::DisplayRole);
        model->setData(index, QSize(fm.width( le->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
        break;
    }
    }
    model->setData(index.sibling(index.row(), 0), true, ShowEscortDialogModel::ChangedRole);
}
