#include "admissonmainform.h"
#include "ui_admissonmainform.h"

#include "AdmissionForms/AdmissionStartForm/admissionstartform.h"
#include "person/personadmissionmodel.h"
#include "si/siadmissionmodel.h"
#include "med/medadmissionmodel.h"
#include "agroup/agroupadmissionmodel.h"
#include "zde/zdeadmissionmodel.h"
#include "ak/akadmissionmodel.h"
#include "sopr/sopradmissionmodel.h"
#include "znk/znkadmissionmodel.h"
#include "guard/guardadmissionmodel.h"
#include "kran/kranadmissionmodel.h"
#include "damagedsi/damagedsiadmissionmodel.h"
#include "tech/techlpaadmissionmodel.h"
#include "person/personitemdelegate.h"
#include "si/siitemdelegate.h"
#include "med/meditemdelegate.h"
#include "agroup/agroupitemdelegate.h"
#include "zde/zdeitemdelegate.h"
#include "ak/akitemdelegate.h"
#include "sopr/sopritemdelegate.h"
#include "znk/znkitemdelegate.h"
#include "guard/guarditemdelegate.h"
#include "kran/kranitemdelegate.h"
#include "damagedsi/damagedsiitemdelegate.h"
#include "tech/techlpaitemdelegate.h"
#include "1zde/firstzdedelegate.h"
#include "1zde/firstzdemodel.h"
#include "2zde/secondzdemodel.h"
#include "2zde/secondzdedelegate.h"
#include "mech/mechitemdelegate.h"
#include "mech/mechmodel.h"
#include "pznk/pznkmodel.h"
#include "pznk/pznktemdelegate.h"
#include "radio/radioitemdelegate.h"
#include "radio/radiomodel.h"
#include "razvod/razvoditemdelegate.h"
#include "razvod/razvodmodel.h"

#include "header_view/HierarchicalHeaderView.h"
#include "OdsInterface"
#include "QStandardItem"
#include "StaffStructureManager"
#include "QMessageBox"
#include "global_defs.h"

AdmissonMainForm::AdmissonMainForm(Form formType, int vc, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdmissonMainForm),
    formType(formType),    
    currentVcId(vc)
{
    setupUi(this);
    init();
}

AdmissonMainForm::~AdmissonMainForm()
{
    switch (formType)
    {
    case Personal:
        delete personModel;
        break;
    case Si:
        delete siModel;
        break;
    case Med:
        delete medModel;
        break;
    case AGroup:
        delete agModel;
        break;
    case Zde:
        delete zdeModel;
        break;
    case Ak:
        delete akModel;
        break;
    case Sopr:
        delete soprModel;
        break;
    case Znk:
        delete znkModel;
        break;
    case Guard:
        delete guardModel;
        break;
    case Kran:
        delete kranModel;
        break;
    case DamagedSi:
        delete damageModel;
        break;
    case TechLpa:
        delete techLpaModel;
        break;
    case firstZde:
        delete fzdeModel;
        break;
    case secondZde:
        delete szdeModel;
        break;
    case Mech:
        delete mechModel;
        break;
    case Pznk:
        delete pznkModel;
        break;
    case Radio:
        delete radioModel;
        break;
    case Razvod:
        delete razvodModel;
        break;
    }
    delete ui;
}

void AdmissonMainForm::init()
{
    getVcs();

    if (currentVcId != 0){
        cbVc->setCurrentIndex(cbVc->findData(AdmissionStartForm::getCurrentVcId()));
    }

    setModel();
    setDelegate();

    tv->resizeColumnsToContents();
    tv->verticalHeader()->setVisible(false);
#if QT_VERSION >= 0x050000
    ui->tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tv->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
#else
    ui->tv->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tv->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
#endif

    connect(pbAdd, SIGNAL(released()), this, SLOT(add()));
    connect(pbDel, SIGNAL(released()), this, SLOT(remove()));
    connect(pbCancel, SIGNAL(released()), this, SLOT(cancel()));
    connect(pbSave, SIGNAL(released()), this, SLOT(save()));
    connect(pbExit, SIGNAL(released()), this, SLOT(close()));
    connect(cbVc, SIGNAL(currentIndexChanged(int)), this, SLOT(vcChanged(int)));
}

