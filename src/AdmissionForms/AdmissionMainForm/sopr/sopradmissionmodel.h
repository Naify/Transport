#ifndef SOPRADMISSIONMODEL_H
#define SOPRADMISSIONMODEL_H

#include <QStandardItemModel>

namespace ODS{
class OdsInterface;
class IObject;
}

class SoprAdmissionModel : public QStandardItemModel
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

    SoprAdmissionModel(int vcId, QObject *parent = 0);
    ~SoprAdmissionModel();

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

#endif // SOPRADMISSIONMODEL_H
