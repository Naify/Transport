#ifndef FIRSTZDEDELEGATE_H
#define FIRSTZDEDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class FirstZdeDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    FirstZdeDelegate(QObject *parent = 0);
    ~FirstZdeDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // FIRSTZDEDELEGATE_H
