#ifndef WEAPMATCALCFORM_H
#define WEAPMATCALCFORM_H

#include <QWidget>
#include "QStandardItemModel"

namespace Ui {
class WeapMatCalcForm;
}

class WeapMatCalcForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit WeapMatCalcForm(int transitId, QWidget *parent = 0);
    ~WeapMatCalcForm();
    
private:
    enum FormType{
        Weapons,
        Reserve,
        Comm,
        Med,
        Firefighter,
        Radioactive
    };
    Ui::WeapMatCalcForm *ui;
    int currentTrId;
    QStandardItemModel* model;
    WeapMatCalcForm::FormType form;
    QString kp;
    int peopleCount;
    QMap<QString, int> postsMap;
    QList<int> toDelPeopleIds;
    void init();
    void load();
    void initTv();
    void initOther();
    void calcWeap();
    void calcReserve();
    void calcComm();
    void calcMed();
    void calcFire();
    void calcRadioact();

private slots:
    void calc();
    void save();
    void print();
    void allPrint();
    void close();
    void loadWeap(){ form = Weapons; load(); }
    void loadReserve(){ form = Reserve; load(); }
    void loadComm(){ form = Comm; load(); }
    void loadMed(){ form = Med; load(); }
    void loadFireproof(){ form = Firefighter; load(); }
    void loadRhb(){ form = Radioactive; load(); }

signals:
    void msgToStatus(QString);
};

#endif // WEAPMATCALCFORM_H
