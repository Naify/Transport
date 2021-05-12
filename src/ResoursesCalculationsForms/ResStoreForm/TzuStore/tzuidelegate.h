#ifndef TZUIDELEGATE_H
#define TZUIDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class TzuIDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    TzuIDelegate(QObject *parent = 0);
    ~TzuIDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // TZUIDELEGATE_H
