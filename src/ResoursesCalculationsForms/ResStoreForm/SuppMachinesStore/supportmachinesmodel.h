#ifndef SUPPORTMACHINESMODEL_H
#define SUPPORTMACHINESMODEL_H

#include <QStandardItemModel>

namespace ODS{
class OdsInterface;
class IObject;
}

class SupportMachinesModel: public QStandardItemModel
{
    Q_OBJECT
public:
    enum FieldType{
        Num = 1,
        Name,
        Designation,
        FactoryNumb,
        MilitaryNumb,
        DateRelease,
        Ready
    };

    enum Role{
        TypeRole = Qt::UserRole+1,
        IdRole,
        ChangedRole
    };

    SupportMachinesModel(QObject *parent = 0);
    ~SupportMachinesModel();

    void addEmptyRow();
    void remove(const QModelIndex& index);
    void load();
    bool save();

private:
    ODS::OdsInterface* iface;
    QList<int> idToDelList;
    void init();
    void makeRow(const ODS::IObject& obj);
    bool saveItem(QStandardItem* item, ODS::IObject&obj);
};

#endif // SUPPORTMACHINESMODEL_H
