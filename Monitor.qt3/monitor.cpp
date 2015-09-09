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


#include "monitor.h"
#include <qlcdnumber.h>
#include <qfont.h>
#include <qlabel.h>
#include <qsocket.h>
#include "board.h"
#include "comunication.h"


Monitor::Monitor(char *host, QWidget *parent, const char *name)
	: QWidget(parent, name)
{			
	host_name = new char[strlen(host) + 1];
	strcpy(host_name, host);
	char *caption = new char[strlen(host_name) + strlen(TITLE) + 1];
	strcpy(caption, TITLE);
	strcat(caption, host_name);
	setCaption(caption);	
	delete[] caption;
	
	setMinimumSize( MONITOR_WIDTH, MONITOR_HEGHT);
	setMaximumSize( MONITOR_WIDTH, MONITOR_HEGHT);
	
	Board *board = new Board(this, "board");
	board->setGeometry(10, 10, BOARD_SIZE, BOARD_SIZE);
	
	QPushButton *quit = new QPushButton( "Quit", this, "quit" );
	quit->setGeometry( MONITOR_WIDTH-60, MONITOR_HEGHT-35, 50, 28 );
	quit->setFont( QFont( "Times", 12, QFont::Bold ) );
	QObject::connect( quit, SIGNAL(clicked()), board, SLOT(exit()) );
	
	QLCDNumber *score = new QLCDNumber( this, "score");
	score->setGeometry(20, MONITOR_HEGHT-35, 80, 30);
	score->setSegmentStyle( QLCDNumber::Filled );
	
	QLabel *info = new QLabel("Welcome !", this);
	info->setGeometry(120,  MONITOR_HEGHT-35, 220, 30);
	info->setFont( QFont( "Times", 10, QFont::Bold ));
	
	//TCPClient *client = new TCPClient(host);
	
	QObject::connect( board, SIGNAL(sayByebye()), qApp, SLOT(quit()));	
	QObject::connect( board, SIGNAL(score_update(const QString&)), score, SLOT(display(const QString&)));
	QObject::connect( board, SIGNAL(say(const QString&)), info, SLOT(setText(const QString&)));
			
	//Comunication *comunication = new Comunication(board, client);
	//comunication->start();
} 
