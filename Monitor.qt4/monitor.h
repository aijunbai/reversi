#ifndef MONITOR_H
#define MONITOR_H

/***************************************************************************
 *   Copyright (C) 2006 by Bai Aijun   *
 *   baj@mail.ustc.edu.cn,   baj_forever@126.com   *
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

/**
	@author Bai Aijun <baj@mail.ustc.edu.cn,   baj_forever@126.com>
 */


#include <qapplication.h>
#include <qobject.h>
#include <qwidget.h>
#include <qcolor.h>
#include <qpushbutton.h>
#include <qfont.h>

#include "common.h"

#define TITLE "Reversi Monitor - "

class Monitor : public QWidget
{
	char *host_name;	
	public:
		Monitor(char *host = 0, QWidget *parent = 0, const char *name = 0 );
		~Monitor(){
			if(host_name)	delete[] host_name;
		}
};

#endif
