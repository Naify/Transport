// Версия программы
#define VERSION QString(" 2.3.1")

#include <QSettings>
#include <QtGui>

#if QT_VERSION >= 0x050000
    #include <QDesktopWidget>
    #include <QMdiArea>
    #include <QStatusBar>
    #include <QMenu>
    #include <QMenuBar>
    #include <QMessageBox>
#else
    #include <QPrinter>
    #include <QPrintDialog>
    #include <QPrintPreviewDialog>
#endif

#include <QTextDocument>

#include <OdsTemplateManager>
#include "wordmaker.h"
#include <StaffStructureManager>
#include <OdsInterface>

#include "mainwindow.h"
#include "mdichild.h"
#include "OrderForm/orderform.h"
#include "OrderForm/addcontentform.h"
#include "CargoForm/CargoForm.h"
#include "AutoForm/AutoTransportForm.h"
#include "EscortForm/EscortForm.h"
#include "EscortCntrlForm/EscortControlForm.h"
#include "PartsForm/PartsForm.h"
#include "TrainForm/TransportForm.h"
#include "TravelAuthForm/TravelAuthtorizationForm.h"
#include "ShuttleTaskForm/createshuttletask.h"
#include "ShipmentForm/ShipmentForm.h"
#include "AdmissionForms/AdmissionStartForm/admissionstartform.h"
#include "AdmissionForms/AdmissionMainForm/admissonmainform.h"
#include "ResoursesCalculationsForms/ResCalcStartForm/rescalcstartform.h"
#include "ResoursesCalculationsForms/ResStoreForm/resstoreform.h"
#include "ResoursesCalculationsForms/ResCalcForm/RailCalc/railscalcform.h"
#include "ResoursesCalculationsForms/ResCalcForm/PeopleCalc/peoplecalcform.h"
#include "ResoursesCalculationsForms/ResCalcForm/WeapMatCalc/weapmatcalcform.h"
#include "ClaimForm/claimform.h"

extern bool RESOLUTION_HIGH;

MainWindow::MainWindow()
{
    QWidget *scr = QApplication::desktop()->screen( 0 ); // 0 - это номер экрана (если он один то всегда 0)
    int scrWidth = scr->width(); // ширина экрана

    if ( scrWidth > 1280 ) RESOLUTION_HIGH = true;

    mdiArea = new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(updateMenus()));

    OrderForm_form = 0;

    createActions();
    createMenus();
    createStatusBar();
    updateMenus();

    readSettings();

    this->setWindowTitle( MY_APP_NAME + VERSION);
    setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    mdiArea->closeAllSubWindows();

    writeSettings();
    event->accept();

    return;

    if (mdiArea->currentSubWindow()) {
        event->ignore();
    } else {
        writeSettings();
        event->accept();
    }
}

void MainWindow::writeStatus(const QString &message)
{
    statusBar()->showMessage(message, 5000);
    qDebug()<<message;
}

void MainWindow::updateMenus()
{
    bool hasMdiChild = (activeMdiChild() != 0);
    closeAct->setEnabled(hasMdiChild);
    closeAllAct->setEnabled(hasMdiChild);
    tileAct->setEnabled(hasMdiChild);
    cascadeAct->setEnabled(hasMdiChild);
    nextAct->setEnabled(hasMdiChild);
    previousAct->setEnabled(hasMdiChild);
}

void MainWindow::updateWindowMenu()
{
    windowMenu->clear();
    windowMenu->addAction(closeAct);
    windowMenu->addAction(closeAllAct);
    windowMenu->addSeparator();
    windowMenu->addAction(tileAct);
    windowMenu->addAction(cascadeAct);
    windowMenu->addSeparator();
    windowMenu->addAction(nextAct);
    windowMenu->addAction(previousAct);
}

