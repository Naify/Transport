#include "CompleterLineEdit.h"

#include <QCompleter>
#include <QAbstractItemView>
#include <QScrollBar>

CompleterLineEdit::CompleterLineEdit(QWidget *parent)
    : QLineEdit(parent),
    m_c(0)
{
}
//
CompleterLineEdit::~CompleterLineEdit()
{
    delete m_c;
}
void CompleterLineEdit::keyPressEvent(QKeyEvent *e)
{
    QLineEdit::keyPressEvent(e);
    if (!m_c)
        return;

    m_c->setCompletionPrefix(cursorWord(this->text()));
    if (m_c->completionPrefix().length() < 1)
    {
        m_c->popup()->hide();
        return;
    }
    QRect cr = cursorRect();
    cr.setWidth(m_c->popup()->sizeHintForColumn(0)
                + m_c->popup()->verticalScrollBar()->sizeHint().width());
    m_c->complete(cr);
}
//
//void CompleterLineEdit::setCompleter(QCompleter* completer)
//{
//    m_c = completer;
//    m_c->setWidget(this);
//    connect(m_c, SIGNAL(activated(const QString&)),
//            this, SLOT(insertCompletion(const QString&)));
//}
//
QString CompleterLineEdit::cursorWord(const QString &sentence) const
{
    return sentence.mid(sentence.left(cursorPosition()).lastIndexOf(QRegExp("\\s")) + 1,
                        cursorPosition() - sentence.left(cursorPosition()).lastIndexOf(QRegExp("\\s")) - 1);
}

void CompleterLineEdit::insertCompletion(const QString& arg)
{
    setText(text().replace(text().left(cursorPosition()).lastIndexOf(QRegExp("\\s")) + 1,
                           cursorPosition() -
                           text().left(cursorPosition()).lastIndexOf(QRegExp("\\s")) - 1,
                           arg));
}
