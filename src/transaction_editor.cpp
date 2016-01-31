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

#include "transaction_editor.h"

#include "transaction_model.h"

#include <QLineEdit>

namespace QBudget {

CTransactionDelegate::CTransactionDelegate( QObject* parent ) :
	QItemDelegate(parent)
{
}

QWidget *CTransactionDelegate::createEditor(
		QWidget *parent,
		const QStyleOptionViewItem &/* option */,
		const QModelIndex &/* index */) const
{
     QLineEdit *editor = new QLineEdit(parent);
     return editor;
 }

void CTransactionDelegate::setEditorData(
		QWidget *editor,
		const QModelIndex &index ) const
{
	QString value = index.model()->data(index, Qt::EditRole).toString();

	QLineEdit *lineedit = static_cast<QLineEdit*>(editor);
	lineedit->setText(value);
}

void CTransactionDelegate::setModelData(
		QWidget *editor,
		QAbstractItemModel *model,
		const QModelIndex &index ) const
{
	QLineEdit *lineedit = static_cast<QLineEdit*>(editor);
	QString text_value = lineedit->text();

	switch( index.column() ){
	case CTransactionModel::TRANSACTION:
	{
		double value;
		if ( parseTransaction( text_value, value ) ){
			model->setData(index, value, Qt::EditRole);
		}
		break;
	}
	case CTransactionModel::COMMENT:
		model->setData( index, text_value, Qt::EditRole );
		break;
	default:
		break;
	}

}

void CTransactionDelegate::updateEditorGeometry(
		QWidget *editor,
		const QStyleOptionViewItem &option,
		const QModelIndex & /*index*/ ) const
{
	editor->setGeometry(option.rect);
}

/**
 * when parsing a transaction only addition and substraction are handled
 * only line on the form =a+b-c... are processed.
 */
bool CTransactionDelegate::parseTransaction(
		const QString& text,
		double& transaction ) const {

	QString to_parse = text.trimmed();


	QChar* token = new QChar[to_parse.length()];

	QList<double> operands;
	QList<QChar> operators;

	int index = 0;
	const QChar* data_to_parse = to_parse.constData();
	for( int i=0; i< to_parse.length(); i++ ){
        switch( data_to_parse[i].toLatin1()){
		case 61/*'='*/:
			break;
		case 43 /*'+'*/:
		case 45 /*'-'*/:
			operators.append(data_to_parse[i]);
			token[index] = '\0';
			operands.append ( QString(token).toDouble() );
			index = 0;
			break;

		case 48 /*0*/:
		case 49 /*1*/:
		case 50 /*2*/:
		case 51 /*3*/:
		case 52 /*4*/:
		case 53 /*5*/:
		case 54 /*6*/:
		case 55 /*7*/:
		case 56 /*8*/:
		case 57 /*9*/:
		case 44 /*,*/:
		case 46 /*.*/:
			token[index] = data_to_parse[i];
			index++;
			break;
		default:
			break;

		}
	}

	//get last token
	token[index] = '\0';
	operands.append ( QString(token).toDouble() );

	transaction = operands[0];
	for( int i = 0; i< operators.size(); i++ ){
		if ( operators.at(i) == '+'){
			transaction += operands.at(i+1);
		}
		else {
			transaction -= operands.at(i+1);
		}
	}


	delete [] token;

	return true;
}



}


