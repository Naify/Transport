#ifndef MECHITEMDELEGATE_H
#define MECHITEMDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class MechItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    MechItemDelegate(QObject *parent = 0);
    ~MechItemDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // MECHITEMDELEGATE_H