MdiChild *MainWindow::createMdiChild(const QString & formName, QWidget * windowWidget)
{
    if(windowWidget == 0)
        return 0;

    MdiChild *child = new MdiChild(formName);
    child->setWidget( windowWidget );
//    child->move(0,0);

    if (formName == ORDER_RECIEVE_SEND_TECH) {
        child->setGeometry(0,0, 900,730);
    } else if (formName == START_ASSIGN){
        child->setGeometry(0,0, 500, 800);
    } else if (formName == ADD_CONTENT){
        child->setGeometry(0,0,840,145);
    } else if (formName == AUTO_TR || formName == PERSONAL_ASSIGN){
        child->setGeometry(0,0,700,300);
    } else if (formName == CARGO || formName == ESCORT || formName == PARTS ||
                formName == SI_ASSIGN || formName == MED_ASSIGN || formName == AGROUP_ASSIGN ||
                formName == ZDE_ASSIGN || formName == AK_ASSIGN || formName == SOPR_ASSIGN ||
                formName == ZNK_ASSIGN || formName == GUARD_ASSIGN || formName == KRAN_ASSIGN ||
                formName == DAMAGEDSI_ASSIGN || formName == TECHLPA_ASSIGN ||
                formName == RAILS || formName == CARS || formName == TZUSK ||
                formName == ENGTECH || formName == SUPPMACHINES || formName == "1-й ЗНЭШ" ||
                formName == "2-ы ЗНЭШ" || formName == "Механик спец вагона" ||
                formName == "Помощник начальника караула" || formName == "Расчет радиостанции"
                || formName == "Разводящие" ){
        child->setGeometry(0,0,1060,300);
    } else if (formName == TRAIN_TR){
        child->setGeometry(0,0,600,300);
    } else if (formName == ESCORT_CTRL){
        child->setGeometry(0,0,1000,200);
    } else if(formName == TRAVEL_AUTH){
        child->setGeometry(0,0,850,500);
    } else if(formName == SHIPMENT){
        child->setGeometry(0,0,1300,300);
    } else if(formName == START_ASSIGN){
        child->setGeometry(0,0,655,540);
    } else if (formName == CLAIM){
        child->setGeometry(0,0,1300,400);
    }
    mdiArea->addSubWindow(child);

//    child->setWindowState( Qt::WindowMaximized );

    connect(child, SIGNAL(sigActivate(QMdiSubWindow*)), SLOT(setActiveSubWindow(QMdiSubWindow *)));
    child->show();
    return child;
}

