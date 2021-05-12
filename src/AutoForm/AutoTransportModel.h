#ifndef AUTOTRANSPORTMODEL_H
#define AUTOTRANSPORTMODEL_H

#include <QStandardItemModel>

namespace ODS{
class OdsInterface;
class IObject;
}

class AutoTransportModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum FieldType{
        Post = 1,
        Rank,
        FIO,
        Mission,
        CarType,
        CarNum
    };

    enum Role{
        TypeRole = Qt::UserRole+1,
        IdRole,
        ChangedRole
    };

    AutoTransportModel(int order_id, int column_num, ODS::OdsInterface* iface, QObject *parent = 0);
    ~AutoTransportModel();
    //
    void addEmptyRow();
    void removeAutoTransport(const QModelIndex&);
    void load();
    void load(int column_num);
    bool removeAutocolumnMoreThan(int column_count);
    bool save();
    void changeOrder(int order);
private:
    ODS::OdsInterface* m_iface;
    QList<int> m_idToDelList;
    int m_order_id;
    int m_column_num;
    void init();
    void makeRow(const ODS::IObject& obj);
    bool saveItem(QStandardItem*, ODS::IObject&);
};

#endif // AUTOTRANSPORTMODEL_H
