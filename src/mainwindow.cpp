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
#include <QDateTimeEdit>

#include "mainwindow.h"
#include "topic_dialog.h"
#include "cdate.h"
#include "transaction_editor.h"

namespace QBudget {

    CMainWindow::CMainWindow( QWidget* ) : modified_(false), transaction_model_( 0 )
    {
        ui_.setupUi( this );

        transaction_model_ = new CTransactionModel(this);
        ui_.transaction_view_->setModel( transaction_model_ );
        ui_.transaction_view_->setItemDelegate( new CTransactionDelegate( ui_.transaction_view_) );

        connect ( transaction_model_, SIGNAL(globalAmountComputed(double)),
                  this, SLOT(globalAmountComputedSlot(double)) );
        connect ( transaction_model_, SIGNAL(globalTransactionComputed(double)),
                  this, SLOT(globalTransactionComputedSlot(double)) );
        connect ( transaction_model_, SIGNAL(transactionEdited()),
                        this, SLOT(transactionEditedSlot()) );

        on_date_edit__dateChanged( QDate::currentDate() );
    }


    void CMainWindow::on_topic_action__triggered(){
        CTopicsDialog dlg;
        dlg.setModal( true );

        dlg.exec();
        transaction_model_->select();
    }



    void CMainWindow::on_propose_action__triggered(){
        modified_ = transaction_model_->propose();
        updateUI();
    }

    void CMainWindow::on_valide_action__triggered(){
        transaction_model_->validate();

        modified_ = false;
        updateUI();
    }

    void CMainWindow::updateUI(){
        if ( modified_ ){
            ui_.valide_action_->setEnabled(true);
        }
        else {
            ui_.valide_action_->setEnabled(false);
        }
    }



    void CMainWindow::on_next_button__clicked() {
        transaction_model_->next();
        ui_.date_edit_->blockSignals(true);
        ui_.date_edit_->setDate( transaction_model_->date() );
        ui_.date_edit_->blockSignals( false );

        modified_ = false;
        updateUI();
    }

    void CMainWindow::on_previous_button__clicked(){
        transaction_model_->previous();
        ui_.date_edit_->blockSignals(true);
        ui_.date_edit_->setDate( transaction_model_->date() );
        ui_.date_edit_->blockSignals( false );

        modified_ = false;
        updateUI();
    }

    void CMainWindow::on_revert_action__triggered(){
    	if ( 0 == transaction_model_ ){
    		return;
    	}

    	transaction_model_->setDate(  transaction_model_->date() );
    	if ( modified_ )
    		modified_ = transaction_model_->propose();

    	updateUI();

    }

    void CMainWindow::on_date_edit__dateChanged( const QDate& date ){

        if ( 0 == transaction_model_ ){
            return;
        }

        CDate  cdate( date );

        ui_.date_edit_->blockSignals(true);
        ui_.date_edit_->setDate( cdate );
        ui_.date_edit_->blockSignals( false );


        transaction_model_->setDate( cdate );

        modified_ = false;
        updateUI();
    }

    void CMainWindow::transactionEditedSlot(){
    	modified_ = true;
    	updateUI();
    }

    void CMainWindow::globalAmountComputedSlot( double global_amount ){
        ui_.global_amount_lineedit_->setText( QString::number( global_amount ) );
    }

    void CMainWindow::globalTransactionComputedSlot( double global_transaction ){

        if ( global_transaction == 0 ){
            ui_.global_transaction_lineedit_->setText( "" );
            return;
        }

        if ( global_transaction > 0 ){
            ui_.global_transaction_lineedit_->setStyleSheet("color: green;");
        }
        else if ( global_transaction < 0 ){
            ui_.global_transaction_lineedit_->setStyleSheet("color: red;");
        }

        ui_.global_transaction_lineedit_->setText(  QString::number( global_transaction ) );
    }

}
