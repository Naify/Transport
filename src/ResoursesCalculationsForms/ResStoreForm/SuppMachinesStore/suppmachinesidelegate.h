#ifndef SUPPMACHINESIDELEGATE_H
#define SUPPMACHINESIDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class SuppMachinesIDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    SuppMachinesIDelegate(QObject *parent = 0);
    ~SuppMachinesIDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // SUPPMACHINESIDELEGATE_H
