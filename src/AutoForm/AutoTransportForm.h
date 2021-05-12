#ifndef AUTOTRANSPORTFORM_H
#define AUTOTRANSPORTFORM_H

#include <QWidget>
#include "ui_AutoTransportForm.h"

class AutoTransportModel;
class ProxyModelWithHeaderModels;
class QStandardItemModel;

namespace ODS{
class OdsInterface;
}

class AutoTransportForm : public QWidget, public Ui::AutoTransportForm
{
    Q_OBJECT
public:
    AutoTransportForm(int order_id, ODS::OdsInterface* iface, QWidget *parent = 0);
    ~AutoTransportForm();
private:
    AutoTransportModel* m_model;
    QStandardItemModel* m_headerModel;
    ProxyModelWithHeaderModels* m_proxy_header;
    ODS::OdsInterface* m_iface;
    int m_order_id;
    //
    void init();
    void fillHeaderModel(QStandardItemModel*);
    void initOrders();
private slots:
    void addAutoTransport();
    void removeAutoTransport();
    void cancelAutoTransport();
    void save();
    void columnCountChanged(int);
    void currentColumnChanged(int);
    void closeForm();
    void orderChanged(int order);
};

#endif // AUTOTRANSPORTFORM_H
