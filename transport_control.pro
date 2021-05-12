TEMPLATE = app

CONFIG += qt \    
    warn_on

QT += core \
    gui \
    xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH = . \
    src \
    ui

UI_DIR = ./ui

DESTDIR = ./bin

win32{
   INCLUDEPATH += "C:/Program Files/Eureca/Ods/include" \
                  "C:/Program Files/Eureca/wml/include" \
                  "C:/lib" \#staffstruct
                  "C:/Program Files/Eureca/IRS/include"
   LIBS += -L"C:/Program Files/Eureca/Ods/lib" \
           -L"C:/Program Files/Eureca/system/ptconvert" \
           -L"C:/Program Files/Eureca/system/postgres" \
           -L"C:/Users/user/Downloads/6/Kerberos/bin" \
           -L"C:/lib" \ #staffstruct
           -L"C:/Program Files/Eureca/IRS/bin" \
           -L"C:/Program Files/Eureca/wml/lib" \
           -lOdsInterface1 \
           -llibpq \
           -lipgsql1 \
           -lwshelp32 \
           -lptconvert \
           -lstaffStructure1 \
           -lwml1 \
           -lirs
}
unix{
   INCLUDEPATH += /usr/local/ods/include \
             /usr/local/odsStaff/include \
             /usr/local/irs/include \
            /usr/local/wml/include \
            /usr/local/planapi/include
   LIBS += -L/usr/local/ods/lib -lOdsInterface \
              -L/usr/local/odsStaff/lib -lstaffStructure \
              -L/usr/local/irs/lib -lirs \
              -L/usr/local/wml/lib -lwml \
              -L/usr/local/planapi/lib -lplanapi \
              -L/usr/local/ods/lib -lipgsql
}

CONFIG(debug, debug|release) { 
    # Debug
    unix:TARGET = $$join(TARGET,,,_debug)
    else:TARGET = $$join(TARGET,,,d)
    OBJECTS_DIR = ./build/debug/.obj
    MOC_DIR = ./build/debug/.moc
    RCC_DIR = ./build/debug/.rcc
}
else { 
    # Release
    OBJECTS_DIR = ./build/release/.obj
    MOC_DIR = ./build/release/.moc
    RCC_DIR = ./build/release/.rcc
}

