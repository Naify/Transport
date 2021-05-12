#ifndef CARSIDELEGATE_H
#define CARSIDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class CarsIDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    CarsIDelegate(QObject *parent = 0);
    ~CarsIDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // CARSIDELEGATE_H
