#include "secondzdedelegate.h"
#include "secondzdemodel.h"

#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <CatalogManager.h>
#include <QDateEdit>

SecondZdeDelegate::SecondZdeDelegate(QObject *parent):
    QItemDelegate(parent)
{
}

SecondZdeDelegate::~SecondZdeDelegate()
{
}

QWidget *SecondZdeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
    switch(index.data(SecondZdeModel::TypeRole).toInt())
    {
    case SecondZdeModel::Num:
    {
        QSpinBox* spb = new QSpinBox(parent);
        spb->setValue(index.data(Qt::DisplayRole).toInt());
        return spb;
    }
    case SecondZdeModel::Rank:
    case SecondZdeModel::Post:
    case SecondZdeModel::Fio:
    case SecondZdeModel::Date:
    {
        QLineEdit* editor = new QLineEdit(parent);
        editor->setReadOnly(false);
        editor->setText(index.data(Qt::DisplayRole).toString());
        return editor;
    }
    case SecondZdeModel::NumAndDate:
    {
        QLineEdit* editor = new QLineEdit(parent);
        editor->setText(index.data(Qt::DisplayRole).toString());
        return editor;
    }
    case SecondZdeModel::Term:
    {
        QSpinBox* spb = new QSpinBox(parent);
        spb->setValue(index.data(Qt::DisplayRole).toInt());
        return spb;
    }
    case SecondZdeModel::DateEnd:
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

void SecondZdeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QFontMetrics fm(editor->font());

    switch(index.data(SecondZdeModel::TypeRole).toInt())
    {
    case SecondZdeModel::Num:{
        QSpinBox* spb = qobject_cast<QSpinBox*>(editor);
        model->setData(index, QString::number(spb->value()), Qt::DisplayRole);
        break;
    }
    case SecondZdeModel::Rank:
    case SecondZdeModel::Post:
    case SecondZdeModel::Fio:
    case SecondZdeModel::Date:
        break;
//    case SecondZdeModel::NumAndDate:
    case SecondZdeModel::Term:{
        QSpinBox* spb = qobject_cast<QSpinBox*>(editor);
        model->setData(index, QString::number(spb->value()), Qt::DisplayRole);
        break;
    }
    case SecondZdeModel::DateEnd:{
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

    model->setData(index.sibling(index.row(), 0), true, SecondZdeModel::ChangedRole);
}
