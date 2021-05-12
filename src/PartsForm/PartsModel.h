#ifndef PARTSMODEL_H
#define PARTSMODEL_H

#include <QStandardItemModel>

namespace ODS{
class OdsInterface;
class IObject;
}


class PartsModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum FieldType{
        Name = 1,
        NomNum,
        Index,
        Num,
        Grif,
        Code,
        PlaceCount,
        Seal,
        SealCount,
        CarNum,
        Comment
    };

    enum Role{
        TypeRole = Qt::UserRole+1,
        IdRole,
        ChangedRole,
        GrifRole
    };

    PartsModel(int order_id, ODS::OdsInterface* iface, QObject *parent = 0);
    ~PartsModel();
    //
    void addEmptyRow();
    void removeParts(const QModelIndex&);
    void load();
    bool save();
    void changeOrder(int order);
private:
    ODS::OdsInterface* m_iface;
    QList<int> m_idToDelList;
    int m_order_id;
    void init();
    void makeRow(const ODS::IObject& obj);
    bool saveItem(QStandardItem*, ODS::IObject&);
    QString textByGrif(int);
};

#endif // PARTSMODEL_H
