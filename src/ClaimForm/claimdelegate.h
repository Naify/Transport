#ifndef CLAIMDELEGATE_H
#define CLAIMDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class ClaimDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ClaimDelegate(QObject *parent = 0);
    ~ClaimDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // CLAIMDELEGATE_H
