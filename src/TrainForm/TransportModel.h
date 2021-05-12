#ifndef TRANSPORTMODEL_H
#define TRANSPORTMODEL_H

#include <QStandardItemModel>

namespace ODS{
class OdsInterface;
class IObject;
}


class TransportModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum FieldType{
        CarType = 1,
        Mission,
        Num,
        Flank,
        FlankNum
    };

    enum Role{
        TypeRole = Qt::UserRole+1,
        IdRole,
        ChangedRole
    };

    TransportModel(int order_id, ODS::OdsInterface* iface, QObject *parent = 0);
    ~TransportModel();
    //
    void addEmptyRow();
    void removeTransport(const QModelIndex&);
    void load();
    bool save();
    void changeOrder(int order);
private:
    ODS::OdsInterface* m_iface;
    QList<int> m_idToDelList;
    int m_order_id;
    void init();
    void makeRow(const ODS::IObject& obj);
    bool saveItem(QStandardItem*, ODS::IObject&);
};

#endif // TRANSPORTMODEL_H
