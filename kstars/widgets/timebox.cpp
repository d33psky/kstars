/***************************************************************************
                          timebox.cpp  -  description
                             -------------------
    begin                : Sun Jan 20 2002
    copyright            : (C) 2002 by Pablo de Vicente
    email                : vicente@oan.es
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "timebox.h"

#include <stdlib.h>

#include <QTime>
#include <QDate>
#include <QRegExp>

#include <kglobal.h>
#include <klocale.h>
#include <kdebug.h>

timeBox::timeBox(QWidget *parent, bool tt) : QLineEdit(parent)
{

    if (tt) {
        setMaxLength(9);
        setMaximumWidth(90);
    }
    else {
        setMaxLength(12);
        setMaximumWidth(120);
    }

    timet = tt;
}

void timeBox::showTime( const QTime &t )
{
    setEntry( t.toString("hh:mm:ss") );
}

void timeBox::showDate( const QDate &t )
{
    setEntry( t.toString() );

}

QTime timeBox::createTime ( bool *ok )
{
    //	QString entry;
    int h = 0, m = 0, is = 0;
    double s = 0.0;
    QTime qt;
    bool valueFound = false, badEntry = false , checkValue = false;

    //Initialize bool for result
    if ( ok != NULL ) *ok = false;

    //	QString errMsg = i18n( "Unable to parse %1 entry. Specify a %1 value as a simple integer, a floating-point number, or a triplet of values using colons or spaces as separators." );

    QString entry = text().trimmed();

    //Try simplest cases: integer or double representation

    h = entry.toInt( &checkValue );
    if ( checkValue ) {
        qt = QTime( h, 0, 0 );
        valueFound = true;
        if ( ok != NULL ) *ok = true;
        return qt;
    } else {
        double x = entry.toDouble( &checkValue );
        if ( checkValue ) {
            int seconds = int(x * 3600);
            QTime qt(0,0,0);
            qt.addSecs(seconds);
            valueFound = true;
            if ( ok != NULL ) *ok = true;
            return qt;
        }
    }

    //no success yet...try assuming multiple fields

    if ( !valueFound ) {
        QStringList fields;

        //check for colon-delimiters or space-delimiters
        if ( entry.contains(':') )
            fields = entry.split( ':', QString::SkipEmptyParts );
        else fields = entry.split( ' ', QString::SkipEmptyParts );

        // If two fields we will add a third one, and then parse with
        // the 3-field code block. If field[1] is a double, convert
        // it to integer mins, and convert the remainder to secs

        if ( fields.count() == 2 ) {
            double mx = fields[1].toDouble( &checkValue );
            if ( checkValue ) {
                fields[1] = QString("%1").arg( int(mx) );
                fields.append( QString("%1").arg( int( 60.0*(mx - int(mx)) ) ) );
            } else {
                fields.append( QString( "0" ) );
            }
        }

        // Three fields space-delimited ( h/d m s );
        // ignore all after 3rd field

        if ( fields.count() >= 3 ) {
            fields[0].replace( QRegExp("h"), QString() );
            fields[1].replace( QRegExp("m"), QString() );
            fields[2].replace( QRegExp("s"), QString() );
        }
        //See if first two fields parse as integers.
        //
        h = fields[0].toInt( &checkValue );
        if ( !checkValue ) badEntry = true;
        m = fields[1].toInt( &checkValue );
        if ( !checkValue ) badEntry = true;
        s = fields[2].toDouble( &checkValue );
        if ( !checkValue ) badEntry = true;

        if ( !badEntry ) {
            valueFound = true;
            is = (int)s;

            if ( ok != NULL ) *ok = true;

            QTime qt(h,m,is);
            return qt;

        } else {
            if ( ok != NULL ) *ok = false;
        }
    }

    //	 if ( !valueFound )
    //		KMessageBox::sorry( 0, errMsg.arg( "Angle" ), i18n( "Could Not Set Value" ) );


    return qt;
}


QDate timeBox::createDate (bool */*ok*/)
{

    QString entry = text().trimmed();

    // if entry is an empty string or invalid date use current date

    QDate date = QDate::fromString(entry);

    if ( !date.isValid() ) {
        kDebug() << "Invalid date";
        showDate(QDate::currentDate());
        entry = text().trimmed();
        return QDate::currentDate();
    } else {
        return date;
    }
}

timeBox::~timeBox(){
}
