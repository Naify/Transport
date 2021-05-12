#include "suppmachinesidelegate.h"
#include "supportmachinesmodel.h"

#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <CatalogManager.h>
#include <QDateEdit>

SuppMachinesIDelegate::SuppMachinesIDelegate(QObject *parent):
    QItemDelegate(parent)
{
}

SuppMachinesIDelegate::~SuppMachinesIDelegate()
{
}

QWidget *SuppMachinesIDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
    switch(index.data(SupportMachinesModel::TypeRole).toInt())
    {
    case SupportMachinesModel::Num:
    {
        QSpinBox* spb = new QSpinBox(parent);
        spb->setValue(index.data(Qt::DisplayRole).toInt());
        return spb;
    }
    case SupportMachinesModel::DateRelease:
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
    case SupportMachinesModel::Ready:
    {
        QComboBox* editor = new QComboBox(parent);
        editor->setEditable(false);
        editor->setInsertPolicy(QComboBox::NoInsert);
        editor->addItem("Готов");
        editor->addItem("Не готов");
        return editor;
    }
    }
    QLineEdit* editor = new QLineEdit(parent);
    editor->setText(index.data(Qt::DisplayRole).toString());
    return editor;
}

void SuppMachinesIDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QFontMetrics fm(editor->font());

    switch(index.data(SupportMachinesModel::TypeRole).toInt())
    {
    case SupportMachinesModel::Num:{
        QSpinBox* spb = qobject_cast<QSpinBox*>(editor);
        model->setData(index, QString::number(spb->value()), Qt::DisplayRole);
        break;
    }
    case SupportMachinesModel::DateRelease:{
        QDateEdit* de = qobject_cast<QDateEdit*>(editor);
        model->setData(index, de->date().toString("dd.MM.yyyy"), Qt::DisplayRole);
        break;
    }
    case SupportMachinesModel::Ready:{
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

    model->setData(index.sibling(index.row(), 0), true, SupportMachinesModel::ChangedRole);
}
