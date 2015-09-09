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


#include "board.h"
//Added by qt3to4:
#include <QTimerEvent>
#include <QPaintEvent>
#include <QPixmap>
#include <QMouseEvent>

const QColor BLACK = QColor(0, 0, 0);
const QColor WHITE = QColor(255, 255, 255);
const QColor BACKGROUND = QColor(180, 133, 46);
const QColor BACKLINE =	QColor(200, 200, 200);

#define NEED_TO_SEND 	(char)255
#define NO_NEED_TO_SEND (char)254
#define FINISHED 	(char)253

const QPen   Board::white_pen(WHITE);
const QPen   Board::black_pen(BLACK);
const QBrush Board::white_brush(WHITE);
const QBrush Board::black_brush(BLACK);
const QBrush Board::back_brush(BACKGROUND);
const QPen   Board::back_pen(BACKLINE);

Board::Board(QWidget *parent, const char *name )
	: QWidget( parent, name )
{
	message = NULL;
	for(int i = 0; i < 65; i++)
		layout[i] = CH_EMPTY;
	black_num = 0;
	white_num = 0;
	timer = startTimer( 500 );		// 1/2 second timer events
}

inline char Board::get_color(int x, int y)
{
	return layout[x + 8*(y-1)];
}

void Board::set_color(int x, int y, char flag)
{
	assert(x > 0 && x < 9);
	assert(y > 0 && y < 9);
	layout[x + 8*(y-1)] = flag;
}

void Board::paintEvent( QPaintEvent * )
{
	paintAll();
}

inline void Board::paintAll()
{
	static QPixmap pix(rect().size());
	static QPainter p;

	p.begin(&pix);
	drawBackground(&p);
	for(int i = 1; i < 65; i++){
		drawGrid(i, &p);
	}
	p.end();
	bitBlt( this, rect().topLeft(), &pix );
	score.sprintf( "%2d:%2d", black_num, white_num);
	emit score_update(score);
}

inline void Board::drawBackground(QPainter *p)
{
	static bool first = true;
	static QPixmap bkg(rect().size());
	if(first == true){
		first = false;
		QPainter *pp = new QPainter(&bkg);
		pp->setBrush(back_brush);
		pp->setPen(back_pen);
		pp->drawRect(0, 0, FRAME_WIDTH*2 + GRID_SIZE*8, FRAME_WIDTH*2 + GRID_SIZE*8);
		for(int i = 0; i < 9; i++)
			pp->drawLine(FRAME_WIDTH, FRAME_WIDTH + GRID_SIZE*i, \
					FRAME_WIDTH + GRID_SIZE*8, FRAME_WIDTH + GRID_SIZE*i);
		for(int i = 0; i < 9; i++)
			pp->drawLine(FRAME_WIDTH + GRID_SIZE*i, FRAME_WIDTH, \
					FRAME_WIDTH + GRID_SIZE*i, FRAME_WIDTH + GRID_SIZE*8);
		delete pp;
	}
	p->drawPixmap(rect().topLeft(), bkg);
}

inline void Board::drawGrid(int pos, QPainter *p)
{
	int x = (pos%8 == 0)? 8: pos%8;
	int y = (pos%8 == 0)? pos/8: pos/8+1;
	int cord_x = (x-1)*GRID_SIZE + FRAME_WIDTH;
	int cord_y = (y-1)*GRID_SIZE + FRAME_WIDTH;

	if(get_color(x, y) == CH_BLACK){
		p->setBrush(black_brush);
		p->setPen(black_pen);
		p->drawEllipse(cord_x+GRID_CH_SUB, cord_y+GRID_CH_SUB, \
				GRID_SIZE-GRID_CH_SUB*2, GRID_SIZE-GRID_CH_SUB*2);
	}
	else if(get_color(x, y) == CH_WHITE){
		p->setBrush(white_brush);
		p->setPen(white_pen);
		p->drawEllipse(cord_x+GRID_CH_SUB, cord_y+GRID_CH_SUB, \
				GRID_SIZE-GRID_CH_SUB*2, GRID_SIZE-GRID_CH_SUB*2);
	}
}

