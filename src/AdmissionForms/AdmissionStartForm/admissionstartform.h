#ifndef ADMISSIONSTARTFORM_H
#define ADMISSIONSTARTFORM_H

#include <QWidget>
#include "StaffStructureManager"
#include "QStandardItemModel"

namespace Ui {
class AdmissionStartForm;
}

class AdmissionStartForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit AdmissionStartForm(QWidget *parent = 0);
    ~AdmissionStartForm();
    static int getCurrentVcId(){ return currentVC;}
    
private:
    Ui::AdmissionStartForm *ui;
    static int currentVC;
    QStandardItemModel* model;
    void init();
    bool getVCs();
    bool createVC(STAFF::Division::ptr);
    void checkAdmissionWarnings(QString table);

private slots:
    void changeVc(int);
    void createAdmisWarning(QString table, ODS::IObject iobj, Qt::GlobalColor color);
    void approveWarning(QModelIndex);
    void openPersonalForm();
    void openSIForm();
    void openMedForm();
    void openAGroupForm();
    void openZDEForm();
    void openAKForm();
    void openSoprForm();
    void openZNKForm();
    void openGuardForm();
    void openKranForm();
    void openDamagedSIForm();
    void openTechLPAForm();
    void open1ZDEForm();
    void open2ZDEForm();
    void openMechForm();
    void openPZNKForm();
    void openRadioForm();
    void openRazvodForm();

signals:
    void openPersonal();
    void openSI();
    void openMed();
    void openAGroup();
    void openZDE();
    void openAK();
    void openSopr();
    void openZNK();
    void openGuard();
    void openKran();
    void openDamagedSI();
    void openTechLPA();
    void open1ZDE();
    void open2ZDE();
    void openMech();
    void openPZNK();
    void openRadio();
    void openRazvod();
};

#endif // ADMISSIONSTARTFORM_H
