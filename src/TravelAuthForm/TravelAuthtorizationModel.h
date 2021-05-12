#ifndef TRAVELAUTHTORIZATIONMODEL_H
#define TRAVELAUTHTORIZATIONMODEL_H

#include <QStandardItemModel>

namespace ODS{
class OdsInterface;
class IObject;
}


class TravelAuthtorizationModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum FieldType{
        Post = 1,
        Rank,
        LastName,
        FirstName,
        MidName,
        With
    };

    enum Role{
        TypeRole = Qt::UserRole+1,
        IdRole,
        ChangedRole
    };

    TravelAuthtorizationModel(int order_id, ODS::OdsInterface* iface, QObject *parent = 0);
    ~TravelAuthtorizationModel();
    //
    void load();
    void changeOrder(int order);
private:
    ODS::OdsInterface* m_iface;
    int m_order_id;
    void init();
    void makeRow(const ODS::IObject& obj);
};

#endif // TRAVELAUTHTORIZATIONMODEL_H
