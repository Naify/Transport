#ifndef TECHLPAITEMDELEGATE_H
#define TECHLPAITEMDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class TechLpaItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    TechLpaItemDelegate(QObject *parent = 0);
    ~TechLpaItemDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // TECHLPAITEMDELEGATE_H