void MainWindow::createActions()
{
    closeAct = new QAction(tr("Закрыть"), this);
    closeAct->setStatusTip(tr("Закрыть активное окно"));
    connect(closeAct, SIGNAL(triggered()),
            mdiArea, SLOT(closeActiveSubWindow()));

    closeAllAct = new QAction(tr("Закрыть все"), this);
    closeAllAct->setStatusTip(tr("Закрыть все окна"));
    connect(closeAllAct, SIGNAL(triggered()),
            mdiArea, SLOT(closeAllSubWindows()));

    tileAct = new QAction(tr("Мозайкой"), this);
    tileAct->setStatusTip(tr("Расположить мозайкой"));
    connect(tileAct, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));

    cascadeAct = new QAction(tr("Каскадом"), this);
    cascadeAct->setStatusTip(tr("Расположить каскадом"));
    connect(cascadeAct, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));

    nextAct = new QAction(tr("Следующее"), this);
    nextAct->setShortcuts(QKeySequence::NextChild);
    nextAct->setStatusTip(tr("Следующее окно"));
    connect(nextAct, SIGNAL(triggered()),
            mdiArea, SLOT(activateNextSubWindow()));

    previousAct = new QAction(tr("Предыдущее"), this);
    previousAct->setShortcuts(QKeySequence::PreviousChild);
    previousAct->setStatusTip(tr("Предыдущее окно"));
    connect(previousAct, SIGNAL(triggered()),
            mdiArea, SLOT(activatePreviousSubWindow()));

    // MY ACTIONS

    OrdersAction = new QAction(ORDER_RECIEVE_SEND_TECH, this);
    connect(OrdersAction, SIGNAL(triggered()), this, SLOT(openForm_orders()));

    ShuttleAction = new QAction(CREATE_SHUTTLE_TASK, this);
    connect(ShuttleAction, SIGNAL(triggered()), this, SLOT(openForm_create_shuttle_task()));

    CargoAction = new QAction(CARGO, this);
    connect(CargoAction, SIGNAL(triggered()), this, SLOT(openForm_cargo()));

    PartsAction = new QAction(PARTS, this);
    connect(PartsAction, SIGNAL(triggered()), this, SLOT(openForm_parts()));

    InOutDocAction = new QAction(IN_OUT_DOC, this);
    connect(InOutDocAction, SIGNAL(triggered()), this, SLOT(openDoc_inout()));

    TrainAction = new QAction(TRAIN_TR, this);
    connect(TrainAction, SIGNAL(triggered()), this, SLOT(openForm_train()));

    AutoAction = new QAction(AUTO_TR, this);
    connect(AutoAction, SIGNAL(triggered()), this, SLOT(openForm_auto()));

    EscortAction = new QAction(ESCORT, this);
    connect(EscortAction, SIGNAL(triggered()), this, SLOT(openForm_escort()));

    TravelAuthAction = new QAction(TRAVEL_AUTH, this);
    connect(TravelAuthAction, SIGNAL(triggered()), this, SLOT(openForm_travel_auth()));

    TravelAuthDocAction = new QAction(TRAVEL_AUTH_DOC, this);
    connect(TravelAuthDocAction, SIGNAL(triggered()), this, SLOT(openDoc_travel_auth()));

    TrainCommanderDocAction = new QAction(TRAIN_COMMANDER_DOC, this);
    connect(TrainCommanderDocAction, SIGNAL(triggered()), this, SLOT(openDoc_train()));

    PostsTableDocAction = new QAction(TABEL_DOC, this);
    connect(PostsTableDocAction, SIGNAL(triggered()), this, SLOT(openDoc_tabel()));

    PostsSheetDocActoin = new QAction(POSTS_DOC, this);
    connect(PostsSheetDocActoin, SIGNAL(triggered()), this, SLOT(openDoc_posts()));

    CommanderDocAction = new QAction(COMMANDER_DOC, this);
    connect(CommanderDocAction, SIGNAL(triggered()), this, SLOT(openDoc_commander()));

    ActsDocAction = new QAction(ACTS_DOC, this);
    connect(ActsDocAction, SIGNAL(triggered()), this, SLOT(openDoc_acts()));

    OrderTransportDocAction = new QAction(ORDER_TRANSP_DOC, this);
    connect(OrderTransportDocAction, SIGNAL(triggered()), this, SLOT(openDoc_order_transp()));

    TrainCommanderDocAction = new QAction(TRAIN_COMMANDER_DOC, this);
    connect(TrainCommanderDocAction, SIGNAL(triggered()), this, SLOT(openDoc_train()));

    CarsCommanderDocAction = new QAction(CARS_COMMANDER_DOC, this);
    connect(CarsCommanderDocAction, SIGNAL(triggered()), this, SLOT(openDoc_cars()));

    PlanDocAction = new QAction(PLAN_DOC, this);
    connect(PlanDocAction, SIGNAL(triggered()), this, SLOT(openDoc_plan()));

    ShipmentAction = new QAction(SHIPMENT, this);
    connect(ShipmentAction, SIGNAL(triggered()), this, SLOT(openForm_shipment()));

    StartAssignAction = new QAction(START_ASSIGN, this);
    connect(StartAssignAction, SIGNAL(triggered()), this, SLOT(openForm_startassignment()));

    PersonalAssignAction = new QAction(PERSONAL_ASSIGN, this);
    connect(PersonalAssignAction, SIGNAL(triggered()), this, SLOT(openForm_personal()));

    SIAssignAction = new QAction(SI_ASSIGN, this);
    connect(SIAssignAction, SIGNAL(triggered()), this, SLOT(openForm_si()));

    MedAssignAction = new QAction(MED_ASSIGN, this);
    connect(MedAssignAction, SIGNAL(triggered()), this, SLOT(openForm_med()));

    AGroupAssignAction = new QAction(AGROUP_ASSIGN, this);
    connect(AGroupAssignAction, SIGNAL(triggered()), this, SLOT(openForm_agroup()));

    ZDEAssignAction = new QAction(ZDE_ASSIGN, this);
    connect(ZDEAssignAction, SIGNAL(triggered()), this, SLOT(openForm_zde()));

    AKAssignAction = new QAction(AK_ASSIGN, this);
    connect(AKAssignAction, SIGNAL(triggered()), this, SLOT(openForm_ak()));

    SoprAssignAction = new QAction(SOPR_ASSIGN, this);
    connect(SoprAssignAction, SIGNAL(triggered()), this, SLOT(openForm_sopr()));

    ZNKAssignAction = new QAction(ZNK_ASSIGN, this);
    connect(ZNKAssignAction, SIGNAL(triggered()), this, SLOT(openForm_znk()));

    GuardAssignAction = new QAction(GUARD_ASSIGN, this);
    connect(GuardAssignAction, SIGNAL(triggered()), this, SLOT(openForm_guard()));

    KranAssignAction = new QAction(KRAN_ASSIGN, this);
    connect(KranAssignAction, SIGNAL(triggered()), this, SLOT(openForm_kran()));

    DamagedSIAssignAction = new QAction(DAMAGEDSI_ASSIGN, this);
    connect(DamagedSIAssignAction, SIGNAL(triggered()), this, SLOT(openForm_damagedsi()));

    TechLPAAssignAction = new QAction(TECHLPA_ASSIGN, this);
    connect(TechLPAAssignAction, SIGNAL(triggered()), this, SLOT(openForm_techlpa()));

    StartResCalcAction = new QAction(START_RESCALC, this);
    connect(StartResCalcAction, SIGNAL(triggered()), this, SLOT(openForm_startrescalc()));

    ClaimAction = new QAction(CLAIM, this);
    connect(ClaimAction, SIGNAL(triggered()), this, SLOT(openForm_claim()));
}

