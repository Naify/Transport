#ifndef ORDERFORM_H
#define ORDERFORM_H

#include <QWidget>

namespace Ui {
class OrderForm;
}

class OrderForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit OrderForm(QWidget *parent = 0);
    static int get_current_order_id() { return current_order_id; }
    static int get_row_count() { return row_count; }
    ~OrderForm();

public slots:
    void renewTableJournal();
    void renewTableOrders();

private slots:
    void closeForm();
    void journalClicked(int row, int column);
    void addOrder();
    void addContent();
    void addContentTable();
    void delContentTable();
    void makeWord();

private:
    Ui::OrderForm *ui;
    static int row_count;
    static int current_order_id;
    QString getSenderName(int sender);
    QString getCargoName(int cargo);

signals:
    void openAddForm();
    void msgToStatus(const QString);
};

#endif // ORDERFORM_H
