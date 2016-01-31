/***************************************************************************
 *   Copyright (C) 2007 by Jean-François Lecomte <jef.lecomte@gmail.com>   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "cdate.h"

namespace QBudget {

class CTransaction {
public:
    enum TransactionState {
        NONE,
        NEW,
        REMOVE,
        MODIFIED,
        VALIDATED
    };

public:
    CTransaction();
    CTransaction( int topic_id );

    bool   valid() const;
    TransactionState state() const;
    void   setState( TransactionState state );

    void setId( int id );
    int id() const;

    int topicId() const;
    void setTopicId( int id );

    void setDate( const CDate& date );
    const CDate& date() const;

    void setComment( const QString& comment );
    const QString& comment() const;

    void setAmount( double amount );
    double amount() const;

    void update();

private:
    TransactionState state_;
    int    id_;
    int    topic_id_;
    CDate  date_;
    double amount_;
    QString comment_;

};

inline void CTransaction::setState( TransactionState state ){
   state_ = state;
}

inline bool CTransaction::valid() const{
    return state_ == VALIDATED;
}

inline CTransaction::TransactionState CTransaction::state() const {
    return state_;
}

inline int CTransaction::topicId() const {
    return topic_id_;
}

inline void CTransaction::setTopicId( int id ){
	topic_id_ = id;
}

inline void CTransaction::setId( int id ) {
    id_ = id;
}

inline int CTransaction::id() const {
    return id_;
}

inline void CTransaction::setDate( const CDate& date ){
    date_ = date;
}

inline const CDate& CTransaction::date() const{
    return date_;
}

inline void CTransaction::setAmount( double amount ){
    amount_ = amount;
}

inline double CTransaction::amount() const{
    return amount_;
}

inline void CTransaction::setComment( const QString& comment ){
	comment_ = comment;
}

inline const QString& CTransaction::comment() const {
	return comment_;
}



}

#endif // TRANSACTION_H
