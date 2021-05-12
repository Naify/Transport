#ifndef RAZVODITEMDELEGATE_H
#define RAZVODITEMDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class RazvodItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    RazvodItemDelegate(QObject *parent = 0);
    ~RazvodItemDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // RAZVODITEMDELEGATE_H
