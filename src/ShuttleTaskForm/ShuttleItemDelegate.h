#ifndef SHUTTLEITEMDELEGATE_H
#define SHUTTLEITEMDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class ShuttleItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ShuttleItemDelegate(QObject *parent = 0);
    ~ShuttleItemDelegate();
    //
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // SHUTTLEITEMDELEGATE_H
