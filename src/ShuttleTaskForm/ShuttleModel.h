#ifndef SHUTTLEMODEL_H
#define SHUTTLEMODEL_H

#include <QStandardItemModel>

namespace ODS{
class OdsInterface;
class IObject;
}


class ShuttleModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum FieldType{
        Num = 1,
        NumTrain,
        Pa,
        Pnvt
    };

    enum Role{
        TypeRole = Qt::UserRole+1,
        IdRole,
        ChangedRole
    };

    ShuttleModel(int order_id, ODS::OdsInterface* iface, QObject *parent = 0);
    ~ShuttleModel();
    //
    void addEmptyRow();
//    void remove(const QModelIndex&);
    void load();
//    bool save();
    void changeShuttle(int order);
    void setId(int id);
private:
    ODS::OdsInterface* m_iface;
    QList<int> m_idToDelList;
    int m_shuttle_id;
    void init();
    void makeRow(const ODS::IObject& obj);
//    bool saveItem(QStandardItem*, ODS::IObject&);
};

#endif // SHUTTLEMODEL_H
