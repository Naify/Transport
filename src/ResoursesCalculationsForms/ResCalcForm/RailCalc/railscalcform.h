#ifndef RAILSCALCFORM_H
#define RAILSCALCFORM_H

#include <QWidget>
#include "QStandardItemModel"
#include "CatalogManager.h"

namespace Ui {
class RailsCalcForm;
}

class RailsCalcForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit RailsCalcForm(int transitId, QWidget *parent = 0);
    ~RailsCalcForm();
    
private:
    Ui::RailsCalcForm *ui;
    int currentTrId;
    int siTypeId;
    int siCount;
    QString kp;
    QStandardItemModel* model;
    void init();
    void load();
    void initTv();
    void initOther();
    int calcWithoutContainer();
    int calcWithContainer();
    void makeRow(int siCount, QString tzuType, int tzuTypeId, int tzuCount, QString railType, int railTypeId, int railCount);
    int searchForValueIRS(IRS::CatalogEntry::ptrList list);

private slots:
    void calc();
    void save();
    void close();

signals:
    void msgToStatus(QString);
};

#endif // RAILSCALCFORM_H
