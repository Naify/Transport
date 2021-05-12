#ifndef TRANSPORTITEMDELEGATE_H
#define TRANSPORTITEMDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class TransportItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    TransportItemDelegate(QObject *parent = 0);
    ~TransportItemDelegate();
    //
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // TRANSPORTITEMDELEGATE_H