void MainWindow::createMenus()
{
    orderMenu = menuBar()->addMenu("Распоряжение/\"Дорожка\"/Заявка");
    docMenu = menuBar()->addMenu(DOCS_MENU);
    assignMenu = menuBar()->addMenu(ASSIGN_MENU);
    resCalcMenu = menuBar()->addMenu(RESCALC_MENU);

    orderMenu->addAction( OrdersAction );
    orderMenu->addAction( ShuttleAction );
    orderMenu->addAction( ClaimAction );

    docMenu->addAction( CargoAction );
    docMenu->addAction( PartsAction );
    docMenu->addSeparator();
    docMenu->addAction( InOutDocAction );
    docMenu->addSeparator();
    docMenu->addAction( AutoAction );
    docMenu->addAction( TrainAction );
    docMenu->addSeparator();
    docMenu->addAction( EscortAction );
    docMenu->addAction( TravelAuthAction );
    docMenu->addSeparator();
    docMenu->addAction( TravelAuthDocAction );
    docMenu->addAction( TrainCommanderDocAction );
    docMenu->addAction( CarsCommanderDocAction );
    docMenu->addAction( PostsTableDocAction );
    docMenu->addAction( PostsSheetDocActoin );
    docMenu->addAction( CommanderDocAction );
    docMenu->addAction( OrderTransportDocAction );
    docMenu->addAction( ActsDocAction );
    docMenu->addAction( PlanDocAction );
//    docMenu->addAction( TrainCommanderDocAction);
    docMenu->addSeparator();
    docMenu->addAction( ShipmentAction );

    assignMenu->addAction(StartAssignAction);
    assignMenu->addAction(PersonalAssignAction);
    assignMenu->addSeparator();
    assignMenu->addAction(SIAssignAction);
    assignMenu->addAction(MedAssignAction);
    assignMenu->addAction(AGroupAssignAction);
    assignMenu->addSeparator();
    assignMenu->addAction(ZDEAssignAction);
    assignMenu->addAction(AKAssignAction);
    assignMenu->addAction(SoprAssignAction);
    assignMenu->addAction(ZNKAssignAction);
    assignMenu->addAction(GuardAssignAction);
    assignMenu->addAction(KranAssignAction);
    assignMenu->addSeparator();
    assignMenu->addAction(DamagedSIAssignAction);
    assignMenu->addAction(TechLPAAssignAction);

    resCalcMenu->addAction(StartResCalcAction);

    windowMenu = menuBar()->addMenu(tr("Окно"));

    updateWindowMenu();
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

    // Форма для отображения на старте
    openForm(ORDER_RECIEVE_SEND_TECH);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Готов"));
}

void MainWindow::readSettings()
{
    QSettings settings( MY_APP_NAME );
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    move(pos);
    resize(size);
}

