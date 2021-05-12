#ifndef KRANITEMDELEGATE_H
#define KRANITEMDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class KranItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    KranItemDelegate(QObject *parent = 0);
    ~KranItemDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // KRANITEMDELEGATE_H
