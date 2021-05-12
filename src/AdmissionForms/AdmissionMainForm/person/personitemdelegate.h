#ifndef PERSONITEMDELEGATE_H
#define PERSONITEMDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class PersonItemDelegate : public QItemDelegate

{
    Q_OBJECT
public:
    PersonItemDelegate(QObject *parent = 0);
    ~PersonItemDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // PERSONITEMDELEGATE_H
