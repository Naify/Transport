#ifndef MEDITEMDELEGATE_H
#define MEDITEMDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class MedItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:    
    MedItemDelegate(QObject *parent = 0);
    ~MedItemDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // MEDITEMDELEGATE_H
