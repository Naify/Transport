#ifndef SHOWESCORTDIALOGMODEL_H
#define SHOWESCORTDIALOGMODEL_H

#include <QStandardItemModel>

namespace ODS{
class OdsInterface;
class IObject;
class IObjectCursor;
}

class ShowEscortDialogModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum FieldType{
        Post = 1,
        Rank,
        Fio,
        WeaponName,
        WeaponNumber,
        TransportName,
        TransportNumber
    };

    enum Role{
        TypeRole = Qt::UserRole+1,
        IdRole,
        ChangedRole,
        TrIdRole,
        PersonIdRole,
        PostIdRole
    };

    ShowEscortDialogModel(QObject *parent = 0);
    ~ShowEscortDialogModel();

    void remove(const QModelIndex& index);
    void load(QMap<int,int> &newpeopleList, int trId);
    void loadPos(const QString& posName, ODS::IObjectCursor &cur );
    bool save();

private:
    ODS::OdsInterface* iface;
    QList<int> idToDelList;
    void init();
    void makeRow(int personId, int posId, const QString& post, int trId, int io_id = 0);
    bool saveItem(QStandardItem* item, ODS::IObject&obj);
    QMap<int,int> newPeopleList;
    int curTrId;
};

#endif // SHOWESCORTDIALOGMODEL_H
