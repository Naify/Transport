#ifndef SIITEMDELEGATE_H
#define SIITEMDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class SiItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    SiItemDelegate(QObject *parent = 0);
    ~SiItemDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // SIITEMDELEGATE_H
