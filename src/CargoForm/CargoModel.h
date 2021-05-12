#ifndef CARGOMODEL_H
#define CARGOMODEL_H

#include <QStandardItemModel>

namespace ODS{
class OdsInterface;
class IObject;
}


class CargoModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum FieldType{
        Name = 1,
        NomNum,
        Num,
        Container,
        SG,
        Block,
        ContainerSeal,
        TzuType,
        TzuNum,
        TzuSeal,
        CarType,
        CarNum,
        CarSeal
    };

    enum Role{
        TypeRole = Qt::UserRole+1,
        IdRole,
        ChangedRole
    };

    CargoModel(int order_id, ODS::OdsInterface* iface, QObject *parent = 0);
    ~CargoModel();
    //
    void addEmptyRow();
    void removeCargo(const QModelIndex&);
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

#endif // CARGOMODEL_H
