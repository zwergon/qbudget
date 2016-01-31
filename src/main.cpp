/***************************************************************************
 *   Copyright (C) 2007 by Jean-François Lecomte,,,,   *
 *   jflecomt@free.fr   *
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


#include <QApplication>
#include <QMessageBox>
#include <QtSql>

#include "db_manager.h"

#include "mainwindow.h"

using namespace QBudget;


int main(int argc, char *argv[])
{
  Q_INIT_RESOURCE(application);
  QApplication app(argc, argv);

  if ( !CDBManager::instance()->openDB() ){
	  QMessageBox::critical(0, qApp->tr("Cannot open database"),
			  qApp->tr("Unable to establish a database connection.\n"
					  "This example needs SQLite support. Please read "
					  "the Qt SQL driver documentation for information how "
					  "to build it.\n\n"
					  "Click Cancel to exit."), QMessageBox::Cancel);
	  return -1;
  }


  CMainWindow mainwindow;
  mainwindow.show();
  int stat = app.exec();

  CDBManager::instance()->closeDB();

  return stat;
}

