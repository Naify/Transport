#ifndef AGROUPITEMDELEGATE_H
#define AGROUPITEMDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class AGroupItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    AGroupItemDelegate(QObject *parent = 0);
    ~AGroupItemDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // AGROUPITEMDELEGATE_H
