#ifndef ESCORTCONTROLTMODEL_H
#define ESCORTCONTROLTMODEL_H

#include <QStandardItemModel>

namespace ODS{
class OdsInterface;
class IORecord;
class IObject;
}


class EscortControlModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum FieldType{
        In= 1,
        Out,
        Comment
    };

    enum Role{
        TypeRole = Qt::UserRole+1,
        IdRole,
        ChangedRole,
        ConvoyRole
    };

    EscortControlModel(ODS::OdsInterface* iface, QObject *parent = 0);
    ~EscortControlModel();
    //
    void load();
    bool save(const QModelIndex&);
private:
    ODS::OdsInterface* m_iface;
    void init();
    void makeRow(const ODS::IORecord& obj);
};

#endif // ESCORTCONTROLTMODEL_H
