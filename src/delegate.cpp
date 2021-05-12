//#include <QtGui>
#include <QCalendarWidget>
#include "delegate.h"


// ***************************** Time Edit *************************************************

TimeEditDelegate::TimeEditDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *TimeEditDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &/* option */,
                                        const QModelIndex &/* index */) const
{
    QTimeEdit *editor = new QTimeEdit(parent);
    editor->setDisplayFormat(TIME_FORMAT);
    return editor;
}

void TimeEditDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();

    QTimeEdit *timeEdit = static_cast<QTimeEdit*>(editor);
    timeEdit->setTime(QTime::fromString(QString(value),TIME_FORMAT));
}

void TimeEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    QTimeEdit *timeEdit = static_cast<QTimeEdit*>(editor);
    QString value = timeEdit->time().toString(TIME_FORMAT);

    model->setData(index, value, Qt::EditRole);
}

void TimeEditDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

// ***************************************************************************************


// ***************************** Date Edit *************************************************

DateEditDelegate::DateEditDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *DateEditDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &/* option */,
                                        const QModelIndex &/* index */) const
{
    QDateEdit *editor = new QDateEdit(parent);
    editor->setDisplayFormat(DATE_FORMAT);
    editor->setDate( QDate::currentDate() );
    editor->setCalendarPopup( true );
    editor->calendarWidget()->setFirstDayOfWeek( Qt::Monday );
    return editor;
}

void DateEditDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();

    QDateEdit *dateEdit = static_cast<QDateEdit*>(editor);
    dateEdit->setDate(QDate::fromString(QString(value),DATE_FORMAT));
}

void DateEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    QDateEdit *dateEdit = static_cast<QDateEdit*>(editor);
    QString value = dateEdit->date().toString(DATE_FORMAT);
    model->setData(index, value, Qt::EditRole);
}

void DateEditDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

// ***************************************************************************************

