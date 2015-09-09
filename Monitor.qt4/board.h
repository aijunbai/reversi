#ifndef BOARD_H
#define BOARD_H

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

#include <qwidget.h>
#include <qcolor.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qtimer.h>
#include <qdatetime.h>
//Added by qt3to4:
#include <QPaintEvent>
#include <QMouseEvent>
#include <QTimerEvent>
#include "common.h"

#define CH_EMPTY 	(char)1
#define CH_BLACK 	(char)2
#define CH_WHITE 	(char)3
#define GRID_CH_SUB 	4

#define ST_ACTION 	1
#define ST_WAIT 	2

#define ACTION		(char)70
#define WAIT		(char)71
#define BOARD_DESCRIPTION (char)69

#define ONE_MORE_TIME	(char)72
#define TWO_TIMES_LEFT	(char)73
#define THREE_TIMES_LEFT (char)76

#define YOU_WIN 	(char)74
#define YOU_LOSE	(char)75
#define YOU_TIE 	(char)77

#define BLACK_WIN 	(char)81
#define WHITE_WIN 	(char)80

#define RL_BLACK 	(char)2
#define RL_WHITE 	(char)3
#define RL_MONITOR     	(char)4

#define MESSAGE_SIZE 	256

class Board: public QWidget{
Q_OBJECT
	private:
		int timer;

		static const QPen   white_pen;
		static const QPen   black_pen;
		static const QBrush white_brush;
		static const QBrush black_brush;
		static const QBrush back_brush;
		static const QPen   back_pen;

		char   layout[65];
		QString score;
		QString info;
		char   black_num;
		char   white_num;
		char   *message;
		int  stat;
		bool acted;
		char role;
	public:
		Board( QWidget *parent = 0, const char *name = 0 );
		~Board(){}
		char do_msg(char *, size_t);
	private:
		char get_color(int , int );
		void set_color(int, int, char);
		void wait_to_act();
		void act();
		void wait();
		void I_win();
		void I_lose();
		void We_tie();
		void Black_win();
		void White_win();
		void times_left(int t);

		void paintAll();
		void drawGrid(int , QPainter *);
		void drawBackground(QPainter *);
		void update(char*);
	public slots:
		virtual void paintEvent(QPaintEvent *);
		virtual void mousePressEvent(QMouseEvent *);
		virtual void timerEvent( QTimerEvent * );
		void exit();

	signals:
		void score_update(const QString&);
		void sayByebye();
		void say( const QString&);
};

#endif
