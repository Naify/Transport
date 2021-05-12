#ifndef RESCALCSTARTFORM_H
#define RESCALCSTARTFORM_H

#include <QWidget>
#include "ui_rescalcstartform.h"

//namespace Ui {
//class ResCalcStartForm;
//}

class ResCalcStartForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit ResCalcStartForm(QWidget *parent = 0);
    ~ResCalcStartForm();
//    static int getTransitId(){ return currentTransitId;}
    
private:
    Ui::ResCalcStartForm *ui;
    static int currentTransitId;
    void init();
    bool getOrders();
    int checkTransitExist(int);

private slots:
    void changeOrder(int index);
    void changeOrderCargo(int index);
    void saveTransit();
    void openOrderForm() { emit openOrder();}
    void openRailsForm() { emit openRails();}
    void openCarsForm() { emit openCars();}
    void openTzuSkForm() { emit openTzuSk();}
    void openEngTechForm() { emit openEngTech();}
    void openSupportMachinesForm() { emit openSupportMachines();}
    void openCalcRailsForm() { emit openCalcRails(currentTransitId);}
    void openCalcPeopleForm() { emit openCalcPeople(currentTransitId);}
    void openCalcWeaponsForm() { emit openCalcWeapons(currentTransitId);}

signals:
    void openOrder();
    void openRails();
    void openCars();
    void openTzuSk();
    void openEngTech();
    void openSupportMachines();
    void openCalcRails(int);
    void openCalcPeople(int);
    void openCalcWeapons(int);
    void msgToStatus(QString);
};

#endif // RESCALCSTARTFORM_H
