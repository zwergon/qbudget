/***************************************************************************
 *   Copyright (C) 2007 by Jean-François Lecomte <jflecomt@free.fr>        *
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

#include <QtSql>
#include <QMenu>
#include <QMessageBox>

#include "db_manager.h"
#include "topic_dialog.h"

namespace QBudget {

    CTopicsDialog::CTopicsDialog( QWidget* parent ) : QDialog( parent ) {
        ui_.setupUi( this );

        topics_model_ = new QSqlTableModel(this);
        topics_model_->setTable( CDBManager::nameTopicTable() );

        topics_model_->setEditStrategy(QSqlTableModel::OnFieldChange);
        topics_model_->setHeaderData(1, Qt::Horizontal, tr("Rubriques"));
        topics_model_->setHeaderData(2, Qt::Horizontal, tr("Total"));
        topics_model_->setSort( 1, Qt::AscendingOrder );

        topics_model_->select();

        ui_.topics_view_->setModel( topics_model_ );
        ui_.topics_view_->hideColumn( 0 ); //don't show id

    }


    int CTopicsDialog::getTopicIdAtSelection() const {

        //retrieve selected row
        QModelIndexList indexes = ui_.topics_view_->selectionModel()->selectedIndexes();
        if ( indexes.count() == 0 ) {
            return -1;
        }
        int row = indexes[0].row();

        //retrieve primaryKey of record at this row
        QModelIndex primaryKeyIndex = topics_model_->index( row, 0);
        return topics_model_->data( primaryKeyIndex ).toInt();
    }

    void CTopicsDialog::on_add_topic_button__clicked(){
    	CDBManager::instance()->createTopic();
    	topics_model_->select();
    }

    void CTopicsDialog::on_remove_topic_button__clicked(){
    	int topic_id = getTopicIdAtSelection();
    	if ( topic_id == -1 ){
    		return;
    	}

    	if ( QMessageBox::question( this, "remove one topic",
    			"Are you sure you want to remove this topic"
    			"and all transactions associated to it ?", QMessageBox::Ok|QMessageBox::Cancel) == QMessageBox::Ok ){
    		CDBManager::instance()->deleteTopic( topic_id );
    		topics_model_->select();
    	}
    }

}
