#ifndef CLOGIN_H
#define CLOGIN_H

#include <QDialog>
#include "ui_loginForm.h"

class CLogin : public QDialog, public Ui::Dialog
{
	Q_OBJECT
public:
	CLogin(QWidget * parent = 0);
	~CLogin();

	void init(QString);
	void accept();
private:
	QString iniFileName;
};

#endif //CLOGIN_H
