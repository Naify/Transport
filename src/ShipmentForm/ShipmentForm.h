#ifndef SHIPMENTFORM_H
#define SHIPMENTFORM_H

#include <QWidget>
#include "ui_ShipmentForm.h"

class ShipmentModel;
class ProxyModelWithHeaderModels;
class QStandardItemModel;

namespace ODS{
class OdsInterface;
}

class ShipmentForm : public QWidget, public Ui::ShipmentForm
{
    Q_OBJECT
public:
    ShipmentForm(int order_id, ODS::OdsInterface* iface, QWidget *parent = 0);
    ~ShipmentForm();
private:
    ShipmentModel* m_model;
    QStandardItemModel* m_headerModel;
    ProxyModelWithHeaderModels* m_proxy_header;
    ODS::OdsInterface* m_iface;
    int m_order_id;
    //
    void init();
    void fillHeaderModel(QStandardItemModel*);
    void initOrders();
    //    void fillComboOrders();
private slots:
    void addShipment();
//    void removeShipment();
    void cancelShipment();
    void save();
    void closeForm();
//    void changeOrder(int index);    
    void orderChanged(int order);
};

#endif // SHIPMENTFORM_H
