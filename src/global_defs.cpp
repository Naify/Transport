#include <QString>
#include <QVariant>
#include <math.h>
#include <OdsInterface>
#include "global_defs.h"
#include <QMessageBox>

bool RESOLUTION_HIGH = false;


/*
QString qstring_6(const QString & src)
{
    QString res( src );
    while ( res.length() < 4 )
        res.prepend("0");
    return res;
}

QString intToString(int value)
{
    QString res( QString::number(value));

    for (int i=3; i<res.length(); i+=4)
    {
        res.insert(res.length() - i," ");
    }

    return res;
}

QString doubleToString(double d, bool rub)
{
    qulonglong i = round( d*100.0 );
    QString tmpQstr = QVariant( i ).toString();

    //	QMessageBox::warning(this,"Ошибка", tmpQstr );

    tmpQstr.insert( tmpQstr.size() - 2, ".");
    if ( tmpQstr.size() > 6) tmpQstr.insert( tmpQstr.size() - 6, " ");
    if ( tmpQstr.size() > 10)	tmpQstr.insert( tmpQstr.size() - 10, " ");

    if ( rub ){
        tmpQstr.append(" р.");
    }
    return tmpQstr;
}

*/
QString moneyToPropis(double n)
{
    if ( n > 999999999999.99 )
        return QString("Очень много денег ...");

    if ( n < 0 )
        return QString("Отрицательная сумма !");

    static QString cap[4][10] =
    {
        {
            "",
            QString("один "),
            QString("два "),
            QString("три "),
            QString("четыре "),
            QString("пять "),
            QString("шесть "),
            QString("семь "),
            QString("восемь "),
            QString("девять ")
        },
        {
            "",
            QString("одна "),
            QString("две "),
            QString("три "),
            QString("четыре "),
            QString("пять "),
            QString("шесть "),
            QString("семь "),
            QString("восемь "),
            QString("девять ")
        },
        {
            "",
            "",
            QString("двадцать "),
            QString("тридцать "),
            QString("сорок "),
            QString("пятьдесят "),
            QString("шестьдесят "),
            QString("семьдесят "),
            QString("восемьдесят "),
            QString("девяносто ")
        },
        {
            "",
            QString("сто "),
            QString("двести "),
            QString("триста "),
            QString("четыреста "),
            QString("пятьсот "),
            QString("шестьсот "),
            QString("семьсот "),
            QString("восемьсот "),
            QString("девятьсот ")
        }
    };

    static QString cap2[10] =
    {
        QString("десять "),
        QString("одиннадцать "),
        QString("двенадцать "),
        QString("тринадцать "),
        QString("четырнадцать "),
        QString("пятнадцать "),
        QString("шестнадцать "),
        QString("семнадцать "),
        QString("восемнадцать "),
        QString("девятнадцать ")
    };

    static QString cap3[5][3] =
    {
        {
            QString("копейка "),
            QString("копейки "),
            QString("копеек ")
        },
        {
            QString("рубль  "),
            QString("рубля  "),
            QString("рублей ")
        },
        {
            QString("тысяча "),
            QString("тысячи "),
            QString("тысяч ")
        },
        {
            QString("миллион "),
            QString("милиона "),
            QString("миллионов ")
        },
        {
            QString("миллиард "),
            QString("миллиарда "),
            QString("миллиардов ")
        }
    };

    QString s;

    qlonglong nn = ((qlonglong)(n)*10000 + (qlonglong)(n*1000) % 1000 + 5 ) / 10;

    qlonglong lo = 0;
    qlonglong hi = nn;
    int r[3];
    int rod;

    for ( int nPor = 0; true; ++nPor )
    {
        QString olds = s;
        s = "";

        lo = hi%1000;
        hi = hi/1000;

        if ((nPor == 1) && (lo == 0) && (hi == 0))
        {
            s = QString("ноль ") + cap3[1][2] + olds;
            break;
        }

        for ( int nRaz = 0; nRaz < 3; ++nRaz )
        {
            r[nRaz] = lo%10;
            lo      = lo/10;
        }

        switch ( nPor )
        {
        case 0:
        case 1:
        case 2:
            rod = 1;
            break;
        default:
            rod = 0;
        }

        QString capt;
        if (r[1] != 1)
        {
            if (r[0] == 1)
                capt = cap3[nPor][0];
            else if ((r[0] > 1 ) && (r[0] < 5))
                capt = cap3[nPor][1];
        }
        if (capt.isEmpty())
            capt = cap3[nPor][2];

        if (nPor == 0)
        {
            s = QString::number(r[1]) + QString::number(r[0]) + " " + capt;
            continue;
        }
        if (nPor == 1)
            rod = 0;

        s += cap[2+1][r[2]];
        s += ((r[1] == 1) ? cap2[r[0]] : cap[1+1][r[1]]+cap[0+rod][r[0]])+capt;
        s += olds;

        if ((hi == 0) && (nPor >= 1))
            break;
    }
    QString up(s[0]);
    s.replace(0,1,up);
    return s;
}

bool setIObjRights(const ODS::IObject &iobj) {
    using namespace ODS;
    Group userGroup = OdsInterface::self()->subjectManager().getGroup(USER_GROUP_NAME);
    Group adminGroup = OdsInterface::self()->subjectManager().getGroup(ADMIN_GROUP_NAME);

    if (!userGroup.setRightsToIObject(iobj, Right::Read)) {
        QMessageBox::critical(0,"Ошибка", "Ошибка задания прав для записи");
        qDebug()<<"Ошибка задания прав группе "+userGroup.name();
        return false;
    }
    if (!adminGroup.setRightsToIObject(iobj, Right::All)) {
        QMessageBox::critical(0,"Ошибка", "Ошибка задания прав для записи");
        qDebug()<<"Ошибка задания прав группе "+adminGroup.name();
        return false;
    }
    User user = OdsInterface::self()->subjectManager().getCurrentUser();
    if (!user.setRightsToIObject(iobj, Right::NoRight)) {
        QMessageBox::critical(0,"Ошибка", "Ошибка задания прав для записи");
        return false;
    }
    return true;
}
