#include "engtechidelegate.h"
#include "engtechmodel.h"

#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <CatalogManager.h>
#include <QDateEdit>

EngTechIDelegate::EngTechIDelegate(QObject *parent):
    QItemDelegate(parent)
{
}

EngTechIDelegate::~EngTechIDelegate()
{
}

QWidget *EngTechIDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
    switch(index.data(EngTechModel::TypeRole).toInt())
    {
    case EngTechModel::Num:
    {
        QSpinBox* spb = new QSpinBox(parent);
        spb->setValue(index.data(Qt::DisplayRole).toInt());
        return spb;
    }
    case EngTechModel::DateRelease:
    case EngTechModel::DatePere:
    case EngTechModel::DateO:
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
    case EngTechModel::Ready:
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

void EngTechIDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QFontMetrics fm(editor->font());

    switch(index.data(EngTechModel::TypeRole).toInt())
    {
    case EngTechModel::Num:{
        QSpinBox* spb = qobject_cast<QSpinBox*>(editor);
        model->setData(index, QString::number(spb->value()), Qt::DisplayRole);
        break;
    }
    case EngTechModel::DateRelease:
    case EngTechModel::DatePere:
    case EngTechModel::DateO:{
        QDateEdit* de = qobject_cast<QDateEdit*>(editor);
        model->setData(index, de->date().toString("dd.MM.yyyy"), Qt::DisplayRole);
        break;
    }
    case EngTechModel::Ready:{
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

    model->setData(index.sibling(index.row(), 0), true, EngTechModel::ChangedRole);
}
