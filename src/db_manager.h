/*
 * CDBManager.h
 *
 *  Created on: 17 sept. 2011
 *      Author: jef
 */

#ifndef CDBMANAGER_H_
#define CDBMANAGER_H_

#include <QMap>
#include <qsqldatabase.h>

#include "transaction.h"
#include "topic.h"
#include "cdate.h"

namespace QBudget {

class CDBManager {
public:
	static CDBManager* instance();
	static const char* nameTopicTable();
	static const char* nametransactionTable();

	bool openDB();
	void closeDB();


	bool selectProposals( QMap<int, double>& );
	bool selectTransactions( const CDate& , QMap<int, CTransaction>&  );
	bool selectAllTransactions( const CDate& , QMap<int, CTopic*>&  );
	void createTransaction( CTransaction&  );
	void createTopic( const QString& name = "Unknown", double amount = 0 );
	void deleteTopic( int topic_id );

public:
	~CDBManager();

private:
	CDBManager();

	bool createTables();
	bool createTopicTable();
	bool createTransactionTable();

	int getTransactionMaxId() const;


private:
	static CDBManager* instance_;
	static const char* db_name_;

private:
	QSqlDatabase database_;
};

}

#endif /* CDBMANAGER_H_ */
