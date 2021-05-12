#ifndef AUTOTRANSPORTITEMDELEGATE_H
#define AUTOTRANSPORTITEMDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class AutoTransportItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    AutoTransportItemDelegate(QObject *parent = 0);
    ~AutoTransportItemDelegate();
    //
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // AUTOTRANSPORTITEMDELEGATE_H
