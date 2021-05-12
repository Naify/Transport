#ifndef RAILSMODEL_H
#define RAILSMODEL_H

#include <QStandardItemModel>

namespace ODS{
class OdsInterface;
class IObject;
}

class RailsModel: public QStandardItemModel
{
    Q_OBJECT
public:
    enum FieldType{
        Num = 1,
        Designation,
        FactoryNumb,
        CommonNumb,
        DateRelease,
        DateLastEtr,
        DateLastDr,
        DateLastZr,
        Ready,
        DateNextEtr,
        DateNextDr,
        DateNextZr,
        SecurityType,
        Away
    };

    enum Role{
        TypeRole = Qt::UserRole+1,
        IdRole,
        ChangedRole,
        DesignatRole
    };

    RailsModel(QObject *parent = 0);
    ~RailsModel();

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

#endif // RAILSMODEL_H
