#ifndef TRAVELAUTHTORIZATIONFORM_H
#define TRAVELAUTHTORIZATIONFORM_H

#include <QWidget>
#include "ui_TravelAuthtorizationForm.h"

class TravelAuthtorizationModel;

namespace ODS{
class OdsInterface;
}

class TravelAuthtorizationForm : public QWidget, public Ui::TravelAuthtorizationForm
{
    Q_OBJECT
public:
    TravelAuthtorizationForm(int order_id, ODS::OdsInterface* iface, QWidget *parent = 0);
    ~TravelAuthtorizationForm();
private:
    TravelAuthtorizationModel* m_model;
    ODS::OdsInterface* m_iface;
    int m_order_id;
    int m_id;
    //
    void init();
//    void initCompleter();
    void initOrders();
private slots:
    void save();
    void closeForm();
    void orderChanged(int order);
signals:
    void msgToStatus(QString);
};

#endif // TRAVELAUTHTORIZATIONFORM_H
