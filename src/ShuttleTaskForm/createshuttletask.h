#ifndef CREATESHUTTLETASK_H
#define CREATESHUTTLETASK_H

#include <QWidget>
#include <QMap>

#include "ShuttleTaskForm/ShuttleModel.h"
class ProxyModelWithHeaderModels;
#include <QStandardItemModel>

namespace Ui {
class CreateShuttleTask;
}

class CreateShuttleTask : public QWidget
{
    Q_OBJECT
    
public:
    explicit CreateShuttleTask(QWidget *parent = 0);
    ~CreateShuttleTask();

private slots:
    void processSave();
    void processAddToCheckPoints_S();
    void processAddToCheckPoints_R();
    void processAddToPaPnvt_S();
    void processAddToPaPnvt_R();
    void processAddToRespZone_S();
    void processAddToRespZone_R();
    void displayTransportType(const QString &item);
    void fill_cb_receiver_s(const QString &item);
    void update_return_le(const QString &item);
    void closeForm();
    void clear_form();
    void changeMz(int mzNumb);
    
private:
    Ui::CreateShuttleTask *ui;

    QStandardItemModel* m_headerModel;
    QStandardItemModel* m_headerModel2;

    ProxyModelWithHeaderModels* m_proxy_header;
    ProxyModelWithHeaderModels* m_proxy_header2;

    ShuttleModel *model_pa_pnvt_s;
    QStandardItemModel *model_respzone_s;
    QStandardItemModel *model_checkpoints_s;

    ShuttleModel *model_pa_pnvt_r;
    QStandardItemModel *model_respzone_r;
    QStandardItemModel *model_checkpoints_r;

    QMap<QString, quint32> raspMap;
    QMap<QString, quint32> senderMap;
    QMap<QString, quint32> receiverMap;
    QMap<QString, quint32> objectNameMap;

    int load_count_send;
    int load_count_return;

    void db_error_add();
    void fields_not_filled();    

    void setEditors();
signals:
    void msgToStatus(const QString);
};

#endif // CREATESHUTTLETASK_H
