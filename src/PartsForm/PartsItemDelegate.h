#ifndef PARTSITEMDELEGATE_H
#define PARTSITEMDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class PartsItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    PartsItemDelegate(QObject *parent = 0);
    ~PartsItemDelegate();
    //
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // PARTSITEMDELEGATE_H
