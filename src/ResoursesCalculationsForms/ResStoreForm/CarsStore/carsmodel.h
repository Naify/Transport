#ifndef CARSMODEL_H
#define CARSMODEL_H

#include <QStandardItemModel>

namespace ODS{
class OdsInterface;
class IObject;
}

class CarsModel: public QStandardItemModel
{
    Q_OBJECT
public:
    enum FieldType{
        Num = 1,
        Designation,
        FactoryNumb,
        MilitaryNumb,
        DateRelease,
        DatePere,
        Ready,
        DateO,
        Note,
        Away
    };

    enum Role{
        TypeRole = Qt::UserRole+1,
        IdRole,
        ChangedRole,
        DesignatRole
    };

    CarsModel(QObject *parent = 0);
    ~CarsModel();

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

#endif // CARSMODEL_H