void AdmissonMainForm::setModel()
{
    switch (formType)
    {
    case Personal:
        personModel = new PersonAdmissionModel(currentVcId, tv);
        personModel->setColumnCount(5);
        personModel->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
        personModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Воинское\nзвание")));
        personModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Должность")));
        personModel->setHorizontalHeaderItem(3, new QStandardItem(QString("ФИО")));
        personModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Дата\nназначения в\nподразделение")));
        tv->setModel(personModel);
        break;
    case Si:
        siModel = new SiAdmissionModel(currentVcId, tv);
        siModel->setColumnCount(9);
        siModel->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
        siModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Воинское\nзвание")));
        siModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Должность")));
        siModel->setHorizontalHeaderItem(3, new QStandardItem(QString("ФИО")));
        siModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Дата\nназначения в\nподразделение")));
        siModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Номенклатура СИ")));
        siModel->setHorizontalHeaderItem(6, new QStandardItem(QString("Номер приказа\nи дата")));
        siModel->setHorizontalHeaderItem(7, new QStandardItem(QString("Срок\nдействия\nдопуска")));
        siModel->setHorizontalHeaderItem(8, new QStandardItem(QString("Дата окончания\nсрока действия\nдопуска")));
        tv->setModel(siModel);
        break;
    case Med:
        medModel = new MedAdmissionModel(currentVcId, tv);
        medModel->setColumnCount(9);
        medModel->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
        medModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Воинское\nзвание")));
        medModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Должность")));
        medModel->setHorizontalHeaderItem(3, new QStandardItem(QString("ФИО")));
        medModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Дата\nназначения в\nподразделение")));
        medModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Дата\nпрохождения\nВВК")));
        medModel->setHorizontalHeaderItem(6, new QStandardItem(QString("Срок\nдействия\nзаключения\nВВК")));
        medModel->setHorizontalHeaderItem(7, new QStandardItem(QString("Заключение\nВВК")));
        medModel->setHorizontalHeaderItem(8, new QStandardItem(QString("Дата\nокончания\nсрока\nдействия\nзаключения\nВВК")));
        tv->setModel(medModel);
        break;
    case AGroup:
        agModel = new AGroupAdmissionModel(currentVcId, tv);
        agModel->setColumnCount(9);
        agModel->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
        agModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Воинское\nзвание")));
        agModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Должность")));
        agModel->setHorizontalHeaderItem(3, new QStandardItem(QString("ФИО")));
        agModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Дата\nназначения в\nподразделение")));
        agModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Дата\nпрохождения\nаттестации")));
        agModel->setHorizontalHeaderItem(6, new QStandardItem(QString("Результаты\nаттестации")));
        agModel->setHorizontalHeaderItem(7, new QStandardItem(QString("Номер\nприказа о\nдопуске")));
        agModel->setHorizontalHeaderItem(8, new QStandardItem(QString("Дата\nокончания\nсрока\nдействия\nзаключения\nВВК")));
        tv->setModel(agModel);
        break;
    case Zde:
        zdeModel = new ZdeAdmissionModel(currentVcId, tv);
        zdeModel->setColumnCount(8);
        zdeModel->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
        zdeModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Воинское\nзвание")));
        zdeModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Должность")));
        zdeModel->setHorizontalHeaderItem(3, new QStandardItem(QString("ФИО")));
        zdeModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Дата\nназначения в\nподразделение")));
        zdeModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Номер приказа\nи дата")));
        zdeModel->setHorizontalHeaderItem(6, new QStandardItem(QString("Срок\nдействия\nдопуска")));
        zdeModel->setHorizontalHeaderItem(7, new QStandardItem(QString("Дата окончания\nсрока действия\nдопуска")));
        tv->setModel(zdeModel);
        break;
    case Ak:
        akModel = new AkAdmissionModel(currentVcId, tv);
        akModel->setColumnCount(8);
        akModel->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
        akModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Воинское\nзвание")));
        akModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Должность")));
        akModel->setHorizontalHeaderItem(3, new QStandardItem(QString("ФИО")));
        akModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Дата\nназначения в\nподразделение")));
        akModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Номер приказа\nи дата")));
        akModel->setHorizontalHeaderItem(6, new QStandardItem(QString("Срок\nдействия\nдопуска")));
        akModel->setHorizontalHeaderItem(7, new QStandardItem(QString("Дата окончания\nсрока действия\nдопуска")));
        tv->setModel(akModel);
        break;
    case Sopr:
        soprModel = new SoprAdmissionModel(currentVcId, tv);
        soprModel->setColumnCount(8);
        soprModel->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
        soprModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Воинское\nзвание")));
        soprModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Должность")));
        soprModel->setHorizontalHeaderItem(3, new QStandardItem(QString("ФИО")));
        soprModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Дата\nназначения в\nподразделение")));
        soprModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Номер приказа\nи дата")));
        soprModel->setHorizontalHeaderItem(6, new QStandardItem(QString("Срок\nдействия\nдопуска")));
        soprModel->setHorizontalHeaderItem(7, new QStandardItem(QString("Дата окончания\nсрока действия\nдопуска")));
        tv->setModel(soprModel);
        break;
    case Znk:
        znkModel = new ZnkAdmissionModel(currentVcId, tv);
        znkModel->setColumnCount(8);
        znkModel->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
        znkModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Воинское\nзвание")));
        znkModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Должность")));
        znkModel->setHorizontalHeaderItem(3, new QStandardItem(QString("ФИО")));
        znkModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Дата\nназначения в\nподразделение")));
        znkModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Номер приказа\nи дата")));
        znkModel->setHorizontalHeaderItem(6, new QStandardItem(QString("Срок\nдействия\nдопуска")));
        znkModel->setHorizontalHeaderItem(7, new QStandardItem(QString("Дата окончания\nсрока действия\nдопуска")));
        tv->setModel(znkModel);
        break;
    case Guard:
        guardModel = new GuardAdmissionModel(currentVcId, tv);
        guardModel->setColumnCount(8);
        guardModel->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
        guardModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Воинское\nзвание")));
        guardModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Должность")));
        guardModel->setHorizontalHeaderItem(3, new QStandardItem(QString("ФИО")));
        guardModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Дата\nназначения в\nподразделение")));
        guardModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Номер приказа\nи дата")));
        guardModel->setHorizontalHeaderItem(6, new QStandardItem(QString("Срок\nдействия\nдопуска")));
        guardModel->setHorizontalHeaderItem(7, new QStandardItem(QString("Дата окончания\nсрока действия\nдопуска")));
        tv->setModel(guardModel);
        break;
    case Kran:
        kranModel = new KranAdmissionModel(currentVcId, tv);
        kranModel->setColumnCount(8);
        kranModel->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
        kranModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Воинское\nзвание")));
        kranModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Должность")));
        kranModel->setHorizontalHeaderItem(3, new QStandardItem(QString("ФИО")));
        kranModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Дата\nназначения в\nподразделение")));
        kranModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Номер приказа\nи дата")));
        kranModel->setHorizontalHeaderItem(6, new QStandardItem(QString("Срок\nдействия\nдопуска")));
        kranModel->setHorizontalHeaderItem(7, new QStandardItem(QString("Дата окончания\nсрока действия\nдопуска")));
        tv->setModel(kranModel);
        break;
    case DamagedSi:
        damageModel = new DamagedSiAdmissionModel(currentVcId, tv);
        damageModel->setColumnCount(8);
        damageModel->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
        damageModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Воинское\nзвание")));
        damageModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Должность")));
        damageModel->setHorizontalHeaderItem(3, new QStandardItem(QString("ФИО")));
        damageModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Дата\nназначения в\nподразделение")));
        damageModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Номенклатура СИ")));
        damageModel->setHorizontalHeaderItem(6, new QStandardItem(QString("Номер приказа\nи дата")));
        damageModel->setHorizontalHeaderItem(7, new QStandardItem(QString("Место сдачи\nэкзамена")));
        tv->setModel(damageModel);
        break;
    case TechLpa:
        techLpaModel = new TechLpaAdmissionModel(currentVcId, tv);
        techLpaModel->setColumnCount(8);
        techLpaModel->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
        techLpaModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Воинское\nзвание")));
        techLpaModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Должность")));
        techLpaModel->setHorizontalHeaderItem(3, new QStandardItem(QString("ФИО")));
        techLpaModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Дата\nназначения в\nподразделение")));
        techLpaModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Номер приказаз о допуске\nк техническим средствам\nЛПА и дата")));
        techLpaModel->setHorizontalHeaderItem(6, new QStandardItem(QString("Срок действия\nдопуска")));
        techLpaModel->setHorizontalHeaderItem(7, new QStandardItem(QString("Дата окончания\nсрока действия\nдопуска")));
        tv->setModel(techLpaModel);
        break;
    case firstZde:
        fzdeModel = new FirstZdeModel(currentVcId, tv);
        fzdeModel->setColumnCount(8);
        fzdeModel->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
        fzdeModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Воинское\nзвание")));
        fzdeModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Должность")));
        fzdeModel->setHorizontalHeaderItem(3, new QStandardItem(QString("ФИО")));
        fzdeModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Дата\nназначения в\nподразделение")));
        fzdeModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Номер приказа\nи дата")));
        fzdeModel->setHorizontalHeaderItem(6, new QStandardItem(QString("Срок\nдействия\nдопуска")));
        fzdeModel->setHorizontalHeaderItem(7, new QStandardItem(QString("Дата окончания\nсрока действия\nдопуска")));
        tv->setModel(fzdeModel);
        break;
    case secondZde:
        szdeModel = new SecondZdeModel(currentVcId, tv);
        szdeModel->setColumnCount(8);
        szdeModel->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
        szdeModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Воинское\nзвание")));
        szdeModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Должность")));
        szdeModel->setHorizontalHeaderItem(3, new QStandardItem(QString("ФИО")));
        szdeModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Дата\nназначения в\nподразделение")));
        szdeModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Номер приказа\nи дата")));
        szdeModel->setHorizontalHeaderItem(6, new QStandardItem(QString("Срок\nдействия\nдопуска")));
        szdeModel->setHorizontalHeaderItem(7, new QStandardItem(QString("Дата окончания\nсрока действия\nдопуска")));
        tv->setModel(szdeModel);
        break;
    case Mech:
        mechModel = new MechModel(currentVcId, tv);
        mechModel->setColumnCount(8);
        mechModel->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
        mechModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Воинское\nзвание")));
        mechModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Должность")));
        mechModel->setHorizontalHeaderItem(3, new QStandardItem(QString("ФИО")));
        mechModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Дата\nназначения в\nподразделение")));
        mechModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Номер приказа\nи дата")));
        mechModel->setHorizontalHeaderItem(6, new QStandardItem(QString("Срок\nдействия\nдопуска")));
        mechModel->setHorizontalHeaderItem(7, new QStandardItem(QString("Дата окончания\nсрока действия\nдопуска")));
        tv->setModel(mechModel);
        break;
    case Pznk:
        pznkModel = new PznkModel(currentVcId, tv);
        pznkModel->setColumnCount(8);
        pznkModel->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
        pznkModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Воинское\nзвание")));
        pznkModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Должность")));
        pznkModel->setHorizontalHeaderItem(3, new QStandardItem(QString("ФИО")));
        pznkModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Дата\nназначения в\nподразделение")));
        pznkModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Номер приказа\nи дата")));
        pznkModel->setHorizontalHeaderItem(6, new QStandardItem(QString("Срок\nдействия\nдопуска")));
        pznkModel->setHorizontalHeaderItem(7, new QStandardItem(QString("Дата окончания\nсрока действия\nдопуска")));
        tv->setModel(pznkModel);
        break;
    case Radio:
        radioModel = new RadioModel(currentVcId, tv);
        radioModel->setColumnCount(8);
        radioModel->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
        radioModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Воинское\nзвание")));
        radioModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Должность")));
        radioModel->setHorizontalHeaderItem(3, new QStandardItem(QString("ФИО")));
        radioModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Дата\nназначения в\nподразделение")));
        radioModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Номер приказа\nи дата")));
        radioModel->setHorizontalHeaderItem(6, new QStandardItem(QString("Срок\nдействия\nдопуска")));
        radioModel->setHorizontalHeaderItem(7, new QStandardItem(QString("Дата окончания\nсрока действия\nдопуска")));
        tv->setModel(radioModel);
        break;
    case Razvod:
        razvodModel = new RazvodModel(currentVcId, tv);
        razvodModel->setColumnCount(8);
        razvodModel->setHorizontalHeaderItem(0, new QStandardItem(QString("№")));
        razvodModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Воинское\nзвание")));
        razvodModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Должность")));
        razvodModel->setHorizontalHeaderItem(3, new QStandardItem(QString("ФИО")));
        razvodModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Дата\nназначения в\nподразделение")));
        razvodModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Номер приказа\nи дата")));
        razvodModel->setHorizontalHeaderItem(6, new QStandardItem(QString("Срок\nдействия\nдопуска")));
        razvodModel->setHorizontalHeaderItem(7, new QStandardItem(QString("Дата окончания\nсрока действия\nдопуска")));
        tv->setModel(razvodModel);
        break;
    }
}

