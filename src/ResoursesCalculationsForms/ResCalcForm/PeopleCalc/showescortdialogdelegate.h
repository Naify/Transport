#ifndef SHOWESCORTDIALOGDELEGATE_H
#define SHOWESCORTDIALOGDELEGATE_H

#include <QItemDelegate>

class QAbstractItemModel;

class ShowEscortDialogDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ShowEscortDialogDelegate(QObject *parent = 0);
    ~ShowEscortDialogDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,
                          const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const;
};

#endif // SHOWESCORTDIALOGDELEGATE_H
