#include "LoginForm/CLogin.h"
#include <QPushButton>
#include <QMessageBox>
#include <QFile>
#include <QSettings>
#include <OdsInterface>

CLogin::CLogin(QWidget * parent) : QDialog( parent )
{
	setupUi(this);

	buttonBox->button(QDialogButtonBox::Cancel)->setText("Отмена");
    buttonBox->button(QDialogButtonBox::Ok)->setText("Вход");
}

CLogin::~CLogin()
{

}

void CLogin::init(QString iniFile)
{
	QDialog::reject(); // The default action for this Dialog

	iniFileName = iniFile;
	QSettings odsConfig(iniFileName, QSettings::IniFormat);

	odsConfig.beginGroup("ODS");
	QString ODS_IP = odsConfig.value("database_ip").toString();
	if ( ODS_IP.isEmpty() )
	{
        ODS_IP = "ssp";
		odsConfig.setValue( "database_ip", ODS_IP );
	}
	lineEdit_hostname->setText( ODS_IP );

	QString ODS_DB = odsConfig.value("database_name").toString();
	if ( ODS_DB.isEmpty() )
	{
        ODS_DB = "sovetnik_db";
		odsConfig.setValue( "database_name", ODS_DB );
	}
	lineEdit_dbname->setText( ODS_DB );

	QString ODS_USER = odsConfig.value("username").toString();
	if ( ODS_USER.isEmpty() )
	{
        ODS_USER = getenv("USER");
        QString domain = QString(getenv("ALD_DOMAIN")).toUpper();
        if ( ! domain.isEmpty() )
            ODS_USER.append( domain.replace(0, 1, "@") );
        odsConfig.setValue( "username", ODS_USER );
	}
	lineEdit_username->setText( ODS_USER );

	lineEdit_pwd->setFocus();

	odsConfig.endGroup();
}

void CLogin::accept()
{
	QString ODS_IP = lineEdit_hostname->text();
	QString ODS_DB = lineEdit_dbname->text();
	QString ODS_USER = lineEdit_username->text();
	QString ODS_PWD = lineEdit_pwd->text();

	ODS::LogManager logMgr = ODS::OdsInterface::self()->logManager();
    ODS::ConnectionManager cnMgr = ODS::OdsInterface::self()->connectionManager();
    if ( ! cnMgr.connect( ODS_USER,  ODS_PWD, "transport_control", ODS_IP, ODS_DB, "UTF8") ){
		qDebug() << "ODS connection error: " << logMgr.getLastError().fullTextDesc().toQString().toLocal8Bit();
//        QMessageBox::critical(0,"Ошибка", "Ошибка соединения с базой данных");
        QDialog::reject();
		return;
	}

	QSettings odsConfig(iniFileName, QSettings::IniFormat);

	odsConfig.beginGroup("ODS");
	odsConfig.setValue( "database_ip", ODS_IP );
	odsConfig.setValue( "database_name", ODS_DB );
	odsConfig.setValue( "username", ODS_USER );
	odsConfig.endGroup();

	QDialog::accept();
}