void AdmissonMainForm::setDelegate()
{
    switch (formType)
    {
    case Personal:
        tv->setItemDelegate(new PersonItemDelegate(tv));
        break;
    case Si:
        tv->setItemDelegate(new SiItemDelegate(tv));
        break;
    case Med:
        tv->setItemDelegate(new MedItemDelegate(tv));
        break;
    case AGroup:
        tv->setItemDelegate(new AGroupItemDelegate(tv));
        break;
    case Zde:
        tv->setItemDelegate(new ZdeItemDelegate(tv));
        break;
    case Ak:
        tv->setItemDelegate(new AkItemDelegate(tv));
        break;
    case Sopr:
        tv->setItemDelegate(new SoprItemDelegate(tv));
        break;
    case Znk:
        tv->setItemDelegate(new ZnkItemDelegate(tv));
        break;
    case Guard:
        tv->setItemDelegate(new GuardItemDelegate(tv));
        break;
    case Kran:
        tv->setItemDelegate(new KranItemDelegate(tv));
        break;
    case DamagedSi:
        tv->setItemDelegate(new DamagedSiItemDelegate(tv));
        break;
    case TechLpa:
        tv->setItemDelegate(new TechLpaItemDelegate(tv));
        break;
    case firstZde:
        tv->setItemDelegate(new FirstZdeDelegate(tv));
        break;
    case secondZde:
        tv->setItemDelegate(new SecondZdeDelegate(tv));
        break;
    case Mech:
        tv->setItemDelegate(new MechItemDelegate(tv));
        break;
    case Pznk:
        tv->setItemDelegate(new PznktemDelegate(tv));
        break;
    case Radio:
        tv->setItemDelegate(new RadioItemDelegate(tv));
        break;
    case Razvod:
        tv->setItemDelegate(new RazvodItemDelegate(tv));
        break;
    }
}

