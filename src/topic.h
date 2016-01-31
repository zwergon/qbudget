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
#ifndef TOPIC_H_H
#define TOPIC_H_H

#include <QString>
#include <QHash>

#include "transaction.h"


namespace QBudget {

    class CTransaction;

    class CTopic {
    public:
        enum TransactionState {
            NONE,
            NEW,
            MODIFIED,
            VALIDATED
        };

    public:
        CTopic( int topic_id  );

        int topic_id() const;

        void setName( const QString& name );
        const QString& name() const;

        void setAmount( double amount );

        CTransaction& transaction();
        double amount() const;
        double proposal() const;

    private:
        int     topic_id_;
        QString name_;

        double proposal_;
        double amount_;

        CTransaction transaction_;

    };

    inline int CTopic::topic_id() const{
        return topic_id_;
    }

    inline void CTopic::setName( const QString& name ){
    	name_ = name;
    }

    inline const QString& CTopic::name() const{
        return name_;
    }

    inline double CTopic::amount() const {
        return amount_;
    }
    inline double  CTopic::proposal() const{
        return proposal_;
    }

    inline void CTopic::setAmount( double amount ){
        amount_ = amount;
    }

    inline CTransaction& CTopic::transaction(){
        return transaction_;
    }

    inline uint qHash ( const CTopic& topic ){
        return ::qHash( topic.topic_id() );
    }

    inline bool operator==( const CTopic& t1, const CTopic& t2 ){
        return t1.topic_id() == t2.topic_id();
    }

    inline bool operator<( const CTopic& t1, const CTopic& t2 ){
        return t1.name() < t2.name();
    }
}

#endif // TRANSACTION_H
