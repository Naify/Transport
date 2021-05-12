#ifndef RAILSIDELEGATE_H
#define RAILSIDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class RailsIDelegate: public QItemDelegate
{
    Q_OBJECT
public:
    RailsIDelegate(QObject *parent = 0);
    ~RailsIDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // RAILSIDELEGATE_H
