#ifndef PERSONADMISSIONMODEL_H
#define PERSONADMISSIONMODEL_H

#include <QStandardItemModel>

namespace ODS{
class OdsInterface;
class IObject;
}

class PersonAdmissionModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum FieldType{
        Num = 1,
        Rank,
        Post,
        Fio,
        Date
    };

    enum Role{
        TypeRole = Qt::UserRole+1,
        IdRole,
        ChangedRole,
        IrsIdRole
    };

    PersonAdmissionModel(int vcId, QObject *parent = 0);
    ~PersonAdmissionModel();
    //
    void addEmptyRow();
    void remove(const QModelIndex&);
    void load();
    bool save();
    void changeVc(int vcId);
private:
    ODS::OdsInterface* iface;
    int currentVcid;
    QList<int> idToDelList;
    void init();
    void makeRow(const ODS::IObject& obj);
    bool saveItem(QStandardItem*, ODS::IObject&);
};

#endif // PERSONADMISSIONMODEL_H
