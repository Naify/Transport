#ifndef RESSTOREFORM_H
#define RESSTOREFORM_H

#include <QWidget>
#include"ui_resstoreform.h"

namespace ODS{
class OdsInterface;
}

class QStandardItemModel;
class RailsModel;
class CarsModel;
class TzuSkModel;
class EngTechModel;
class SupportMachinesModel;

class ResStoreForm : public QWidget
{
    Q_OBJECT
    
public:
    enum Form{
        Rails = 1,
        Cars,
        TzuSk,
        EngTech,
        SuppMach
    };
    explicit ResStoreForm(Form formType, QWidget *parent = 0);
    ~ResStoreForm();
    
private:
    Ui::ResStoreForm *ui;
    Form formType;
    RailsModel* railsM;
    CarsModel* carsM;
    TzuSkModel* tzuM;
    EngTechModel* engM;
    SupportMachinesModel* suppM;
    void init();
    void fillHeaderModel(QStandardItemModel*);
    void setModel();
    void setDelegate();

private slots:
    void add();
    void remove();
    void cancel();
    void save();
    void close();
};

#endif // RESSTOREFORM_H
