/*
 * CDBManager.cpp
 *
 *  Created on: 17 sept. 2011
 *      Author: jef
 */

#include "db_manager.h"

#include <QStringList>
#include <QDir>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>


namespace QBudget {

CDBManager* CDBManager::instance_ = 0;
const char* CDBManager::db_name_ = "qbudget.sqlite";

const char* CDBManager::nameTopicTable(){
	return "topics";
}
const char* CDBManager::nametransactionTable(){
	return "transactions";
}

CDBManager* CDBManager::instance(){
	if ( 0 == instance_ ){
		instance_ = new CDBManager;
	}

	return instance_;
}

CDBManager::CDBManager() {
}

CDBManager::~CDBManager() {
}

bool CDBManager::selectProposals( QMap<int, double>& proposals ){
	QString query = QString("SELECT topics.id, topics.amount FROM topics;" );
	QSqlQuery sql_query(query);
	while (sql_query.next()) {
		int topic_id = sql_query.value(0).toInt();
		double transaction_amount = sql_query.value(1).toDouble() / 12.;

		if ( transaction_amount > 0 ){
			proposals[topic_id] = transaction_amount;
		}
	}

	return true;
}


bool CDBManager::selectTransactions( const CDate& date, QMap<int, CTransaction>& transactions ) {
	QString query = QString("SELECT transactions.topic_id, transactions.amount, transactions.id, transactions.comment "
			"FROM transactions "
			"WHERE ( transactions.the_date == '%1' ); " ).arg( date.toString( "yyyy-MM-dd" ));

	QSqlQuery sql_query(query);
	while (sql_query.next()) {
		int topic_id = sql_query.value(0).toInt();
		double amount = sql_query.value(1).toDouble();
		int id = sql_query.value(2).toInt();
		QString comment = sql_query.value(3).toString();

		CTransaction& transaction = transactions[topic_id];
		transaction.setTopicId( topic_id );
		transaction.setDate( date );
		transaction.setComment( comment );
		transaction.setId( id );
		transaction.setAmount( amount  );
		transaction.setState( CTransaction::VALIDATED );
	}

	return true;
}



bool CDBManager::selectAllTransactions(  const CDate& date, QMap<int, CTopic*>& table ) {

	QMap<int, CTransaction> transactions;
	selectTransactions( date, transactions );

	QString query = QString("SELECT topics.id, topics.title, SUM( transactions.amount ) "
			"FROM topics LEFT JOIN transactions "
			"ON ( topics.id = transactions.topic_id ) "
			"AND ( transactions.the_date <= '%1' ) "
			"GROUP BY topics.id "
			"ORDER BY topics.title; " ).arg( date.toString( "yyyy-MM-dd" ));


	QSet<int> id_to_remove = table.keys().toSet();
	QSqlQuery sql_query(query);
	while (sql_query.next()) {
		int topic_id = sql_query.value(0).toInt();

		id_to_remove.remove( topic_id );

		CTopic* topic = 0;
		if ( !table.contains( topic_id ) ){
			topic = new CTopic(topic_id);
			table[topic_id] =  topic;
		}
		else {
			topic = table[topic_id];
		}

		topic->setName( sql_query.value(1).toString() );
		topic->setAmount( sql_query.value(2).toDouble());

		CTransaction& transaction = topic->transaction();


		if ( transactions.contains( topic_id) ){
			transaction = transactions[topic_id];
		}
		else {
			transaction.setDate( date );
			transaction.setState( CTransaction::NONE );
			transaction.setComment( QString() );
			transaction.setAmount( 0. );
		}


	}

	QSetIterator<int> it_remove( id_to_remove );
	while( it_remove.hasNext() ){
		int topic_id = it_remove.next();
		CTopic* topic = table[topic_id];
		delete topic;
		table.remove( topic_id );
	}



	return true;
}
void CDBManager::createTopic( const QString& name, double amount ){
	QSqlQuery query;
	QString sql_string = QString("insert into topics(title, amount) values('%1', %2)").arg(name).arg(amount);
	query.exec(sql_string);
}

void CDBManager::deleteTopic( int topic_id ){

	QString sql_string;
	QSqlQuery query;
	//remove all transaction dependant from topic
	sql_string = QString("delete from %1 where topic_id=%2").arg(nametransactionTable()).arg(topic_id);
	query.exec(sql_string);

	sql_string = QString("delete from %1 where id=%2").arg(nameTopicTable()).arg(topic_id);
	query.exec(sql_string);
}

void CDBManager::createTransaction( CTransaction& transaction ){
	if ( ( transaction.state() == CTransaction::VALIDATED ) ||
		 ( transaction.state() == CTransaction::NONE ) ){
		return;
	}



	QString sql_string;
	switch(  transaction.state() ){
	case CTransaction::NEW:
		sql_string = QString(
				"insert into %1(topic_id, the_date, amount, comment) values ( %3, \'%4\', %5, \'%6\');")
				.arg( nametransactionTable() )
				.arg( transaction.topicId() )
				.arg( transaction.date().toString( "yyyy-MM-dd" ) )
				.arg( transaction.amount() )
				.arg( transaction.comment().left(255) );

		break;
	case CTransaction::MODIFIED:
		sql_string = QString(
				"update %1 set amount=%2, comment=\'%3\' where id=%4;")
				.arg( nametransactionTable() )
				.arg( transaction.amount() )
				.arg( transaction.comment() )
				.arg( transaction.id() );
		break;
	case CTransaction::REMOVE:
		sql_string = QString(
				"delete from %1 where id=%2;")
				.arg( nametransactionTable() )
				.arg( transaction.id() );
		break;

	default:
		break;
	}

	qDebug() << sql_string ;

	if ( sql_string.isEmpty() ){
		return;
	}

	QSqlQuery query;
	if ( query.exec( sql_string ) ){
		switch(  transaction.state() ){
		case CTransaction::NEW:
			transaction.setId( query.lastInsertId().toInt());
			/*no break*/
		case CTransaction::MODIFIED:
			transaction.setState( CTransaction::VALIDATED );
			break;

		case CTransaction::REMOVE:
		default:
			transaction.setAmount( 0 );
			transaction.setState( CTransaction::NONE );
			break;
		}

	}

}


bool CDBManager::openDB(){
	database_ = QSqlDatabase::addDatabase("QSQLITE");
	QString path(QDir::home().path());
	path.append(QDir::separator()).append(db_name_);
	path = QDir::toNativeSeparators(path);
	database_.setDatabaseName(path);
	if (!database_.open()) {
		return false;
	}

	//To ensure data integrity with foreign keys.
	QSqlQuery query;
	query.exec("PRAGMA foreign_keys = ON;");

	//no tables need to create them
	if ( database_.tables().isEmpty() ){
		if ( !createTables() ) {
			return false;
		}
	}

	return true;
}

/**
 * close database
 */
void CDBManager::closeDB(){
	if ( database_.isOpen() ){
		database_.close();
	}
	/*
	 * close connection by specifying a dummy QSqlDatabase
	 * else removeDatabase complains at exit.
	 */
	database_ = QSqlDatabase();
}

bool CDBManager::createTables(){
	createTopicTable();
	createTransactionTable();
	return true;
}

int CDBManager::getTransactionMaxId() const {
	int max_id = -1;
	QSqlQuery query;
	query.exec( "SELECT MAX(transactions.id) FROM transactions; " );
	if ( query.next() ){
		max_id = query.value(0).toInt();
	}

	return max_id+1;
}


bool CDBManager::createTopicTable(){
	QSqlQuery query;
	QString sql_create_string = QString(
			"create table %1 ("
			"id integer primary key, "
			"title varchar(20),"
			"amount float) " ).arg( nameTopicTable() );
	query.exec( sql_create_string );
	query.exec("insert into topics(title, amount) values('Impots'), 4500");
	query.exec("insert into topics(title, amount) values('Assurance', 250 )");
	query.exec("insert into topics(title, amount) values('Ecole', 2500)");
	query.exec("insert into topics(title, amount) values('Abonnements', 600)");

	return true;
}

bool CDBManager::createTransactionTable(){
	QSqlQuery query;
	QString sql_create_string = QString(
			"create table %1 ("
			"id integer primary key, "
			"topic_id integer not null,"
			"the_date date,"
			"amount float,"
			"comment char(256),"
			"FOREIGN KEY(topic_id) REFERENCES topics(id) )").arg( nametransactionTable() );
	query.exec( sql_create_string );
	query.exec("insert into transactions(topic_id, the_date, amount) values( 1, '2007-10-01', 200.)");
	query.exec("insert into transactions(topic_id, the_date, amount) values( 1, '2007-10-15', 340.)");
	query.exec("insert into transactions(topic_id, the_date, amount) values( 1, '2007-11-01', -134.)");
	query.exec("insert into transactions(topic_id, the_date, amount) values( 2, '2007-10-01', 222.)");

	return true;
}



}