SOURCES += \
#    src/delegate.cpp \
    src/main.cpp \
    src/LoginForm/CLogin.cpp \
    src/mainwindow.cpp \
    src/mdichild.cpp \
    src/global_defs.cpp \
    src/OrderForm/orderform.cpp \
    src/OrderForm/addcontentform.cpp \
    src/CargoForm/CargoForm.cpp \
    src/CargoForm/CargoModel.cpp \
    src/header_view/ProxyModelWithHeaderModels.cpp \
    src/header_view/HierarchicalHeaderView.cpp \
    src/CargoForm/CargoItemDelegate.cpp \
    src/AutoForm/AutoTransportForm.cpp \
    src/AutoForm/AutoTransportModel.cpp \
    src/AutoForm/AutoTransportItemDelegate.cpp \
    src/EscortForm/EscortForm.cpp \
    src/EscortForm/EscortModel.cpp \
    src/EscortForm/EscortItemDelegate.cpp \
    src/EscortCntrlForm/EscortControlForm.cpp \
    src/EscortCntrlForm/EscortControlModel.cpp \
    src/EscortCntrlForm/EscortControlItemDelegate.cpp \
    src/PartsForm/PartsForm.cpp \
    src/PartsForm/PartsModel.cpp \
    src/PartsForm/PartsItemDelegate.cpp \
    src/TrainForm/TransportForm.cpp \
    src/TrainForm/TransportModel.cpp \
    src/TrainForm/TransportItemDelegate.cpp \
    src/TravelAuthForm/TravelAuthtorizationForm.cpp \
    src/TravelAuthForm/TravelAuthtorizationModel.cpp \
    src/CompleterLineEdit.cpp \
    src/ShuttleTaskForm/createshuttletask.cpp \
    src/ShuttleTaskForm/ShuttleItemDelegate.cpp \
    src/ShuttleTaskForm/ShuttleModel.cpp \
    src/wordmaker.cpp \
    src/ShipmentForm/ShipmentForm.cpp \
    src/ShipmentForm/ShipmentModel.cpp \
    src/ShipmentForm/ShipmentItemDelegate.cpp \
    src/AdmissionForms/AdmissionStartForm/admissionstartform.cpp \
    src/AdmissionForms/AdmissionMainForm/admissonmainform.cpp \
    src/AdmissionForms/AdmissionMainForm/si/siadmissionmodel.cpp \
    src/AdmissionForms/AdmissionMainForm/person/personadmissionmodel.cpp \
    src/AdmissionForms/AdmissionMainForm/med/medadmissionmodel.cpp \
    src/AdmissionForms/AdmissionMainForm/agroup/agroupadmissionmodel.cpp \
    src/AdmissionForms/AdmissionMainForm/zde/zdeadmissionmodel.cpp \
    src/AdmissionForms/AdmissionMainForm/ak/akadmissionmodel.cpp \
    src/AdmissionForms/AdmissionMainForm/sopr/sopradmissionmodel.cpp \
    src/AdmissionForms/AdmissionMainForm/znk/znkadmissionmodel.cpp \
    src/AdmissionForms/AdmissionMainForm/guard/guardadmissionmodel.cpp \
    src/AdmissionForms/AdmissionMainForm/kran/kranadmissionmodel.cpp \
    src/AdmissionForms/AdmissionMainForm/damagedsi/damagedsiadmissionmodel.cpp \
    src/AdmissionForms/AdmissionMainForm/tech/techlpaadmissionmodel.cpp \
    src/AdmissionForms/AdmissionMainForm/damagedsi/damagedsiitemdelegate.cpp \
    src/AdmissionForms/AdmissionMainForm/agroup/agroupitemdelegate.cpp \
    src/AdmissionForms/AdmissionMainForm/ak/akitemdelegate.cpp \
    src/AdmissionForms/AdmissionMainForm/guard/guarditemdelegate.cpp \
    src/AdmissionForms/AdmissionMainForm/kran/kranitemdelegate.cpp \
    src/AdmissionForms/AdmissionMainForm/med/meditemdelegate.cpp \
    src/AdmissionForms/AdmissionMainForm/person/personitemdelegate.cpp \
    src/AdmissionForms/AdmissionMainForm/si/siitemdelegate.cpp \
    src/AdmissionForms/AdmissionMainForm/sopr/sopritemdelegate.cpp \
    src/AdmissionForms/AdmissionMainForm/tech/techlpaitemdelegate.cpp \
    src/AdmissionForms/AdmissionMainForm/znk/znkitemdelegate.cpp \
    src/AdmissionForms/AdmissionMainForm/zde/zdeitemdelegate.cpp \
    src/AdmissionForms/AdmissionMainForm/choosepersondialog.cpp \
    src/ResoursesCalculationsForms/ResCalcStartForm/rescalcstartform.cpp \
    src/ResoursesCalculationsForms/ResStoreForm/resstoreform.cpp \
    src/ResoursesCalculationsForms/ResStoreForm/RailsStore/railsmodel.cpp \
    src/ResoursesCalculationsForms/ResStoreForm/CarsStore/carsmodel.cpp \
    src/ResoursesCalculationsForms/ResStoreForm/TzuStore/tzuskmodel.cpp \
    src/ResoursesCalculationsForms/ResStoreForm/EngTechStore/engtechmodel.cpp \
    src/ResoursesCalculationsForms/ResStoreForm/SuppMachinesStore/supportmachinesmodel.cpp \
    src/ResoursesCalculationsForms/ResStoreForm/CarsStore/carsidelegate.cpp \
    src/ResoursesCalculationsForms/ResStoreForm/EngTechStore/engtechidelegate.cpp \
    src/ResoursesCalculationsForms/ResStoreForm/RailsStore/railsidelegate.cpp \
    src/ResoursesCalculationsForms/ResStoreForm/SuppMachinesStore/suppmachinesidelegate.cpp \
    src/ResoursesCalculationsForms/ResStoreForm/TzuStore/tzuidelegate.cpp \
    src/ResoursesCalculationsForms/ResCalcForm/RailCalc/railscalcform.cpp \
    src/ResoursesCalculationsForms/ResCalcForm/PeopleCalc/peoplecalcform.cpp \
    src/ResoursesCalculationsForms/ResCalcForm/WeapMatCalc/weapmatcalcform.cpp \
    src/ResoursesCalculationsForms/ResCalcForm/PeopleCalc/choosepeopledialog.cpp \
    src/ResoursesCalculationsForms/ResCalcForm/PeopleCalc/showescortdialog.cpp \
    src/ClaimForm/claimform.cpp \
    src/ClaimForm/claimmodel.cpp \
    src/ClaimForm/claimdelegate.cpp \
    src/AdmissionForms/AdmissionMainForm/1zde/firstzdemodel.cpp \
    src/AdmissionForms/AdmissionMainForm/1zde/firstzdedelegate.cpp \
    src/AdmissionForms/AdmissionMainForm/2zde/secondzdemodel.cpp \
    src/AdmissionForms/AdmissionMainForm/2zde/secondzdedelegate.cpp \
    src/AdmissionForms/AdmissionMainForm/mech/mechmodel.cpp \
    src/AdmissionForms/AdmissionMainForm/mech/mechitemdelegate.cpp \
    src/AdmissionForms/AdmissionMainForm/pznk/pznkmodel.cpp \
    src/AdmissionForms/AdmissionMainForm/pznk/pznktemdelegate.cpp \
    src/AdmissionForms/AdmissionMainForm/radio/radiomodel.cpp \
    src/AdmissionForms/AdmissionMainForm/radio/radioitemdelegate.cpp \
    src/AdmissionForms/AdmissionMainForm/razvod/razvodmodel.cpp \
    src/AdmissionForms/AdmissionMainForm/razvod/razvoditemdelegate.cpp \
    src/ResoursesCalculationsForms/ResCalcForm/PeopleCalc/showescortdialogmodel.cpp \
    src/ResoursesCalculationsForms/ResCalcForm/PeopleCalc/showescortdialogdelegate.cpp