bool AdmissonMainForm::getVcs()
{
    //установить соответствия вч из ошс и кп. проверить все ли вч есть у кп. если нет -
    //создать и назначить время изменения текущим

    STAFF::StaffStructureManager staffMgr(ODS::OdsInterface::self());
    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();

    STAFF::Division::ptr div = staffMgr.getCurrentDivision();
    if (div.isNull()){
        QMessageBox::critical(this, "Ошибка", "В ОШС не задан текущий объект");
        return false;
    }

    ODS::IObjectCursor  cur =  ioMan.getIObjects("Допуска КП.Вч для допусков");
    for (ODS::IObjectCursor::iterator it = cur.begin(); it != cur.end(); ++it) {
        ODS::IObject iobj = *it;
        STAFF::Division::ptr div2 = staffMgr.getDivisionById((iobj.getIntAttr("ОШС Вч")));
        cbVc->addItem(div2->name(), iobj.id());
    }

    STAFF::DivisionType::ptr divType = div->type();
    if (divType->name() == "О"){

        ODS::IObjectQueryFilter filter;
        filter.whereCondition = "\"ОШС Вч\" = " + QString::number(div->id());

        cur =  ioMan.getIObjects("Допуска КП.Вч для допусков", filter);
        if (cur.size() == 0){
            if (createVC(div)){
                return true;
            } else {
                return false;
            }
        } else {
            int index = cbVc->findData(cur.toList().at(0).id());
            if (index != -1){
                cbVc->setCurrentIndex(index);
                currentVcId = cur.toList().at(0).id();
            }
        }
    }
    return true;
}

