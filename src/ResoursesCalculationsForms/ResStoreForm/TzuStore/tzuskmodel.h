#ifndef TZUSKMODEL_H
#define TZUSKMODEL_H

#include <QStandardItemModel>

namespace ODS{
class OdsInterface;
class IObject;
}

class TzuSkModel: public QStandardItemModel
{
    Q_OBJECT
public:
    enum FieldType{
        Num = 1,
        Name,
        Designation,
        FactoryNumb,
        DateRelease,
        Ready,
        Note,
        Away
    };

    enum Role{
        TypeRole = Qt::UserRole+1,
        IdRole,
        ChangedRole,
        DesignatRole
    };

    TzuSkModel(QObject *parent = 0);
    ~TzuSkModel();

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

#endif // TZUSKMODEL_H
