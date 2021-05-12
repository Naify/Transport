#ifndef RADIOITEMDELEGATE_H
#define RADIOITEMDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class RadioItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    RadioItemDelegate(QObject *parent = 0);
    ~RadioItemDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // RADIOITEMDELEGATE_H