bool AdmissonMainForm::createVC(STAFF::Division::ptr div)
{
    ODS::IObjectManager ioMan = ODS::OdsInterface::self()->iobjectManager();
    ODS::OdsInterface::self()->dbManager().transaction();
    ODS::OdsInterface::self()->syncManager().syncTransaction();
    ODS::IObject iobj = ioMan.createIObject("Допуска КП.Вч для допусков");
    ODS::IObject linkObj = ioMan.getIObjectById(div->id());
    if (!iobj.setAttr("ОШС Вч", linkObj)){
        ODS::OdsInterface::self()->dbManager().rollback();
        QMessageBox::critical(this, "Ошибка", "Не удалось задать атрибут \"ОШС Вч\" для вч");
        return false;
    }
    CDate cDate;
    if (!cDate.setCurrentDateTime()){
        ODS::OdsInterface::self()->dbManager().rollback();
        QMessageBox::critical(this, "Ошибка", "Не получить текущую дату");
        qDebug()<<"CDate не выставил currentDateTime";
        return false;
    }
    if (!iobj.setDateTime("Дата изменения", cDate)){
        ODS::OdsInterface::self()->dbManager().rollback();
        QMessageBox::critical(this, "Ошибка", "Не удалось задать атрибут \"Дата изменения\" для вч");
        return false;
    }
     if (iobj.save(ODS::IObject::NotRecursiveSave)){
        if (setIObjRights(iobj)){
            ODS::OdsInterface::self()->dbManager().commit();
            cbVc->addItem(div->name(), QVariant(iobj.id()));
            currentVcId = iobj.id();
            return true;
        } else {
            ODS::OdsInterface::self()->dbManager().rollback();
            QMessageBox::critical(this, "Ошибка", "Не удалось задать права для вч");
            return false;
        }
     } else {
        ODS::OdsInterface::self()->dbManager().rollback();
        QMessageBox::critical(this, "Ошибка", "Не удалось сохранить запись вч");
        return false;
     }
}

