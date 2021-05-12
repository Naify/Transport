#ifndef AKITEMDELEGATE_H
#define AKITEMDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class AkItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    AkItemDelegate(QObject *parent = 0);
    ~AkItemDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // AKITEMDELEGATE_H
