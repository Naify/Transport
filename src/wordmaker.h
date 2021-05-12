#ifndef WORDMAKER_H
#define WORDMAKER_H

#include <QString>
#include <QObject>
#include <OdsTemplateManager>

class WordMaker: public QObject
{
Q_OBJECT
public:
    WordMaker();
    ~WordMaker();
    void makeDoc(const QString &docName, int current_vc_id);

private:
    OdsTemplate constructCommander(OdsTemplate& templ, bool& ok, int current_vc_id);
    OdsTemplate constructinOutDoc(OdsTemplate& templ, bool& ok, int current_vc_id);
    OdsTemplate constructTabel(OdsTemplate& templ, bool& ok, int current_vc_id);
    OdsTemplate constructActs(OdsTemplate& templ, bool& ok, int current_vc_id);
    void constructComAuth(OdsTemplate& templ, bool& ok, int current_vc_id, QString docName);
    void constructComAuthPerson(OdsTemplate& templ, int current_vc_id, int personId);
    void openPlan();
    void saveAsDoc(OdsTemplate& templ, const QString &docName);    

signals:
    void msgToStatus(QString);
};

#endif // WORDMAKER_H
