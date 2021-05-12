#ifndef ESCORTCONTROLTFORM_H
#define ESCORTCONTROLTFORM_H

#include <QWidget>
#include "ui_EscortControlForm.h"

class EscortControlModel;
class ProxyModelWithHeaderModels;
class QStandardItemModel;

namespace ODS{
class OdsInterface;
}

class EscortControlForm : public QWidget, public Ui::EscortControlForm
{
    Q_OBJECT
public:
    EscortControlForm(int order_id, ODS::OdsInterface* iface, QWidget *parent = 0);
    ~EscortControlForm();
private:
    EscortControlModel* m_model;
    QStandardItemModel* m_headerModel;
    ProxyModelWithHeaderModels* m_proxy_header;
    ODS::OdsInterface* m_iface;
    int m_order_id;
    //
    void init();
    void fillHeaderModel(QStandardItemModel*);
};

#endif // ESCORTCONTROLTFORM_H