void AdmissonMainForm::vcChanged(int index)
{
    int vcId = cbVc->itemData(index).toInt();
    switch (formType)
    {
    case Personal:
        personModel->changeVc(vcId);
        break;
    case Si:
        siModel->changeVc(vcId);
        break;
    case Med:
        medModel->changeVc(vcId);
        break;
    case AGroup:
        agModel->changeVc(vcId);
        break;
    case Zde:
        zdeModel->changeVc(vcId);
        break;
    case Ak:
        akModel->changeVc(vcId);
        break;
    case Sopr:
        soprModel->changeVc(vcId);
        break;
    case Znk:
        znkModel->changeVc(vcId);
        break;
    case Guard:
        guardModel->changeVc(vcId);
        break;
    case Kran:
        kranModel->changeVc(vcId);
        break;
    case DamagedSi:
        damageModel->changeVc(vcId);
        break;
    case TechLpa:
        techLpaModel->changeVc(vcId);
        break;
    case firstZde:
        fzdeModel->changeVc(vcId);
        break;
    case secondZde:
        szdeModel->changeVc(vcId);
        break;
    case Mech:
        mechModel->changeVc(vcId);
        break;
    case Pznk:
        pznkModel->changeVc(vcId);
        break;
    case Radio:
        radioModel->changeVc(vcId);
        break;
    case Razvod:
        razvodModel->changeVc(vcId);
        break;
    }
}