void MainWindow::writeSettings()
{
    QSettings settings( MY_APP_NAME );
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

bool MainWindow::checkCurrentStaffVc()
{
    using namespace STAFF;
    StaffStructureManager staffMgr(ODS::OdsInterface::self());

    Division::ptr div = staffMgr.getCurrentDivision();
    if (div.isNull()){
        QMessageBox::critical(this, "Ошибка", "В ОШС не задан текущий объект");
        return false;
    }

    DivisionType::ptr divType = div->type();
    if (divType->name() != "О"){
        return false;
    }
    return true;
}

QWidget *  MainWindow::openForm(const QString &formName, int Vc)
{
    QMdiSubWindow *window = findMdiChild( formName );

    if ( window ) {// Такое окно уже открыто
        setActiveSubWindow(window);
        return window->widget();
    }
    // окна нет, создаем новое
    QWidget * wWidget = 0;

    if ( formName == ORDER_RECIEVE_SEND_TECH )
    {
        wWidget = new OrderForm();
        OrderForm * orderForm = qobject_cast<OrderForm *>(wWidget);
        OrderForm_form = orderForm;
        connect(orderForm, SIGNAL(openAddForm()), SLOT(openForm_add_content()));
        connect(orderForm, SIGNAL(msgToStatus(QString)), SLOT( writeStatus(QString) ) );
    }
    else if ( formName == ADD_CONTENT )
    {
        wWidget = new AddContentForm();
        AddContentForm * addContentForm = qobject_cast<AddContentForm *>(wWidget);
        connect(addContentForm, SIGNAL(row_save()), OrderForm_form,  SLOT(renewTableOrders()));
        connect(addContentForm, SIGNAL(msgToStatus(QString)), SLOT( writeStatus(QString) ) );
    }
    else if ( formName == CARGO )
    {
        wWidget = new CargoForm(OrderForm::get_current_order_id(), ODS::OdsInterface::self());
    }
    else if ( formName == AUTO_TR)
    {
        wWidget = new AutoTransportForm(OrderForm::get_current_order_id(), ODS::OdsInterface::self());
    }
    else if ( formName == ESCORT_CTRL)
    {
//        wWidget = new EscortControlForm(OrderForm::get_current_order_id(), ODS::OdsInterface::self());
    }
    else if ( formName == ESCORT)
    {
        wWidget = new EscortForm(OrderForm::get_current_order_id(), ODS::OdsInterface::self());
    }
    else if ( formName == PARTS)
    {
        wWidget = new PartsForm(OrderForm::get_current_order_id(), ODS::OdsInterface::self());
    }
    else if ( formName == TRAIN_TR)
    {
        wWidget = new TransportForm(OrderForm::get_current_order_id(), ODS::OdsInterface::self());
    }
    else if ( formName == TRAVEL_AUTH)
    {
        wWidget = new TravelAuthtorizationForm(OrderForm::get_current_order_id(), ODS::OdsInterface::self());
        TravelAuthtorizationForm * travelAuthForm = qobject_cast<TravelAuthtorizationForm *>(wWidget);
        connect(travelAuthForm, SIGNAL(msgToStatus(QString)), SLOT( writeStatus(QString) ) );
    }
    else if ( formName == CREATE_SHUTTLE_TASK )
    {
        wWidget = new CreateShuttleTask();
        CreateShuttleTask * shuttleForm = qobject_cast<CreateShuttleTask *>(wWidget);
        connect(shuttleForm, SIGNAL(msgToStatus(QString)), SLOT( writeStatus(QString) ) );
    }
    else if ( formName == SHIPMENT )
    {
        wWidget = new ShipmentForm(OrderForm::get_current_order_id(), ODS::OdsInterface::self());
    }
    else if ( formName == START_ASSIGN )
    {
        if (checkCurrentStaffVc()){
            wWidget = new AdmissionStartForm();
            AdmissionStartForm* admisStartForm = qobject_cast<AdmissionStartForm *>(wWidget);
            connect(admisStartForm, SIGNAL(openPersonal()), SLOT(openForm_personal()));
            connect(admisStartForm, SIGNAL(openSI()), SLOT(openForm_si()));
            connect(admisStartForm, SIGNAL(openMed()), SLOT(openForm_med()));
            connect(admisStartForm, SIGNAL(openAGroup()), SLOT(openForm_agroup()));
            connect(admisStartForm, SIGNAL(openZDE()), SLOT(openForm_zde()));
            connect(admisStartForm, SIGNAL(openAK()), SLOT(openForm_ak()));
            connect(admisStartForm, SIGNAL(openSopr()), SLOT(openForm_sopr()));
            connect(admisStartForm, SIGNAL(openZNK()), SLOT(openForm_znk()));
            connect(admisStartForm, SIGNAL(openGuard()), SLOT(openForm_guard()));
            connect(admisStartForm, SIGNAL(openKran()), SLOT(openForm_kran()));
            connect(admisStartForm, SIGNAL(openDamagedSI()), SLOT(openForm_damagedsi()));
            connect(admisStartForm, SIGNAL(openTechLPA()), SLOT(openForm_techlpa()));
            connect(admisStartForm, SIGNAL(open1ZDE()), SLOT(openForm_1znk()));
            connect(admisStartForm, SIGNAL(open2ZDE()), SLOT(openForm_2znk()));
            connect(admisStartForm, SIGNAL(openMech()), SLOT(openForm_mech()));
            connect(admisStartForm, SIGNAL(openPZNK()), SLOT(openForm_pznk()));
            connect(admisStartForm, SIGNAL(openRadio()), SLOT(openForm_radio()));
            connect(admisStartForm, SIGNAL(openRazvod()), SLOT(openForm_razvod()));
        } else {
            QMessageBox::warning(this, "Ошибка", "Данные по допускам заполняются только на ПАК-О");
            QWidget* wid = 0;
            return wid;
        }
    }
    else if ( formName == PERSONAL_ASSIGN )
    {
        wWidget = new AdmissonMainForm(AdmissonMainForm::Personal, Vc);
    }
    else if ( formName == SI_ASSIGN )
    {
        wWidget = new AdmissonMainForm(AdmissonMainForm::Si, Vc);
    }
    else if ( formName == MED_ASSIGN )
    {
        wWidget = new AdmissonMainForm(AdmissonMainForm::Med, Vc);
    }
    else if ( formName == AGROUP_ASSIGN )
    {
        wWidget = new AdmissonMainForm(AdmissonMainForm::AGroup, Vc);
    }
    else if ( formName == ZDE_ASSIGN )
    {
        wWidget = new AdmissonMainForm(AdmissonMainForm::Zde, Vc);
    }
    else if ( formName == AK_ASSIGN )
    {
        wWidget = new AdmissonMainForm(AdmissonMainForm::Ak, Vc);
    }
    else if ( formName == SOPR_ASSIGN )
    {
        wWidget = new AdmissonMainForm(AdmissonMainForm::Sopr, Vc);
    }
    else if ( formName == ZNK_ASSIGN )
    {
        wWidget = new AdmissonMainForm(AdmissonMainForm::Znk, Vc);
    }
    else if ( formName == GUARD_ASSIGN )
    {
        wWidget = new AdmissonMainForm(AdmissonMainForm::Guard, Vc);
    }
    else if ( formName == KRAN_ASSIGN )
    {
        wWidget = new AdmissonMainForm(AdmissonMainForm::Kran, Vc);
    }
    else if ( formName == DAMAGEDSI_ASSIGN )
    {
        wWidget = new AdmissonMainForm(AdmissonMainForm::DamagedSi, Vc);
    }
    else if ( formName == TECHLPA_ASSIGN )
    {
        wWidget = new AdmissonMainForm(AdmissonMainForm::TechLpa, Vc);
    }
    else if ( formName ==  "1-й ЗНЭШ")
    {
        wWidget = new AdmissonMainForm(AdmissonMainForm::firstZde, Vc);
    }
    else if ( formName ==  "2-ы ЗНЭШ")
    {
        wWidget = new AdmissonMainForm(AdmissonMainForm::secondZde, Vc);
    }
    else if ( formName ==  "Механик спец вагона")
    {
        wWidget = new AdmissonMainForm(AdmissonMainForm::Mech, Vc);
    }
    else if ( formName ==  "Помощник начальника караула")
    {
        wWidget = new AdmissonMainForm(AdmissonMainForm::Pznk, Vc);
    }
    else if ( formName ==  "Расчет радиостанции")
    {
        wWidget = new AdmissonMainForm(AdmissonMainForm::Radio, Vc);
    }
    else if ( formName == "Разводящие")
    {
        wWidget = new AdmissonMainForm(AdmissonMainForm::Razvod, Vc);
    }
    else if ( formName == START_RESCALC )
    {
         if (checkCurrentStaffVc()){
            wWidget = new ResCalcStartForm();
            ResCalcStartForm* resCalcForm = qobject_cast<ResCalcStartForm *>(wWidget);
            connect(resCalcForm, SIGNAL(openOrder()), SLOT(openForm_orders()));
            connect(resCalcForm, SIGNAL(openRails()), SLOT(openForm_rails()));
            connect(resCalcForm, SIGNAL(openCars()), SLOT(openForm_cars()));
            connect(resCalcForm, SIGNAL(openTzuSk()), SLOT(openForm_tzusk()));
            connect(resCalcForm, SIGNAL(openEngTech()), SLOT(openForm_engtech()));
            connect(resCalcForm, SIGNAL(openSupportMachines()), SLOT(openForm_supportmachines()));
            connect(resCalcForm, SIGNAL(openCalcRails(int)), SLOT(openForm_calcrails(int)));
            connect(resCalcForm, SIGNAL(openCalcPeople(int)), SLOT(openForm_calcpeople(int)));
            connect(resCalcForm, SIGNAL(openCalcWeapons(int)), SLOT(openForm_calcweapons(int)));
            connect(resCalcForm, SIGNAL(msgToStatus(QString)), SLOT( writeStatus(QString) ) );
        } else {
             QMessageBox::warning(this, "Ошибка", "Определение сил и средств доступно только на ПАК-О");
             QWidget* wid = 0;
             return wid;
         }
    }
    else if ( formName == RAILS )
    {
        wWidget = new ResStoreForm(ResStoreForm::Rails);
    }
    else if ( formName == CARS )
    {
        wWidget = new ResStoreForm(ResStoreForm::Cars);
    }
    else if ( formName == TZUSK )
    {
        wWidget = new ResStoreForm(ResStoreForm::TzuSk);
    }
    else if ( formName == ENGTECH )
    {
        wWidget = new ResStoreForm(ResStoreForm::EngTech);
    }
    else if ( formName == SUPPMACHINES )
    {
        wWidget = new ResStoreForm(ResStoreForm::SuppMach);
    }
    else if ( formName == CALCRAILS )
    {
        wWidget = new RailsCalcForm(Vc);
        RailsCalcForm* rCalcForm = qobject_cast<RailsCalcForm *>(wWidget);
        connect(rCalcForm, SIGNAL(msgToStatus(QString)), SLOT( writeStatus(QString) ) );
    }
    else if ( formName == CALCPEOPLE )
    {
        wWidget = new PeopleCalcForm(Vc);
        PeopleCalcForm* pCalcForm = qobject_cast<PeopleCalcForm *>(wWidget);
        connect(pCalcForm, SIGNAL(msgToStatus(QString)), SLOT( writeStatus(QString) ) );
    }
    else if ( formName == CALCWEAPONS )
    {
        wWidget = new WeapMatCalcForm(Vc);
        WeapMatCalcForm* wCalcForm = qobject_cast<WeapMatCalcForm *>(wWidget);
        connect(wCalcForm, SIGNAL(msgToStatus(QString)), SLOT( writeStatus(QString) ) );
    }
    else if ( formName == CLAIM )
    {
        wWidget = new ClaimForm();
        ClaimForm* claimForm = qobject_cast<ClaimForm *>(wWidget);
        connect(claimForm, SIGNAL(msgToStatus(QString)), SLOT( writeStatus(QString) ) );
    }

    if ( wWidget ){
        createMdiChild(formName, wWidget);
    }

    return wWidget;
}

MdiChild *MainWindow::activeMdiChild()
{
    if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
        return qobject_cast<MdiChild *>(activeSubWindow);
    return 0;
}

QMdiSubWindow *MainWindow::findMdiChild(const QString &childName)
{
    foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
        MdiChild *mdiChild = qobject_cast<MdiChild *>(window);
        if (mdiChild->formName() == childName)
            return window;
    }
    return 0;
}

void MainWindow::setActiveSubWindow(QMdiSubWindow *window)
{
    if (!window)
        return;
    mdiArea->setActiveSubWindow(window);
}

void MainWindow::openDoc(const QString &docName)
{
    WordMaker* docMaker = new WordMaker();
    connect(docMaker, SIGNAL(msgToStatus(QString)), SLOT( writeStatus(QString) ) );
    if (OrderForm::get_current_order_id() == 0){
        QMessageBox::warning(0,"Ошибка", "Выберите распоряжение для сборки документов");
        return;
    }
    docMaker->makeDoc(docName, OrderForm::get_current_order_id());
    delete docMaker;
}
