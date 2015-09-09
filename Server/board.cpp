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

#define CH_EMPTY (char)1
#define CH_BLACK (char)2
#define CH_WHITE (char)3
#define BOARD_DESCRIPTION (char)69

static const unsigned char possible_dir[][8] = {
	{ 0x83, 0x8F, 0x8F, 0x8F, 0x8F, 0x8F, 0x8F, 0x0E},
	{ 0xE3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E},
	{ 0xE3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E},
	{ 0xE3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E},
	{ 0xE3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E},
	{ 0xE3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E},
	{ 0xE3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E},
	{ 0xE0, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0x38}
};
/*
 ● ○ ○ ● ● ● 2 _
 ● ○ ○ ○ ● ● 2 1
 ● ○ ○ ○ ○ ● ○ ○
 ● ○ ● ● ● ○ ○ ○
 ● ● ● ● ○ ○ ○ ○
 ● ● ● ○ ○ ○ 1 ○
 ● 2 ● ○ ○ ● 3 _
 _ ● ● ● 1 3 ● _

*/

//#define DEBUG
#ifdef DEBUG
char board_st[][8] = {
	{ 2, 3, 3, 2, 2, 2, 1, 1},
	{ 2, 3, 3, 3, 2, 2, 1, 1},
	{ 2, 3, 3, 3, 3, 2, 3, 3},
	{ 2, 3, 2, 2, 2, 3, 3, 3},
	{ 2, 2, 2, 2, 3, 3, 3, 3},
	{ 2, 2, 2, 3, 3, 3, 1, 3},
	{ 2, 1, 2, 3, 3, 2, 1, 1},
	{ 1, 2, 2, 2, 1, 1, 2, 1},
};
#endif

Board::Board()
{
	init();
}

void Board::init()
{
	for(int i=0; i < 66; i++)
		layout[i] = CH_EMPTY;

	layout[0] = BOARD_DESCRIPTION;
	layout[65] = '\0';		///this two will never be changed again

#ifndef DEBUG
	set_color(4, 4, CH_WHITE);
	set_color(5, 4, CH_BLACK);
	set_color(4, 5, CH_BLACK);
	set_color(5, 5, CH_WHITE);
#else
	for(int i = 0; i < 8; i++)
		for(int  j= 0; j < 8; j ++)
			layout[(j + 1) + 8 * i ] = board_st[i][j];
#endif
}

inline char Board::get_color(int x, int y)
{
	return layout[x + 8*(y-1)];
}

inline void Board::set_color(int x, int y, char flag)
{
	layout[x + 8*(y-1)] = flag;
}

bool Board::able_to_put(char color)
{
	for(int i = 1; i < 65; i++)
		if( can_put(i, color))
			return true;
	return false;
}

inline bool Board::can_put(int pos, char color)
{
	if( pos < 1 || pos > 64)
		return false;
	return can_put( (pos%8 == 0)? 8: pos%8, (pos%8 == 0)? pos/8: pos/8 + 1, color);
}

inline bool Board::can_put(int x, int y, char color)
{
	if(x < 1 || x > 8 || y < 1 || y > 8 || get_color(x, y) != CH_EMPTY)
		return false;

	unsigned char dir = possible_dir[y-1][x-1];
	unsigned char ptr;
	while( dir){
		if( can_put(x, y, color, dir))
			return true;
		ptr = 0x80;
		while( !(ptr & dir))
			ptr >>= 1;
		dir = ~(~dir | ptr);
	}
	return false;
}

inline bool Board::can_put(int x, int y, char color, unsigned char dir)
{
	unsigned char ptr = 0x80;
	while( !(ptr & dir))
		ptr >>= 1;
	if( stretch(x, y, ptr, color))
		return true;
	return false;
}

inline void Board::next_pos(int &x, int &y, unsigned char dir)
{
	switch(dir){
		case 0x80: x++;      break;
		case 0x40: x++; y--; break;
		case 0x20:      y--; break;
		case 0x10: x--; y--; break;
		case 0x08: x--;      break;
		case 0x04: x--; y++; break;
		case 0x02:      y++; break;
		case 0x01: x++; y++; break;
	}
}

inline int Board::stretch(int x, int y, unsigned char dir, char color)
{
	next_pos(x, y, dir);
	int i = 0;
	while( x > 0 && x < 9 && y > 0 && y < 9 && get_color(x, y) == opposite(color)){
		next_pos(x, y, dir);
		i++;
	}
	if( x < 1 || x > 8 || y < 1 || y > 8)
		return 0;
	else if( get_color(x, y) == color)
		return i;
	else return 0;
}

inline char Board::opposite(char color)
{
	return (color == CH_BLACK)? CH_WHITE: (color == CH_WHITE)? CH_BLACK: CH_EMPTY;
}

bool Board::update(int color, char *msg)
{
	int pos = (int)*msg;
	if( pos < 1 || pos > 64)
		return false;
	printf(">>>> %s action: x %d, y %d, pos %d\n", (color == CH_BLACK)? "Black": "White",\
			(pos%8 == 0)? 8: pos%8, (pos%8 == 0)? pos/8: pos/8 + 1, pos);
	if( !can_put( pos, color))
		return false;
	else
		put( pos, color);
	show_board(0);
	return true;
}

void Board::put( int pos, char color)
{
	int x = (pos%8 == 0)? 8: pos%8;
	int y = (pos%8 == 0)? pos/8: pos/8 + 1;
	unsigned char dir = possible_dir[y-1][x-1];
	unsigned char ptr;
	set_color(x, y, color);
	while( dir){
		update_dir(x, y, dir, color);
		ptr = 0x80;
		while( !(ptr & dir))
			ptr >>= 1;
		dir = ~(~dir | ptr);
	}

}

inline void Board::update_dir(int x, int y, unsigned char dir, char color)
{
	unsigned char ptr = 0x80;
	while( !(ptr & dir))
		ptr >>= 1;
	int i = stretch(x, y, ptr, color);
	for(; i > 0; i--){
		next_pos(x, y, ptr);
		set_color(x, y, color);
	}
}

char Board::winner()
{
	static bool first = true;
	static char winner;
	if(first == true){
		first = false;
		int black_num = 0;
		int white_num = 0;
		for(int i = 1; i < 65; i++){
			if(layout[i] == CH_BLACK)
				black_num++;
			else if(layout[i] == CH_WHITE)
				white_num++;
		}
		if( black_num > white_num)
			winner = CH_BLACK;
		else if( white_num > black_num)
			winner = CH_WHITE;
		else    winner = '\0';
	}
	return winner;	
}

inline void Board::show_board(int depth)
{
	int black_num = 0, white_num = 0;
	for(int i = 0; i < 8; i++){
		for(int k = 0; k < depth; k++)
			putchar(32);
		for(int j = 0; j < 8; j++){
			switch(get_color(j+1, i+1)){
				case CH_EMPTY: putchar(' '); putchar('_');break;
				case CH_BLACK: putchar(' '); printf("x"); black_num++; break;
				case CH_WHITE: putchar(' '); printf("o"); white_num++; break;
			}
		}
		putchar('\n');
	}
	printf("black_num %d, white_num %d\n", black_num, white_num);
	putchar('\n');
}

