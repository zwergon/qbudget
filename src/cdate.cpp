#include "cdate.h"


namespace QBudget {

    CDate::CDate( const QDate& date ) :
            QDate( date.year(), date.month(), date.day() )
    {
        dateAdjusting();
    }

    CDate& CDate::operator++(){
        QDate date = addDays( 17 );
        setDate( date.year(), date.month(), date.day() );
        dateAdjusting();
        return *this;
    }

    void CDate::operator++(int){
        ++(*this);
    }

    CDate& CDate::operator--(){
        QDate date = addDays( -13 );
        setDate( date.year(), date.month(), date.day() );
        dateAdjusting();
        return *this;
    }

    void CDate::operator--(int){
        --(*this);
    }


    void CDate::dateAdjusting(){
        if ( day() < daysInMonth() / 2 ){
            setDate( year(), month(), 1 );
        }
        else {
            setDate( year(), month(), 15 );
        }
    }

}
