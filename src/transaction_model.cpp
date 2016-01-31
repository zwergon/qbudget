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

#include <QAbstractTableModel>
#include <QSqlQuery>

#include "transaction.h"
#include "transaction_model.h"
#include "db_manager.h"


namespace QBudget {



    CTransactionModel::CTransactionModel(QObject *parent)
            : QAbstractTableModel(parent), date_( QDate::currentDate() )
    {
    }

    void CTransactionModel::next(){
        ++date_;
        select();
    }

    void CTransactionModel::previous(){
        --date_;
        select();
    }

    void CTransactionModel::setDate( const CDate& date ){
        date_ = date;
        select();
    }

    bool CTransactionModel::select(){

        beginResetModel();

        if ( !CDBManager::instance()->selectAllTransactions( date_, table_ ) ) {
            return false;
        }
        computeGlobalTransaction();
        computeGlobalAmount();

        endResetModel();

        return true;
    }



    Qt::ItemFlags CTransactionModel::flags(const QModelIndex &index) const
    {
        if (!index.isValid())
            return 0;

        if ( ( index.column() == TRANSACTION ) ||
        	 ( index.column() == COMMENT ) ){
            return Qt::ItemIsEditable | Qt::ItemIsEnabled;
        }

        return Qt::ItemIsEnabled;
    }

    bool CTransactionModel::setData(const QModelIndex &index, const QVariant &value, int ){
        if (!index.isValid())
            return false;

        switch( index.column() ){
        case TRANSACTION:
        {

            if ( !value.canConvert<double>() ){
                return false;
            }

            double transaction_amount = value.toDouble();


            QList<CTopic*> topics = table_.values();
            CTopic* topic = topics.at( index.row() );
            CTransaction& transaction = topic->transaction();

            if ( transaction_amount == transaction.amount() ){
                return false; // transaction unchanged.
            }

            topic->setAmount( topic->amount() - transaction.amount() + transaction_amount );
            transaction.setAmount( transaction_amount );
            transaction.update();

            computeGlobalAmount();
            computeGlobalTransaction();

            emit transactionEdited();

            return true;
        }
        case COMMENT:
        {

        	QList<CTopic*> topics = table_.values();
        	CTopic* topic = topics.at( index.row() );
        	CTransaction& transaction = topic->transaction();

        	transaction.setComment( value.toString() );
        	transaction.update();

        	emit transactionEdited();
        	return true;
        }
        default:
        	break;
        }

        return false;
    }

    QVariant CTransactionModel::data( const QModelIndex& index, int role ) const{

        if (!index.isValid())
            return QVariant();

        QList<CTopic*> topics = table_.values();
        CTopic* topic = topics.at( index.row() );
        CTransaction& transaction = topic->transaction();

        switch( role ){
        case Qt::ForegroundRole:
            {
                if ( index.column() == TRANSACTION ){
                    switch ( transaction.state() )
                    {
                    case CTransaction::NEW:
                    case CTransaction::MODIFIED:
                    case CTransaction::REMOVE:
                        return QVariant(QColor(Qt::blue));
                    case CTransaction::VALIDATED:
                    default:
                        if ( transaction.amount() < 0 ){
                            return QVariant(QColor(Qt::red));
                        }
                        else {
                            return QVariant(QColor(Qt::green));
                        }
                    }
                }
                else if ( index.column() == COMMENT ){
                	switch ( transaction.state() )
                	{
                	case CTransaction::NEW:
                	case CTransaction::MODIFIED:
                	case CTransaction::REMOVE:
                		return QVariant(QColor(Qt::blue));
                	default:
                		return QVariant();
                	}
                }

                return QVariant();
            }

        case Qt::DisplayRole:
        case Qt::EditRole:
            {

                switch( index.column() ){
                case RUBRIQUE:
                    return topic->name();
                case TRANSACTION:
                    {
                        if ( transaction.state() != CTransaction::NONE ) {
                            return transaction.amount();
                        }
                        return QVariant();
                    }
                case SOLDE:
                    return topic->amount();
                case COMMENT:
                	return transaction.comment();
                default:
                    return QVariant();
                }
            }
        default:
            return QVariant();
        }



    }

    QVariant CTransactionModel::headerData( int section, Qt::Orientation orientation, int role ) const{
        if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
            switch( section ){
            case RUBRIQUE:
                return "Rubrique";
            case TRANSACTION:
                return "Transaction";
            case SOLDE:
                return "Solde";
            case COMMENT:
            	return "Commentaire";
            default:
                return QVariant();
            }
        }

        return QVariant();
    }

    int CTransactionModel::rowCount( const QModelIndex& ) const {
        return table_.count();
    }

    int CTransactionModel::columnCount( const QModelIndex& ) const {
        return LAST_COLUMN;
    }

    void CTransactionModel::computeGlobalAmount(){

        QMapIterator<int, CTopic*> it_topic( table_ );
        double global_amount = 0.0;
        while( it_topic.hasNext() ){
            it_topic.next();
            CTopic* topic = it_topic.value();
            global_amount += topic->amount();
        }

        emit globalAmountComputed( global_amount );
    }

    void CTransactionModel::computeGlobalTransaction(){

        QMapIterator<int, CTopic*> it_topic( table_ );
        double global_transaction = 0.0;
        while( it_topic.hasNext() ){
            it_topic.next();
            CTransaction& transaction = it_topic.value()->transaction();
            if ( transaction.state() != CTransaction::NONE ){
                global_transaction += transaction.amount();
            }
        }

        emit globalTransactionComputed( global_transaction );
    }

    bool CTransactionModel::propose(){

        if ( date_.day() != 1 ){
            return false;
        }


        beginResetModel();
        bool modified = false;
        QMap<int, double> proposals;
        CDBManager::instance()->selectProposals( proposals );
        QMapIterator<int, double> itp( proposals );
        while( itp.hasNext() ){
        	itp.next();
            int topic_id = itp.key();
            double transaction_amount = itp.value();

            CTopic* topic = table_[topic_id];
            CTransaction& transaction = topic->transaction();
            if ( transaction.state() == CTransaction::NONE ){
                transaction.setAmount( transaction_amount );
                transaction.setState( CTransaction::NEW );
                topic->setAmount( topic->amount() + transaction_amount );
                modified = true;
            }
        }

        if ( modified ){
            computeGlobalAmount();
            computeGlobalTransaction();
        }

        endResetModel();



        return modified;
    }

    bool CTransactionModel::validate(){

        beginResetModel();
        QMapIterator<int, CTopic*> it_topic( table_ );
        while( it_topic.hasNext() ){
            it_topic.next();
            CTopic* topic = it_topic.value();
            CTransaction& transaction = topic->transaction();
            CDBManager::instance()->createTransaction( transaction );
        }
        endResetModel();

        return true;
    }



}
