#ifndef CARGOITEMDELEGATE_H
#define CARGOITEMDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class CargoItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    CargoItemDelegate(QObject *parent = 0);
    ~CargoItemDelegate();
    //
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // CARGOITEMDELEGATE_H
