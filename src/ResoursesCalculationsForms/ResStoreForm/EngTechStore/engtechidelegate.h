#ifndef ENGTECHIDELEGATE_H
#define ENGTECHIDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class EngTechIDelegate: public QItemDelegate
{
    Q_OBJECT
public:
    EngTechIDelegate(QObject *parent = 0);
    ~EngTechIDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // ENGTECHIDELEGATE_H