HEADERS += \
#    src/delegate.h \
    src/LoginForm/CLogin.h \
    src/mainwindow.h \
    src/mdichild.h \
    src/global_defs.h \
    src/OrderForm/orderform.h \
    src/OrderForm/addcontentform.h \
    src/CargoForm/CargoForm.h \
    src/CargoForm/CargoModel.h \
    src/header_view/ProxyModelWithHeaderModels.h \
    src/header_view/HierarchicalHeaderView.h \
    src/CargoForm/CargoItemDelegate.h \
    src/AutoForm/AutoTransportForm.h \
    src/AutoForm/AutoTransportModel.h \
    src/AutoForm/AutoTransportItemDelegate.h \
    src/EscortForm/EscortForm.h \
    src/EscortForm/EscortModel.h \
    src/EscortForm/EscortItemDelegate.h \
    src/EscortCntrlForm/EscortControlForm.h \
    src/EscortCntrlForm/EscortControlModel.h \
    src/EscortCntrlForm/EscortControlItemDelegate.h \
    src/PartsForm/PartsForm.h \
    src/PartsForm/PartsModel.h \
    src/PartsForm/PartsItemDelegate.h \
    src/TrainForm/TransportForm.h \
    src/TrainForm/TransportModel.h \
    src/TrainForm/TransportItemDelegate.h \
    src/TravelAuthForm/TravelAuthtorizationForm.h \
    src/TravelAuthForm/TravelAuthtorizationModel.h \
    src/CompleterLineEdit.h \
    src/ShuttleTaskForm/createshuttletask.h \
    src/ShuttleTaskForm/ShuttleItemDelegate.h \
    src/ShuttleTaskForm/ShuttleModel.h \
    src/wordmaker.h \
    src/ShipmentForm/ShipmentForm.h \
    src/ShipmentForm/ShipmentModel.h \
    src/ShipmentForm/ShipmentItemDelegate.h \
    src/AdmissionForms/AdmissionStartForm/admissionstartform.h \
    src/AdmissionForms/AdmissionMainForm/admissonmainform.h \
    src/AdmissionForms/AdmissionMainForm/si/siadmissionmodel.h \
    src/AdmissionForms/AdmissionMainForm/person/personadmissionmodel.h \
    src/AdmissionForms/AdmissionMainForm/med/medadmissionmodel.h \
    src/AdmissionForms/AdmissionMainForm/agroup/agroupadmissionmodel.h \
    src/AdmissionForms/AdmissionMainForm/zde/zdeadmissionmodel.h \
    src/AdmissionForms/AdmissionMainForm/ak/akadmissionmodel.h \
    src/AdmissionForms/AdmissionMainForm/sopr/sopradmissionmodel.h \
    src/AdmissionForms/AdmissionMainForm/znk/znkadmissionmodel.h \
    src/AdmissionForms/AdmissionMainForm/guard/guardadmissionmodel.h \
    src/AdmissionForms/AdmissionMainForm/kran/kranadmissionmodel.h \
    src/AdmissionForms/AdmissionMainForm/damagedsi/damagedsiadmissionmodel.h \
    src/AdmissionForms/AdmissionMainForm/tech/techlpaadmissionmodel.h \
    src/AdmissionForms/AdmissionMainForm/damagedsi/damagedsiitemdelegate.h \
    src/AdmissionForms/AdmissionMainForm/agroup/agroupitemdelegate.h \
    src/AdmissionForms/AdmissionMainForm/ak/akitemdelegate.h \
    src/AdmissionForms/AdmissionMainForm/guard/guarditemdelegate.h \
    src/AdmissionForms/AdmissionMainForm/kran/kranitemdelegate.h \
    src/AdmissionForms/AdmissionMainForm/med/meditemdelegate.h \
    src/AdmissionForms/AdmissionMainForm/person/personitemdelegate.h \
    src/AdmissionForms/AdmissionMainForm/si/siitemdelegate.h \
    src/AdmissionForms/AdmissionMainForm/sopr/sopritemdelegate.h \
    src/AdmissionForms/AdmissionMainForm/tech/techlpaitemdelegate.h \
    src/AdmissionForms/AdmissionMainForm/znk/znkitemdelegate.h \
    src/AdmissionForms/AdmissionMainForm/zde/zdeitemdelegate.h \
    src/AdmissionForms/AdmissionMainForm/choosepersondialog.h \
    src/ResoursesCalculationsForms/ResCalcStartForm/rescalcstartform.h \
    src/ResoursesCalculationsForms/ResStoreForm/resstoreform.h \
    src/ResoursesCalculationsForms/ResStoreForm/RailsStore/railsmodel.h \
    src/ResoursesCalculationsForms/ResStoreForm/CarsStore/carsmodel.h \
    src/ResoursesCalculationsForms/ResStoreForm/TzuStore/tzuskmodel.h \
    src/ResoursesCalculationsForms/ResStoreForm/EngTechStore/engtechmodel.h \
    src/ResoursesCalculationsForms/ResStoreForm/SuppMachinesStore/supportmachinesmodel.h \
    src/ResoursesCalculationsForms/ResStoreForm/CarsStore/carsidelegate.h \
    src/ResoursesCalculationsForms/ResStoreForm/EngTechStore/engtechidelegate.h \
    src/ResoursesCalculationsForms/ResStoreForm/RailsStore/railsidelegate.h \
    src/ResoursesCalculationsForms/ResStoreForm/SuppMachinesStore/suppmachinesidelegate.h \
    src/ResoursesCalculationsForms/ResStoreForm/TzuStore/tzuidelegate.h \
    src/ResoursesCalculationsForms/ResCalcForm/RailCalc/railscalcform.h \
    src/ResoursesCalculationsForms/ResCalcForm/PeopleCalc/peoplecalcform.h \
    src/ResoursesCalculationsForms/ResCalcForm/WeapMatCalc/weapmatcalcform.h \
    src/ResoursesCalculationsForms/ResCalcForm/PeopleCalc/choosepeopledialog.h \
    src/ResoursesCalculationsForms/ResCalcForm/PeopleCalc/showescortdialog.h \
    src/ClaimForm/claimform.h \
    src/ClaimForm/claimmodel.h \
    src/ClaimForm/claimdelegate.h \
    src/AdmissionForms/AdmissionMainForm/1zde/firstzdemodel.h \
    src/AdmissionForms/AdmissionMainForm/1zde/firstzdedelegate.h \
    src/AdmissionForms/AdmissionMainForm/2zde/secondzdemodel.h \
    src/AdmissionForms/AdmissionMainForm/2zde/secondzdedelegate.h \
    src/AdmissionForms/AdmissionMainForm/mech/mechmodel.h \
    src/AdmissionForms/AdmissionMainForm/mech/mechitemdelegate.h \
    src/AdmissionForms/AdmissionMainForm/pznk/pznkmodel.h \
    src/AdmissionForms/AdmissionMainForm/pznk/pznktemdelegate.h \
    src/AdmissionForms/AdmissionMainForm/radio/radiomodel.h \
    src/AdmissionForms/AdmissionMainForm/radio/radioitemdelegate.h \
    src/AdmissionForms/AdmissionMainForm/razvod/razvodmodel.h \
    src/AdmissionForms/AdmissionMainForm/razvod/razvoditemdelegate.h \
    src/ResoursesCalculationsForms/ResCalcForm/PeopleCalc/showescortdialogmodel.h \
    src/ResoursesCalculationsForms/ResCalcForm/PeopleCalc/showescortdialogdelegate.h

RESOURCES += images/transport_control.qrc

FORMS += \
    ui/loginForm.ui \
    ui/orderform.ui \
    ui/addcontentform.ui \
    ui/CargoForm.ui \
    ui/AutoTransportForm.ui \
    ui/EscortForm.ui \
    ui/EscortControlForm.ui \
    ui/PartsForm.ui \
    ui/TransportForm.ui \
    ui/TravelAuthtorizationForm.ui \
    ui/createshuttletask.ui \
    ui/ShipmentForm.ui \
    ui/admissionstartform.ui \
    ui/admissonmainform.ui \
    ui/choosepersondialog.ui \
    ui/rescalcstartform.ui \
    ui/resstoreform.ui \
    ui/railscalcform.ui \
    ui/peoplecalcform.ui \
    ui/weapmatcalcform.ui \
    ui/choosepeopledialog.ui \
    ui/showescortdialog.ui \
    ui/claimform.ui

include(./src/pri_files/post_script.pri)