void Board::mousePressEvent(QMouseEvent *e)
{
	if( stat == ST_ACTION && e->button() == Qt::LeftButton){
		int x = e->x();
		int y = e->y();
		x = (x-FRAME_WIDTH)/GRID_SIZE + 1;
		y = (y-FRAME_WIDTH)/GRID_SIZE + 1;
		if(message)
			sprintf(message, "%c", (char)( x + 8*(y-1)));
		printf(">>>> action sended: x %d, y %d\n", x, y);
		info.sprintf("OK, Good Luck!\n");
		emit say(info);
		acted = true;
		stat = ST_WAIT;
	}
}

void Board::update(char *msg)
{
	char *p = msg+1;
	black_num = white_num = 0;
	for(int i = 1; i < 65; i++){
		if((layout[i] = *p++) == CH_BLACK)
			black_num++;
		else if(layout[i] == CH_WHITE)
			white_num++;
	}
}

void Board::exit()
{
	killTimer(timer);
	emit sayByebye();
}

inline void Board::wait_to_act()
{
	stat = ST_ACTION;
	acted = false;	
	while(! acted){
		usleep(200000);
	}
}

void Board::act()
{
	info.sprintf("Now, It's your turn, ACTION!\n");
	emit say(info);
	wait_to_act();	
}

void Board::wait()
{
	puts(">>>> wait for one time");
	info.sprintf("Sorry, STOP for one time\n");
	emit say(info);
}

void Board::I_win()
{
	puts(">>>> I win");
	info.sprintf("OK, you are the winner!\n");
	emit say(info);
}

void Board::I_lose()
{
	puts(">>>> I lose");
	info.sprintf("So sorry that...\n");
	emit say(info);
}

void Board::We_tie()
{
	puts(">>> We tie");
	info.sprintf("You two tie at last!\n");
	emit say(info);
}

void Board::Black_win()
{
	puts(">>>> The BLACK win");
	info.sprintf("OK, the winner is BLACK!\n");
	emit say(info);
}

void Board::White_win()
{
	puts(">>>> The WHITE win");
	info.sprintf("OK, the winner is WHITE!\n");
	emit say(info);
}

void Board::times_left(int t)
{
	printf(">>>> %d times left\n", t);
	info.sprintf("Mistake, %d times left.\n", t);
	emit say(info);
	wait_to_act();
}

void Board::timerEvent( QTimerEvent * )
{
	repaint(FALSE);
}

char Board::do_msg(char *msg, size_t size)
{
	assert(strlen(msg) < size);
	char reply = 0;
	stat = ST_WAIT;
	message = msg;
	switch(*msg){
		case ACTION:
			reply = NEED_TO_SEND;
			act(); break;
		case WAIT:
			reply = NO_NEED_TO_SEND;
			wait(); break;
		case BOARD_DESCRIPTION:
			reply = NO_NEED_TO_SEND;
			update(msg); break;
		case ONE_MORE_TIME:
			reply = NEED_TO_SEND;
			times_left(1); break;
		case TWO_TIMES_LEFT:
			reply = NEED_TO_SEND;
			times_left(2); break;
		case THREE_TIMES_LEFT:
			reply = NEED_TO_SEND;
			times_left(3); break;
		case RL_BLACK:
			reply = NO_NEED_TO_SEND; puts(">>>> I'm the Black");
			role = RL_BLACK; break;
		case RL_WHITE:
			reply = NO_NEED_TO_SEND; puts(">>>> I'm the White");
			role = RL_WHITE; break;
		case RL_MONITOR:
			reply = NO_NEED_TO_SEND; puts(">>>> I'm a monitor");
			role = RL_MONITOR; break;
		case YOU_WIN:
			reply = FINISHED;
			I_win(); break;
		case YOU_LOSE:
			reply = FINISHED;
			I_lose(); break;
		case YOU_TIE:
			reply = FINISHED;
			We_tie(); break;
		case BLACK_WIN:
			reply = FINISHED;
			Black_win(); break;
		case WHITE_WIN:
			reply = FINISHED;
			White_win(); break;
	}
	return reply;
}


