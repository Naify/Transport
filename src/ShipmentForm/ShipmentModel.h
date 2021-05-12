#ifndef SHIPMENTMODEL_H
#define SHIPMENTMODEL_H

#include <QStandardItemModel>

namespace ODS{
class OdsInterface;
class IObject;
}

class ShipmentModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum FieldType{
        NomNum = 1,
        Num,
        SG,
        Block,
        ContainerSeal,
        TzuType,
        TzuNum,
        TzuSeal,
        Team,
        Building,
        Auto,
        Run
    };

    enum Role{
        TypeRole = Qt::UserRole+1,
        IdRole,
        ChangedRole
    };

    ShipmentModel(int order_id, ODS::OdsInterface* iface, QObject *parent = 0);
    ~ShipmentModel();
    //
    void addEmptyRow();
//    void removeShipment(const QModelIndex&);
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

#endif // SHIPMENTMODEL_H
