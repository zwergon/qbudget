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

#ifndef QBUDGET_TRANSACTION_MODEL_
#define QBUDGET_TRANSACTION_MODEL_

#include <QtSql>
#include <QMap>

#include "cdate.h"
#include "topic.h"
#include "transaction.h"

namespace QBudget {

    class CTransactionModel : public QAbstractTableModel {
        Q_OBJECT

    public:
        enum ColumnType {
            RUBRIQUE,
            TRANSACTION,
            SOLDE,
            COMMENT,
            LAST_COLUMN
        };

    public:
        CTransactionModel(QObject *parent = 0);

        const CDate& date() const;
        void setDate( const CDate& date );

        void next();
        void previous();

        bool select();


        Qt::ItemFlags flags(const QModelIndex &index) const;
        QVariant data( const QModelIndex& index, int role ) const;
        bool setData( const QModelIndex& index, const QVariant& value, int role  );
        QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
        int rowCount( const QModelIndex& parent ) const;
        int columnCount( const QModelIndex& parent ) const;

    public slots:
        bool propose();
        bool validate();

    signals:
		void transactionEdited();

    protected:
        bool selectTransactions();
        bool selectAllTransactions();

    signals:
        void globalAmountComputed( double global_amount );
        void globalTransactionComputed( double global_amount );

    private:
        void computeGlobalAmount();
        void computeGlobalTransaction();

    private:
        CDate date_;
        QMap<int, CTopic*> table_;

    };

    inline const CDate& CTransactionModel::date() const {
        return date_;
    }


}

#endif
