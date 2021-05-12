#ifndef ZDEITEMDELEGATE_H
#define ZDEITEMDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class ZdeItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ZdeItemDelegate(QObject *parent = 0);
    ~ZdeItemDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // ZDEITEMDELEGATE_H