void AdmissonMainForm::add()
{
    //уже внутри модели создавать окно для выбора человека, где это нужно
    switch (formType)
    {
    case Personal:
        personModel->addEmptyRow();
        break;
    case Si:
        siModel->addEmptyRow();
        break;
    case Med:
        medModel->addEmptyRow();
        break;
    case AGroup:
        agModel->addEmptyRow();
        break;
    case Zde:
        zdeModel->addEmptyRow();
        break;
    case Ak:
        akModel->addEmptyRow();
        break;
    case Sopr:
        soprModel->addEmptyRow();
        break;
    case Znk:
        znkModel->addEmptyRow();
        break;
    case Guard:
        guardModel->addEmptyRow();
        break;
    case Kran:
        kranModel->addEmptyRow();
        break;
    case DamagedSi:
        damageModel->addEmptyRow();
        break;
    case TechLpa:
        techLpaModel->addEmptyRow();
        break;
    case firstZde:
        fzdeModel->addEmptyRow();
        break;
    case secondZde:
        szdeModel->addEmptyRow();
        break;
    case Mech:
        mechModel->addEmptyRow();
        break;
    case Pznk:
        pznkModel->addEmptyRow();
        break;
    case Radio:
        radioModel->addEmptyRow();
        break;
    case Razvod:
        razvodModel->addEmptyRow();
        break;
    }
}

void AdmissonMainForm::remove()
{
    QModelIndex index = tv->currentIndex();
    if (!index.isValid())
        return;
    switch (formType)
    {
    case Personal:
        personModel->remove(index);
        break;
    case Si:
        siModel->remove(index);
        break;
    case Med:
        medModel->remove(index);
        break;
    case AGroup:
        agModel->remove(index);
        break;
    case Zde:
        zdeModel->remove(index);
        break;
    case Ak:
        akModel->remove(index);
        break;
    case Sopr:
        soprModel->remove(index);
        break;
    case Znk:
        znkModel->remove(index);
        break;
    case Guard:
        guardModel->remove(index);
        break;
    case Kran:
        kranModel->remove(index);
        break;
    case DamagedSi:
        damageModel->remove(index);
        break;
    case TechLpa:
        techLpaModel->remove(index);
        break;
    case firstZde:
        fzdeModel->remove(index);
        break;
    case secondZde:
        szdeModel->remove(index);
        break;
    case Mech:
        mechModel->remove(index);
        break;
    case Pznk:
        pznkModel->remove(index);
        break;
    case Radio:
        radioModel->remove(index);
        break;
    case Razvod:
        razvodModel->remove(index);
        break;
    }
}

