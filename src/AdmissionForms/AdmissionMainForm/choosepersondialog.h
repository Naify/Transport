#ifndef CHOOSEPERSONDIALOG_H
#define CHOOSEPERSONDIALOG_H

#include <QDialog>

namespace Ui {
class ChoosePersonDialog;
}

class QListWidgetItem;

class ChoosePersonDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ChoosePersonDialog(QWidget *parent = 0);
    ~ChoosePersonDialog();
    
private:
    Ui::ChoosePersonDialog *ui;
    void init();

private slots:
    void handleOkButton();

signals:
    void sendId(int);
};

#endif // CHOOSEPERSONDIALOG_H
