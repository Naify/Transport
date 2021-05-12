#ifndef SIADMISSIONMODEL_H
#define SIADMISSIONMODEL_H

#include <QStandardItemModel>
#include <QDebug>

namespace ODS{
class OdsInterface;
class IObject;
}

class SiAdmissionModel : public QStandardItemModel
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
        Term,
        DateEnd
    };

    enum Role{
        TypeRole = Qt::UserRole+1,
        IdRole,
        ChangedRole,
        PersonIdRole,
        SiIdRole
    };

    SiAdmissionModel(int vcId, QObject *parent = 0);
    ~SiAdmissionModel();

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
    bool saveItem(QStandardItem*, ODS::IObject&);
    void choosePerson();

private slots:
    void setPersonId(int id){ personId = id;}
};

#endif // SIADMISSIONMODEL_H
