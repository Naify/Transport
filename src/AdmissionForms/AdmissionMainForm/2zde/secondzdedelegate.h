#ifndef SECONDZDEDELEGATE_H
#define SECONDZDEDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class SecondZdeDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    SecondZdeDelegate(QObject *parent = 0);
    ~SecondZdeDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
    
};

#endif // SECONDZDEDELEGATE_H
