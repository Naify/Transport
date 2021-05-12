#ifndef ESCORTCONTROLTITEMDELEGATE_H
#define ESCORTCONTROLTITEMDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class EscortControlItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    EscortControlItemDelegate(QObject *parent = 0);
    ~EscortControlItemDelegate();
    //
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // ESCORTCONTROLTITEMDELEGATE_H
