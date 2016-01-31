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
#include "transaction.h"
#include <math.h>

namespace QBudget {

	CTransaction::CTransaction() :
			state_( NONE ),
			id_(-1),
			topic_id_( -1 ),
			date_( QDate() ),
			amount_( 0.0 )
	{
	}

    CTransaction::CTransaction( int topic_id ) :
				  state_( NONE ),
        		  id_(-1),
        		  topic_id_( topic_id ),
        		  date_( QDate() ),
        		  amount_( 0.0 )
    {
    }

    void CTransaction::update(){

    	if (( amount_ == 0 ) && comment_.isEmpty() ){
    		state_ = CTransaction::REMOVE;
    		return;
    	}
    	switch( state_ ){
    	case CTransaction::NONE:
    		if ( ( fabs(amount_) > 0 ) || !comment_.isEmpty() ){
    			state_ = CTransaction::NEW;
    		}
    		break;
    	case CTransaction::VALIDATED:
    		if ( fabs(amount_) > 0 || !comment_.isEmpty() ){
    			state_ = CTransaction::MODIFIED;
    		}
    		break;
    	default:
    		break;
    	}
    }

}

