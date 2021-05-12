#ifndef SHIPMENTITEMDELEGATE_H
#define SHIPMENTITEMDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class ShipmentItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ShipmentItemDelegate(QObject *parent = 0);
    ~ShipmentItemDelegate();
    //
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // SHIPMENTITEMDELEGATE_H
