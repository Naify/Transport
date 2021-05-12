#ifndef ESCORTITEMDELEGATE_H
#define ESCORTITEMDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class EscortItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    EscortItemDelegate(QObject *parent = 0);
    ~EscortItemDelegate();
    //
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // ESCORTITEMDELEGATE_H
