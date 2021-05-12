#ifndef SHOWESCORTDIALOG_H
#define SHOWESCORTDIALOG_H

#include <QDialog>
#include <QStandardItemModel>

class ShowEscortDialogModel;

namespace Ui {
class showEscortDialog;
}

class showEscortDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit showEscortDialog(QMap<int,int> people, int trId, QWidget *parent = 0);
    ~showEscortDialog();
    
private:
    Ui::showEscortDialog *ui;    
    QMap<int,int> peopleList;
    int trId;
    ShowEscortDialogModel* model;
    void init();

private slots:
    void del();
    void save();
    void close();

signals:
    void delId(int);
    void clearNewPeople();
};

#endif // SHOWESCORTDIALOG_H
