#ifndef GUARDITEMDELEGATE_H
#define GUARDITEMDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class GuardItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    GuardItemDelegate(QObject *parent = 0);
    ~GuardItemDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // GUARDITEMDELEGATE_H
