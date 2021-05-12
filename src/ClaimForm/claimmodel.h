#ifndef CLAIMMODEL_H
#define CLAIMMODEL_H

#include <QStandardItemModel>

namespace ODS{
class OdsInterface;
class IObject;
}

class ClaimModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum FieldType{
        Num = 1,
        Sender,
        SendStation,
        DateTimeCar,
        CarType,
        Count,
        Numbers,
        DateTimeSend,
        ArriveStation,
        Reciever,
        Oficers,
        Ensignes,
        Sergants,
        Soldier
    };

    enum Role{
        TypeRole = Qt::UserRole+1,
        IdRole,
        ChangedRole,
        IrsRole
    };

    ClaimModel(int proID, QObject *parent = 0);
    ~ClaimModel();
    void addEmptyRow();
    void remove(const QModelIndex&index);
    void load();
    bool save();
    void changeProj(int projectId);
private:
    ODS::OdsInterface* iface;
    int projId;
    QList<int> idToDelList;
    void makeRow(const ODS::IObject& obj);
    bool saveItem(QStandardItem*item, ODS::IObject&obj);
};

#endif // CLAIMMODEL_H
