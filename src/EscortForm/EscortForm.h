#ifndef ESCORTFORM_H
#define ESCORTFORM_H

#include <QWidget>
#include "ui_EscortForm.h"

class EscortModel;
class ProxyModelWithHeaderModels;
class QStandardItemModel;

namespace ODS{
class OdsInterface;
}

class EscortForm : public QWidget, public Ui::EscortForm
{
    Q_OBJECT
public:
    EscortForm(int order_id, ODS::OdsInterface* iface, QWidget *parent = 0);
    ~EscortForm();
private:
    EscortModel* m_model;
    QStandardItemModel* m_headerModel;
    ProxyModelWithHeaderModels* m_proxy_header;
    ODS::OdsInterface* m_iface;
    int m_order_id;
    //
    void init();
    void fillHeaderModel(QStandardItemModel*);
    void initOrders();
private slots:
    void addEscort();
    void removeEscort();
    void cancelEscort();
    void save();
    void closeForm();    
    void orderChanged(int order);
};

#endif // ESCORTFORM_H
