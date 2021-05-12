#ifndef KRANADMISSIONMODEL_H
#define KRANADMISSIONMODEL_H

#include <QStandardItemModel>

namespace ODS{
class OdsInterface;
class IObject;
}

class KranAdmissionModel : public QStandardItemModel
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

    KranAdmissionModel(int vcId, QObject *parent = 0);
    ~KranAdmissionModel();

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

#endif // KRANADMISSIONMODEL_H
