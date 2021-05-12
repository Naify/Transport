#include "agroupitemdelegate.h"
#include "agroupadmissionmodel.h"

#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <CatalogManager.h>
#include <QDateEdit>

AGroupItemDelegate::AGroupItemDelegate(QObject *parent):
    QItemDelegate(parent)
{
}

AGroupItemDelegate::~AGroupItemDelegate()
{
}

QWidget *AGroupItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
    switch(index.data(AGroupAdmissionModel::TypeRole).toInt())
    {
    case AGroupAdmissionModel::Num:
    {
        QSpinBox* spb = new QSpinBox(parent);
        spb->setValue(index.data(Qt::DisplayRole).toInt());
        return spb;
    }
    case AGroupAdmissionModel::Rank:
    case AGroupAdmissionModel::Post:
    case AGroupAdmissionModel::Fio:
    case AGroupAdmissionModel::Date:
    {
        QLineEdit* editor = new QLineEdit(parent);
        editor->setReadOnly(false);
        editor->setText(index.data(Qt::DisplayRole).toString());
        return editor;
    }
    case AGroupAdmissionModel::Date2:
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
    case AGroupAdmissionModel::Result:
    {
        QComboBox* editor = new QComboBox(parent);
        editor->setEditable(true);
        editor->setInsertPolicy(QComboBox::NoInsert);
        editor->addItem("Допущен", 1);
        editor->addItem("Не допущен", 0);
        editor->setCurrentIndex(editor->findText(index.data().toString()));
        return editor;
    }
    case AGroupAdmissionModel::NumAndDate:
    {
        QLineEdit* editor = new QLineEdit(parent);
        editor->setReadOnly(false);
        editor->setText(index.data(Qt::DisplayRole).toString());
        return editor;
    }
    case AGroupAdmissionModel::DateEnd:
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

void AGroupItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QFontMetrics fm(editor->font());

    switch(index.data(AGroupAdmissionModel::TypeRole).toInt())
    {
    case AGroupAdmissionModel::Num:{
        QSpinBox* spb = qobject_cast<QSpinBox*>(editor);
        model->setData(index, QString::number(spb->value()), Qt::DisplayRole);
        break;
    }
    case AGroupAdmissionModel::Rank:
    case AGroupAdmissionModel::Post:
    case AGroupAdmissionModel::Fio:
    case AGroupAdmissionModel::Date:
        break;
    case AGroupAdmissionModel::Date2:
    {
        QDateEdit* de = qobject_cast<QDateEdit*>(editor);
        model->setData(index, de->date().toString("dd.MM.yyyy"), Qt::DisplayRole);
        break;
    }
    case AGroupAdmissionModel::Result:
    {
        QComboBox* cb = qobject_cast<QComboBox*>(editor);
        model->setData(index, cb->currentText(), Qt::DisplayRole);
        model->setData(index, QSize(fm.width(cb->currentText())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
        model->setData(index, cb->itemData(cb->currentIndex()).toInt(), AGroupAdmissionModel::AttestResultRole);
        break;
    }
    case AGroupAdmissionModel::DateEnd:{
            QDateEdit* de = qobject_cast<QDateEdit*>(editor);
            model->setData(index, de->date().toString("dd.MM.yyyy"), Qt::DisplayRole);
            break;
    }
//    case AGroupAdmissionModel::NumAndDate:
    default:
    QLineEdit* le = qobject_cast<QLineEdit*>(editor);
    model->setData(index, le->text(), Qt::DisplayRole);
    model->setData(index, QSize(fm.width( le->text())+10, qMax(27, fm.height()+2+2)), Qt::SizeHintRole);
    break;
    }

    model->setData(index.sibling(index.row(), 0), true, AGroupAdmissionModel::ChangedRole);
}

