#ifndef CARGOFORM_H
#define CARGOFORM_H

#include <QWidget>
#include "ui_CargoForm.h"

class CargoModel;
class ProxyModelWithHeaderModels;
class QStandardItemModel;

namespace ODS{
class OdsInterface;
}

class CargoForm : public QWidget, public Ui::CargoForm
{
    Q_OBJECT
public:
    CargoForm(int order_id, ODS::OdsInterface* iface, QWidget *parent = 0);
    ~CargoForm();
private:
    CargoModel* m_model;
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
    void addCargo();
    void removeCargo();
    void cancelCargo();
    void save();
    void closeForm();
//    void changeOrder(int index);    
    void orderChanged(int order);
};

#endif // CARGOFORM_H
