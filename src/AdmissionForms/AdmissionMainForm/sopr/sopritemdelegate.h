#ifndef SOPRITEMDELEGATE_H
#define SOPRITEMDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class SoprItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    SoprItemDelegate(QObject *parent = 0);
    ~SoprItemDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // SOPRITEMDELEGATE_H
