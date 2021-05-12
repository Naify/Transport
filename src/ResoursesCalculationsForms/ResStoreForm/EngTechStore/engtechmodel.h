#ifndef ENGTECHMODEL_H
#define ENGTECHMODEL_H

#include <QStandardItemModel>

namespace ODS{
class OdsInterface;
class IObject;
}

class EngTechModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum FieldType{
        Num = 1,
        Name,
        Designation,
        FactoryNumb,
        RegNumb,
        MilitaryNumb,
        DateRelease,
        DatePere,
        Ready,
        DateO
    };

    enum Role{
        TypeRole = Qt::UserRole+1,
        IdRole,
        ChangedRole
    };

    EngTechModel(QObject *parent = 0);
    ~EngTechModel();

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

#endif // ENGTECHMODEL_H
