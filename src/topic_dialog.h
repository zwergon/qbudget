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

#ifndef QBUDGET_TOPICS_DIALOG_
#define QBUDGET_TOPICS_DIALOG_

#include "ui_topic_dialog.h"

class QSqlTableModel;

namespace QBudget {

class CTopicsDialog : public QDialog {
  Q_OBJECT
    
public:
    CTopicsDialog( QWidget* parent = 0 );

protected slots:
	virtual void on_add_topic_button__clicked();
	virtual void on_remove_topic_button__clicked();

protected:
	int getTopicIdAtSelection() const;

  
private:
  Ui::TopicsDialog ui_;
  QSqlTableModel* topics_model_;

};

}

#endif
