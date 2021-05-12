#include "firstzdedelegate.h"
#include "firstzdemodel.h"

#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <CatalogManager.h>
#include <QDateEdit>

FirstZdeDelegate::FirstZdeDelegate(QObject *parent):
    QItemDelegate(parent)
{
}

FirstZdeDelegate::~FirstZdeDelegate()
{
}

QWidget *FirstZdeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
    switch(index.data(FirstZdeModel::TypeRole).toInt())
    {
    case FirstZdeModel::Num:
    {
        QSpinBox* spb = new QSpinBox(parent);
        spb->setValue(index.data(Qt::DisplayRole).toInt());
        return spb;
    }
    case FirstZdeModel::Rank:
    case FirstZdeModel::Post:
    case FirstZdeModel::Fio:
    case FirstZdeModel::Date:
    {
        QLineEdit* editor = new QLineEdit(parent);
        editor->setReadOnly(false);
        editor->setText(index.data(Qt::DisplayRole).toString());
        return editor;
    }
    case FirstZdeModel::NumAndDate:
    {
        QLineEdit* editor = new QLineEdit(parent);
        editor->setText(index.data(Qt::DisplayRole).toString());
        return editor;
    }
    case FirstZdeModel::Term:
    {
        QSpinBox* spb = new QSpinBox(parent);
        spb->setValue(index.data(Qt::DisplayRole).toInt());
        return spb;
    }
    case FirstZdeModel::DateEnd:
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
    }
    QLineEdit* editor = new QLineEdit(parent);
    editor->setText(index.data(Qt::DisplayRole).toString());
    return editor;
}

void FirstZdeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QFontMetrics fm(editor->font());

    switch(index.data(FirstZdeModel::TypeRole).toInt())
    {
    case FirstZdeModel::Num:{
        QSpinBox* spb = qobject_cast<QSpinBox*>(editor);
        model->setData(index, QString::number(spb->value()), Qt::DisplayRole);
        break;
    }
    case FirstZdeModel::Rank:
    case FirstZdeModel::Post:
    case FirstZdeModel::Fio:
    case FirstZdeModel::Date:
        break;
//    case FirstZdeModel::NumAndDate:
    case FirstZdeModel::Term:{
        QSpinBox* spb = qobject_cast<QSpinBox*>(editor);
        model->setData(index, QString::number(spb->value()), Qt::DisplayRole);
        break;
    }
    case FirstZdeModel::DateEnd:{
            QDateEdit* de = qobject_cast<QDateEdit*>(editor);
            model->setData(index, de->date().toString("dd.MM.yyyy"), Qt::DisplayRole);
            break;
    }
    default:
    QLineEdit* le = qobject_cast<QLineEdit*>(editor);
    model->setData(index, le->text(), Qt::DisplayRole);
    model->setData(index, QSize(fm.width( le->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    break;
    }

    model->setData(index.sibling(index.row(), 0), true, FirstZdeModel::ChangedRole);
}
