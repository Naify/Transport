#ifndef ADDCONTENTFORM_H
#define ADDCONTENTFORM_H

#include <QWidget>

namespace Ui {
class AddContentForm;
}

class AddContentForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit AddContentForm(QWidget *parent = 0);
    ~AddContentForm();

private slots:
    void closeForm();
    void handleSaveButton();

private:
    Ui::AddContentForm *ui;

signals:
    void row_save();
    void msgToStatus(QString);
};

#endif // ADDCONTENTFORM_H
