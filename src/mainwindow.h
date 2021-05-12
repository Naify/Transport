#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "global_defs.h"
#include "OrderForm/orderform.h"

class MdiChild;

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;
class QTextDocument;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
    QWidget * openForm(const QString &, int Vc = 0);

protected:
	void closeEvent(QCloseEvent *event);

private slots:
    void openForm_orders(){openForm(ORDER_RECIEVE_SEND_TECH);}    
    void openForm_create_shuttle_task(){openForm(CREATE_SHUTTLE_TASK);}
    void openForm_add_content(){openForm(ADD_CONTENT);}
    void openForm_cargo(){openForm(CARGO);}
    void openForm_parts(){openForm(PARTS);}
    void openDoc_inout(){openDoc(IN_OUT_DOC);}
    void openForm_auto(){openForm(AUTO_TR);}
    void openForm_train(){openForm(TRAIN_TR);}
    void openForm_escort(){openForm(ESCORT);}
    void openForm_travel_auth(){openForm(TRAVEL_AUTH);}    
    void openDoc_travel_auth(){openDoc(TRAVEL_AUTH_DOC);}
    void openDoc_train(){openDoc(TRAIN_COMMANDER_DOC);}
    void openDoc_cars(){openDoc(CARS_COMMANDER_DOC);}
    void openDoc_tabel(){openDoc(TABEL_DOC);}
    void openDoc_posts(){openDoc(POSTS_DOC);}
    void openDoc_commander(){openDoc(COMMANDER_DOC);}
    void openDoc_acts(){openDoc(ACTS_DOC);}
    void openDoc_order_transp(){openDoc(ORDER_TRANSP_DOC);}
    void openDoc_plan(){openDoc(PLAN_DOC);}
    void openForm_shipment(){openForm(SHIPMENT);}

    void openForm_startassignment(){openForm(START_ASSIGN);}
    void openForm_personal(){openForm(PERSONAL_ASSIGN);}
    void openForm_si(){openForm(SI_ASSIGN);}
    void openForm_med(){openForm(MED_ASSIGN);}
    void openForm_agroup(){openForm(AGROUP_ASSIGN);}
    void openForm_zde(){openForm(ZDE_ASSIGN);}
    void openForm_ak(){openForm(AK_ASSIGN);}
    void openForm_sopr(){openForm(SOPR_ASSIGN);}
    void openForm_znk(){openForm(ZNK_ASSIGN);}
    void openForm_guard(){openForm(GUARD_ASSIGN);}
    void openForm_kran(){openForm(KRAN_ASSIGN);}
    void openForm_damagedsi(){openForm(DAMAGEDSI_ASSIGN);}
    void openForm_techlpa(){openForm(TECHLPA_ASSIGN);}
    void openForm_1znk(){openForm("1-й ЗНЭШ");}
    void openForm_2znk(){openForm("2-ы ЗНЭШ");}
    void openForm_mech(){openForm("Механик спец вагона");}
    void openForm_pznk(){openForm("Помощник начальника караула");}
    void openForm_radio(){openForm("Расчет радиостанции");}
    void openForm_razvod(){openForm("Разводящие");}

    void openForm_startrescalc(){openForm(START_RESCALC);}
    void openForm_rails(){openForm(RAILS);}
    void openForm_cars(){openForm(CARS);}
    void openForm_tzusk(){openForm(TZUSK);}
    void openForm_engtech(){openForm(ENGTECH);}
    void openForm_supportmachines(){openForm(SUPPMACHINES);}
    void openForm_calcrails(int id){openForm(CALCRAILS, id);}
    void openForm_calcpeople(int id){openForm(CALCPEOPLE, id);}
    void openForm_calcweapons(int id){openForm(CALCWEAPONS, id);}

    void openForm_claim(){openForm(CLAIM);}

    void writeStatus(const QString &message);

	void updateMenus();
	void updateWindowMenu();
	MdiChild *createMdiChild(const QString &, QWidget *);
	void setActiveSubWindow(QMdiSubWindow *);

    void openDoc(const QString &);

private:
	void createActions();
	void createMenus();
	void createStatusBar();
	void readSettings();
	void writeSettings();
    bool checkCurrentStaffVc();
    MdiChild* activeMdiChild();
    QMdiSubWindow* findMdiChild(const QString &fileName);

    QMdiArea* mdiArea;
    QSignalMapper* windowMapper;

	// MENU
    QMenu* orderMenu;
    QMenu* cargoMenu;
    QMenu* transportMenu;
    QMenu* escortMenu;
    QMenu* docMenu;
    QMenu* orgMenu;
    QMenu* assignMenu;
    QMenu* windowMenu;
    QMenu* resCalcMenu;
    QMenu* helpMenu;

    // MENU ACTIONS
    QAction* OrdersAction;
    QAction* OrdersDocAction;
    QAction* ShuttleAction;
    QAction* CargoAction;
    QAction* PartsAction;
    QAction* InOutDocAction;
    QAction* TrainAction;
    QAction* AutoAction;
    QAction* EscortAction;    
    QAction* TravelAuthAction;
    QAction* TravelAuthDocAction;
    QAction* PostsTableDocAction;
    QAction* PostsSheetDocActoin;
    QAction* CommanderDocAction;
    QAction* ActsDocAction;
    QAction* OrderTransportDocAction;
    QAction* TrainCommanderDocAction;
    QAction* CarsCommanderDocAction;
    QAction* PlanDocAction;
    QAction* ShipmentAction;

    QAction* StartAssignAction;
    QAction* PersonalAssignAction;
    QAction* SIAssignAction;
    QAction* MedAssignAction;
    QAction* AGroupAssignAction;
    QAction* ZDEAssignAction;
    QAction* AKAssignAction;
    QAction* SoprAssignAction;
    QAction* ZNKAssignAction;
    QAction* GuardAssignAction;
    QAction* KranAssignAction;
    QAction* DamagedSIAssignAction;
    QAction* TechLPAAssignAction;

    QAction* StartResCalcAction;

    QAction* ClaimAction;

    QAction* closeAct;
    QAction* closeAllAct;
    QAction* tileAct;
    QAction* cascadeAct;
    QAction* nextAct;
    QAction* previousAct;

    //Forms
    OrderForm* OrderForm_form;
};

#endif
