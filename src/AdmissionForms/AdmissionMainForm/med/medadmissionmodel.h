#ifndef MEDADMISSIONMODEL_H
#define MEDADMISSIONMODEL_H

#include "QStandardItemModel"

namespace ODS{
class OdsInterface;
class IObject;
}

class MedAdmissionModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum FieldType{
        Num = 1,
        Rank,
        Post,
        Fio,
        Date,
        Date2,
        Term,
        Result,
        DateEnd
    };

    enum Role{
        TypeRole = Qt::UserRole+1,
        IdRole,
        ChangedRole,
        PersonIdRole,
        AttestResultRole
    };

    MedAdmissionModel(int vcId, QObject *parent = 0);
    ~MedAdmissionModel();

    void addEmptyRow();
    void remove(const QModelIndex&index);
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

#endif // MEDADMISSIONMODEL_H
