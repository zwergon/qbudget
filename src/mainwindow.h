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

#ifndef QBUDGET_MAINWINDOW_H_
#define QBUDGET_MAINWINDOW_H_

#include "ui_mainwindow.h"
#include "transaction_model.h"

class QSqlRelationalTableModel;

namespace QBudget {

class CMainWindow : public QMainWindow 
{
  Q_OBJECT
    
public:
    CMainWindow( QWidget* parent = 0 );
  
protected slots:

    void on_date_edit__dateChanged( const QDate& date );
    void on_next_button__clicked();
    void on_previous_button__clicked();
    void on_topic_action__triggered();
    void on_valide_action__triggered();
    void on_propose_action__triggered();
    void on_revert_action__triggered();

    void globalAmountComputedSlot( double );
    void globalTransactionComputedSlot( double );
    void transactionEditedSlot();

    void updateUI();
  
private:
  bool modified_;
  Ui::MainWindow ui_;
  CTransactionModel* transaction_model_;
  
};

}

#endif
