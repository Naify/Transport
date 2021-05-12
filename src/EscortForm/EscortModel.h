#ifndef ESCORTMODEL_H
#define ESCORTMODEL_H

#include <QStandardItemModel>

namespace ODS{
class OdsInterface;
class IObject;
}


class EscortModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum FieldType{
        Post = 1,
        Rank,
        LastName,
        FirstName,
        MidName,
        With,
        WeaponName,
        WeaponNum,
        CarType,
        CarNum,
        Comment
    };

    enum Role{
        TypeRole = Qt::UserRole+1,
        IdRole,
        ChangedRole,
        ConvoyRole
    };

    EscortModel(int order_id, ODS::OdsInterface* iface, QObject *parent = 0);
    ~EscortModel();
    //
    void addEmptyRow();
    void removeEscort(const QModelIndex&);
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

#endif // ESCORTMODEL_H
