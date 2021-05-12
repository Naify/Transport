#ifndef PARTSFORM_H
#define PARTSFORM_H

#include <QWidget>
#include "ui_PartsForm.h"

class PartsModel;
class ProxyModelWithHeaderModels;
class QStandardItemModel;

namespace ODS{
class OdsInterface;
}

class PartsForm : public QWidget, public Ui::PartsForm
{
    Q_OBJECT
public:
    PartsForm(int order_id, ODS::OdsInterface* iface, QWidget *parent = 0);
    ~PartsForm();
private:
    PartsModel* m_model;
    QStandardItemModel* m_headerModel;
    ProxyModelWithHeaderModels* m_proxy_header;
    ODS::OdsInterface* m_iface;
    int m_order_id;
    //
    void init();
    void fillHeaderModel(QStandardItemModel*);
    void initOrders();
private slots:
    void addParts();
    void removeParts();
    void cancelParts();
    void save();
    void closeForm();    
    void orderChanged(int order);
};

#endif // PARTSFORM_H
