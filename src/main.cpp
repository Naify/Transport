#include <QApplication>
#include <QTextCodec>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include "mainwindow.h"
#include "LoginForm/CLogin.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(transport_control);

	QApplication app(argc, argv);

#if QT_VERSION < 0x050000
    QTextCodec::setCodecForTr( QTextCodec::codecForName("UTF-8") );
    QTextCodec::setCodecForCStrings( QTextCodec::codecForName("UTF-8") );
#endif

	app.setStyle("plastique");

    QDir dir(QDir::homePath()+"/transport/");
    if (!dir.exists()){
        dir.mkpath(QDir::homePath()+"/transport/");
    }

    CLogin dLogin;
    dLogin.init(QDir::homePath()+"/transport/config.ini");

    if ( dLogin.exec() == QDialog::Rejected )
    {        
        return 1;
    }

	MainWindow mainWin;
    mainWin.setWindowIcon(QIcon(":/images/transport_icon.png"));
    mainWin.showMaximized();

	return app.exec();
}
