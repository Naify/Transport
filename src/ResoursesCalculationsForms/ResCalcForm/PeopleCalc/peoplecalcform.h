#ifndef PEOPLECALCFORM_H
#define PEOPLECALCFORM_H

#include <QWidget>
#include "QStandardItemModel"

namespace Ui {
class PeopleCalcForm;
}

class PeopleCalcForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit PeopleCalcForm(int transitId, QWidget *parent = 0);
    ~PeopleCalcForm();
    
private:
    Ui::PeopleCalcForm *ui;
    int currentTrId;
    QString kp;
    QStandardItemModel* model;
    QMap<int,int> newPeople;
    QList<int> toDelPeopleIds;
    int peopleCount;
    int siId;
    void init();
    void load();
    void initTv();
    void initOther();
    void getKpVals();
    void getPosVals(const QString& pos);

private slots:
    void calc();
    void save();
    void close();
    void escort();
    void tvDblClick(QModelIndex);
    void savePersonIds(QMap<int, int> personPostIds);
    void delFromEscort(int personId);
    void printRailMater();
    void clearPeopleList();

signals:
    void msgToStatus(QString);
};

#endif // PEOPLECALCFORM_H
