#ifndef TRANSPORTFORM_H
#define TRANSPORTFORM_H

#include <QWidget>
#include "ui_TransportForm.h"

class TransportModel;
class ProxyModelWithHeaderModels;
class QStandardItemModel;

namespace ODS{
class OdsInterface;
}

class TransportForm : public QWidget, public Ui::TransportForm
{
    Q_OBJECT
public:
    TransportForm(int order_id, ODS::OdsInterface* iface, QWidget *parent = 0);
    ~TransportForm();
private:
    TransportModel* m_model;
    QStandardItemModel* m_headerModel;
    ProxyModelWithHeaderModels* m_proxy_header;
    ODS::OdsInterface* m_iface;
    int m_order_id;
    //
    void init();
    void fillHeaderModel(QStandardItemModel*);
    void initOrders();
private slots:
    void addTransport();
    void removeTransport();
    void cancelTransport();
    void save();
    void closeForm();
    void orderChanged(int order);
};

#endif // TRANSPORTFORM_H
