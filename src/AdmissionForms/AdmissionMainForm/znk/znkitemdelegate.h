#ifndef ZNKITEMDELEGATE_H
#define ZNKITEMDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class ZnkItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ZnkItemDelegate(QObject *parent = 0);
    ~ZnkItemDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // ZNKITEMDELEGATE_H
