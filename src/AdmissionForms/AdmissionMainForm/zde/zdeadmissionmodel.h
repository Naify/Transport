#ifndef ZDEADMISSIONMODEL_H
#define ZDEADMISSIONMODEL_H

#include <QStandardItemModel>

namespace ODS{
class OdsInterface;
class IObject;
}

class ZdeAdmissionModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum FieldType{
        Num = 1,
        Rank,
        Post,
        Fio,
        Date,
        NumAndDate,
        Term,
        DateEnd
    };

    enum Role{
        TypeRole = Qt::UserRole+1,
        IdRole,
        ChangedRole,
        PersonIdRole
    };

    ZdeAdmissionModel(int vcId, QObject *parent = 0);
    ~ZdeAdmissionModel();

    void addEmptyRow();
    void remove(const QModelIndex& index);
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
    bool saveItem(QStandardItem* item, ODS::IObject& obj);
    void choosePerson();

private slots:
    void setPersonId(int id){ personId = id;}
};

#endif // ZDEADMISSIONMODEL_H
