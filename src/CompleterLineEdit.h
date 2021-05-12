#ifndef COMPLETERLINEEDIT_H
#define COMPLETERLINEEDIT_H

#include <QLineEdit>

class QCompleter;

class CompleterLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    CompleterLineEdit(QWidget *parent = 0);
    ~CompleterLineEdit();
    //
//    void setCompleter(QCompleter* completer);
protected:
    void keyPressEvent(QKeyEvent *e);
private:
    QCompleter* m_c;
    QString cursorWord(const QString&sentence) const;
private slots:
    void insertCompletion(const QString& arg);
};

#endif // COMPLETERLINEEDIT_H
