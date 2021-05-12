#ifndef DAMAGEDSIITEMDELEGATE_H
#define DAMAGEDSIITEMDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class DamagedSiItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    DamagedSiItemDelegate(QObject *parent = 0);
    ~DamagedSiItemDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // DAMAGEDSIITEMDELEGATE_H
