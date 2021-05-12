#ifndef PZNKTEMDELEGATE_H
#define PZNKTEMDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class PznktemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    PznktemDelegate(QObject *parent = 0);
    ~PznktemDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // PZNKTEMDELEGATE_H
