#ifndef CLAIMFORM_H
#define CLAIMFORM_H

#include <QWidget>
#include "claimmodel.h"
#include "QListWidget"

namespace Ui {
class ClaimForm;
}

class QStandardItemModel;

class ClaimForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit ClaimForm(QWidget *parent = 0);
    static QString getTrType() { return trType; }
    ~ClaimForm();
private:
    Ui::ClaimForm *ui;
    ClaimModel* model;
    int projectId;
    void init();
    void initTv();
    void initOther();
    void clearForm();
    static QString trType;

private slots:
    void clearTe();
    void loadProj(QListWidgetItem *lwItem);
    void newProj();
    void trTypeChange(QString transpType) {trType = transpType;}
    void add();
    void del();
    void save();
    void print();
    void close();

signals:
    void msgToStatus(QString);
};

#endif // CLAIMFORM_H
