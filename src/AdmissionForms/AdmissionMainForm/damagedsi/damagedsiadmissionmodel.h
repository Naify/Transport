#ifndef DAMAGEDSIADMISSIONMODEL_H
#define DAMAGEDSIADMISSIONMODEL_H

#include <QStandardItemModel>

namespace ODS{
class OdsInterface;
class IObject;
}

class DamagedSiAdmissionModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum FieldType{
        Num = 1,
        Rank,
        Post,
        Fio,
        Date,
        Si,
        NumAndDate,
        Place
    };

    enum Role{
        TypeRole = Qt::UserRole+1,
        IdRole,
        ChangedRole,
        PersonIdRole,
        SiIdRole,
        PlaceRole
    };

    DamagedSiAdmissionModel(int vcId, QObject *parent = 0);
    ~DamagedSiAdmissionModel();

    void addEmptyRow();
    void remove(const QModelIndex&);
    void load();
    bool save();
    void changeVc(int vcId);

private:
    ODS::OdsInterface* iface;
    int currentVcid;
    QList<int> idToDelList;
    int personId;
    void init();
    void makeRow(const ODS::IObject& obj);
    bool saveItem(QStandardItem*item, ODS::IObject&obj);
    void choosePerson();

private slots:
    void setPersonId(int id){ personId = id;}
};

#endif // DAMAGEDSIADMISSIONMODEL_H
