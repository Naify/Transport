#include "znkitemdelegate.h"
#include "znkadmissionmodel.h"

#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <CatalogManager.h>
#include <QDateEdit>

ZnkItemDelegate::ZnkItemDelegate(QObject *parent):
    QItemDelegate(parent)
{
}

ZnkItemDelegate::~ZnkItemDelegate()
{
}

QWidget *ZnkItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
    switch(index.data(ZnkAdmissionModel::TypeRole).toInt())
    {
    case ZnkAdmissionModel::Num:
    {
        QSpinBox* spb = new QSpinBox(parent);
        spb->setValue(index.data(Qt::DisplayRole).toInt());
        return spb;
    }
    case ZnkAdmissionModel::Rank:
    case ZnkAdmissionModel::Post:
    case ZnkAdmissionModel::Fio:
    case ZnkAdmissionModel::Date:
    {
        QLineEdit* editor = new QLineEdit(parent);
        editor->setReadOnly(false);
        editor->setText(index.data(Qt::DisplayRole).toString());
        return editor;
    }
    case ZnkAdmissionModel::NumAndDate:
    {
        QLineEdit* editor = new QLineEdit(parent);
        editor->setText(index.data(Qt::DisplayRole).toString());
        return editor;
    }
    case ZnkAdmissionModel::Term:
    {
        QSpinBox* spb = new QSpinBox(parent);
        spb->setValue(index.data(Qt::DisplayRole).toInt());
        return spb;
    }
    case ZnkAdmissionModel::DateEnd:
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

void ZnkItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QFontMetrics fm(editor->font());

    switch(index.data(ZnkAdmissionModel::TypeRole).toInt())
    {
    case ZnkAdmissionModel::Num:{
        QSpinBox* spb = qobject_cast<QSpinBox*>(editor);
        model->setData(index, QString::number(spb->value()), Qt::DisplayRole);
        break;
    }
    case ZnkAdmissionModel::Rank:
    case ZnkAdmissionModel::Post:
    case ZnkAdmissionModel::Fio:
    case ZnkAdmissionModel::Date:
        break;
//    case ZnkAdmissionModel::NumAndDate:
    case ZnkAdmissionModel::Term:{
        QSpinBox* spb = qobject_cast<QSpinBox*>(editor);
        model->setData(index, QString::number(spb->value()), Qt::DisplayRole);
        break;
    }
    case ZnkAdmissionModel::DateEnd:{
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

    model->setData(index.sibling(index.row(), 0), true, ZnkAdmissionModel::ChangedRole);
}
