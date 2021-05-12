#ifndef ADMISSONMAINFORM_H
#define ADMISSONMAINFORM_H

#include <QWidget>
#include "StaffStructureManager"
#include "ui_admissonmainform.h"

namespace ODS{
class OdsInterface;
}

class QStandardItemModel;
class AGroupAdmissionModel;
class AkAdmissionModel;
class DamagedSiAdmissionModel;
class GuardAdmissionModel;
class KranAdmissionModel;
class MedAdmissionModel;
class PersonAdmissionModel;
class SiAdmissionModel;
class SoprAdmissionModel;
class TechLpaAdmissionModel;
class ZdeAdmissionModel;
class ZnkAdmissionModel;
class FirstZdeModel;
class SecondZdeModel;
class MechModel;
class PznkModel;
class RadioModel;
class RazvodModel;

class AdmissonMainForm : public QWidget, public Ui::AdmissonMainForm
{
    Q_OBJECT
    
public:
    enum Form{
        Personal = 1,
        Si,
        Med,
        AGroup,
        Zde,
        Ak,
        Sopr,
        Znk,
        Guard,
        Kran,
        DamagedSi,
        TechLpa,
        firstZde,
        secondZde,
        Mech,
        Pznk,
        Radio,
        Razvod
    };
    explicit AdmissonMainForm(Form formType, int vc = 0, QWidget *parent = 0);
    ~AdmissonMainForm();
    
private:
    Ui::AdmissonMainForm *ui;
    Form formType;
    AGroupAdmissionModel* agModel;
    AkAdmissionModel* akModel;
    DamagedSiAdmissionModel* damageModel;
    GuardAdmissionModel* guardModel;
    KranAdmissionModel* kranModel;
    MedAdmissionModel* medModel;
    PersonAdmissionModel* personModel;
    SiAdmissionModel* siModel;
    SoprAdmissionModel* soprModel;
    TechLpaAdmissionModel* techLpaModel;
    ZdeAdmissionModel* zdeModel;
    ZnkAdmissionModel* znkModel;
    FirstZdeModel* fzdeModel;
    SecondZdeModel* szdeModel;
    MechModel* mechModel;
    PznkModel* pznkModel;
    RadioModel* radioModel;
    RazvodModel* razvodModel;
    int currentVcId;
    void init();
    bool getVcs();
    bool createVC(STAFF::Division::ptr div);
    void setModel();
    void setDelegate();

private slots:
    void add();
    void remove();
    void cancel();
    void save();
    void close();
    void vcChanged(int index);
};

#endif // ADMISSONMAINFORM_H