void AdmissonMainForm::cancel()
{
    switch (formType)
    {
    case Personal:
        personModel->load();
        break;
    case Si:
        siModel->load();
        break;
    case Med:
        medModel->load();
        break;
    case AGroup:
        agModel->load();
        break;
    case Zde:
        zdeModel->load();
        break;
    case Ak:
        akModel->load();
        break;
    case Sopr:
        soprModel->load();
        break;
    case Znk:
        znkModel->load();
        break;
    case Guard:
        guardModel->load();
        break;
    case Kran:
        kranModel->load();
        break;
    case DamagedSi:
        damageModel->load();
        break;
    case TechLpa:
        techLpaModel->load();
        break;
    case firstZde:
        fzdeModel->load();
        break;
    case secondZde:
        szdeModel->load();
        break;
    case Mech:
        mechModel->load();
        break;
    case Pznk:
        pznkModel->load();
        break;
    case Radio:
        radioModel->load();
        break;
    case Razvod:
        razvodModel->load();
        break;
    }
}

void AdmissonMainForm::save()
{
    switch (formType)
    {
    case Personal:
        if (!personModel->save()){
            qDebug()<<"Error saving"<<Q_FUNC_INFO;
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить записи");
        }
        break;
    case Si:
        if (!siModel->save()){
            qDebug()<<"Error saving"<<Q_FUNC_INFO;
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить записи");
        }
        break;
    case Med:
        if (!medModel->save()){
            qDebug()<<"Error saving"<<Q_FUNC_INFO;
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить записи");
        }
        break;
    case AGroup:
        if (!agModel->save()){
            qDebug()<<"Error saving"<<Q_FUNC_INFO;
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить записи");
        }
        break;
    case Zde:
        if (!zdeModel->save()){
            qDebug()<<"Error saving"<<Q_FUNC_INFO;
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить записи");
        }
        break;
    case Ak:
        if (!akModel->save()){
            qDebug()<<"Error saving"<<Q_FUNC_INFO;
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить записи");
        }
        break;
    case Sopr:
        if (!soprModel->save()){
            qDebug()<<"Error saving"<<Q_FUNC_INFO;
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить записи");
        }
        break;
    case Znk:
        if (!znkModel->save()){
            qDebug()<<"Error saving"<<Q_FUNC_INFO;
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить записи");
        }
        break;
    case Guard:
        if (!guardModel->save()){
            qDebug()<<"Error saving"<<Q_FUNC_INFO;
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить записи");
        }
        break;
    case Kran:
        if (!kranModel->save()){
            qDebug()<<"Error saving"<<Q_FUNC_INFO;
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить записи");
        }
        break;
    case DamagedSi:
        if (!damageModel->save()){
            qDebug()<<"Error saving"<<Q_FUNC_INFO;
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить записи");
        }
        break;
    case TechLpa:
        if (!techLpaModel->save()){
            qDebug()<<"Error saving"<<Q_FUNC_INFO;
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить записи");
        }
        break;
    case firstZde:
        if (!fzdeModel->save()){
            qDebug()<<"Error saving"<<Q_FUNC_INFO;
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить записи");
        }
        break;
    case secondZde:
        if (!szdeModel->save()){
            qDebug()<<"Error saving"<<Q_FUNC_INFO;
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить записи");
        }
        break;
    case Mech:
        if (!mechModel->save()){
            qDebug()<<"Error saving"<<Q_FUNC_INFO;
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить записи");
        }
        break;
    case Pznk:
        if (!pznkModel->save()){
            qDebug()<<"Error saving"<<Q_FUNC_INFO;
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить записи");
        }
        break;
    case Radio:
        if (!radioModel->save()){
            qDebug()<<"Error saving"<<Q_FUNC_INFO;
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить записи");
        }
        break;
    case Razvod:
        if (!razvodModel->save()){
            qDebug()<<"Error saving"<<Q_FUNC_INFO;
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить записи");
        }
        break;
    }
}

void AdmissonMainForm::close()
{
    this->parentWidget()->close();
}
