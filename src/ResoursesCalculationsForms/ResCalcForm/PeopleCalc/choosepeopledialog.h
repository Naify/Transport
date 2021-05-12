#ifndef CHOOSEPEOPLEDIALOG_H
#define CHOOSEPEOPLEDIALOG_H

#include <QDialog>
#include <QMap>

namespace Ui {
class ChoosePeopleDialog;
}

class QListWidgetItem;

class ChoosePeopleDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ChoosePeopleDialog(int postId, int positionCount, int siId, QWidget *parent = 0);
    ~ChoosePeopleDialog();
    
private:
    Ui::ChoosePeopleDialog *ui;
    int posId;
    QString posName;
    int count;
    int siId;
    void init();
    void getValidPeople();
    void checkAdmissionWarnings(QString);
    bool checkPerson(int vcId, int personId);
    QMap<int,int> idList;

private slots:
    void lwClicked(QListWidgetItem *item);
    void handleOkButton();

signals:
    void sendIds(QMap<int,int>);
};

#endif // CHOOSEPEOPLEDIALOG_H
