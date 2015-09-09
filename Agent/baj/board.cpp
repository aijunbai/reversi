/***************************************************************************
 *   Copyright (C) 2006-2007 by Bai Aijun   *
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
#include "hash.h"

#define NO_CAN_PUT  0x00
#define ME_CAN_PUT  0x01
#define HE_CAN_PUT  0x02

const unsigned char Board::possible_dir[][8] = {
	{ 0x83, 0x8F, 0x8F, 0x8F, 0x8F, 0x8F, 0x8F, 0x0E},
	{ 0xE3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E},
	{ 0xE3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E},
	{ 0xE3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E},
	{ 0xE3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E},
	{ 0xE3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E},
	{ 0xE3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E},
	{ 0xE0, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0x38}
};

void Board::update(char *msg)
{
	char *p = msg+1;
	for(int i = 1; i < 65; ++i){
		layout[i] = *p++;
	}
	update_configure_all();
}

char Board::get_color(int pos)
{
	return layout[pos];
}

inline char Board::get_color(int x, int y)
{
	return layout[pp(x, y)];
}

inline void Board::set_color(int pos, char color)
{
	hash->update_both(pos, layout[pos], color);
	layout[pos] = color;
#if 0
	hash->check_update(layout);
#endif
}

inline void Board::set_color(int x, int y, char color)
{
	hash->update_both(pp(x, y), layout[pp(x, y)], color);
	layout[pp(x, y)] = color;
#if 0
	hash->check_update(layout);
#endif
}

int Board::able_to_put(char &color)
{
	return (color == role)? comp_mobility: oppo_mobility;
}

bool Board::can_put(int pos, char &color)
{
	return mobility[pos] & ((color == role)? ME_CAN_PUT: HE_CAN_PUT);
}

int Board::can_put_corner(char &color)
{
	return (can_put(1, color)? 1: 0) + 
			(can_put(8, color)? 1: 0) +
			(can_put(57, color)? 1: 0) +
			(can_put(64, color)? 1: 0);			 
}

/**
	IN ASSERTION:	!(x < 1 || x > 8 || y < 1 || y > 8)
			get_color(x, y) != CH_EMPTY
*/
inline bool Board::can_put(int x, int y, char color)
{
	static unsigned char dir;
	register unsigned char ptr = 0x80;
	dir = possible_dir[y-1][x-1];
	for(;;){
		while( ptr && !(ptr & dir))
			ptr >>= 1;
		if( !ptr)
			break;
		if( stretch(x, y, ptr, color))
			return true;
		ptr >>= 1;
	}
	return false;
}

inline void Board::next_pos(int &x, int &y, unsigned char ptr)
{
	switch(ptr){
		case 0x80: ++x;      return;
		case 0x40: ++x; --y; return;
		case 0x20:      --y; return;
		case 0x10: --x; --y; return;
		case 0x08: --x;      return;
		case 0x04: --x; ++y; return;
		case 0x02:      ++y; return;
		case 0x01: ++x; ++y; return;
	}
}

inline int Board::stretch(int x, int y, unsigned char ptr, char color)
{
	static int n;
	static int opp_color;
	n = 0;
	opp_color = opposite(color);
	next_pos(x, y, ptr);
	while( x > 0 && x < 9 && y > 0 && y < 9 && get_color(x, y) == opp_color){
		next_pos(x, y, ptr);
		++n;
	}
	if( x < 1 || x > 8 || y < 1 || y > 8)
		return 0;
	else if( get_color(x, y) == color)
		return n;
	else 
		return 0;
}

bool Board::adjacent(int x, int y, char color)
{
	static unsigned char dir;
	static int tx, ty;
	register unsigned char ptr = 0x80;
	dir = possible_dir[y-1][x-1];
	tx = x, ty = y;
	for(;;){
		x = tx, y = ty;
		while( ptr && !(ptr & dir))
			ptr >>= 1;
		if( !ptr)
			break;
		next_pos(x, y, ptr);
		if(get_color(x, y) == color)
			return true;
		ptr >>= 1;
	}
	return false;
}

void Board::put( int pos, char color)
{
	static unsigned char dir;
	register unsigned char ptr = 0x80;
	dir = possible_dir[yy[pos]-1][xx[pos]-1];
	set_color(pos, color);
	for(;;){
		while( ptr && !(ptr & dir))
			ptr >>= 1;
		if( !ptr)
			break;
		update_dir(xx[pos], yy[pos], ptr, color);
		ptr >>= 1;
	}
	update_configure_all();
}

inline void Board::update_configure_all()
{
	static char tmp;
	comp_mobility = oppo_mobility = 0;
	comp_counts   = oppo_counts   = 0;
	for(register int i = 1; i < 65; ++i){
		mobility[i] = NO_CAN_PUT;
		if( (tmp = get_color(i)) == CH_EMPTY){
			if(can_put(xx[i], yy[i], role)){
				++comp_mobility ;
				mobility[i] |= ME_CAN_PUT;			
			}
			if(can_put(xx[i], yy[i], opp_role)){
				++oppo_mobility;
				mobility[i] |= HE_CAN_PUT;
			}
		}
		else if(tmp == role)
			++comp_counts ;
		else
			++oppo_counts ;
			
	}
}

inline void Board::update_dir(int x, int y, unsigned char ptr, char color)
{	
	for(register int i = stretch(x, y, ptr, color); i > 0; i--){
		next_pos(x, y, ptr);
		set_color(x, y, color);
	}
}

void Board::save(char *space) const
{
	for(register int i = 0; i < 32; ++i){
		space[i] = layout[i+i+1] << 6 | mobility[i+i+1] << 4 | layout[i+i+2] << 2 | mobility[i+i+2];
	}
	space[32] = comp_mobility;
	space[33] = oppo_mobility;
	space[34] = comp_counts;
	space[35] = oppo_counts;
}

void Board::recover(const char *space)
{
	static char tmp;
	for(register int i = 0; i < 32; ++i){
		layout[i+i+1] = ((tmp = space[i]) >> 6) & 3;
		mobility[i+i+1] = (tmp >> 4) & 3;
		layout[i+i+2] = (tmp >> 2) & 3;
		mobility[i+i+2] = (tmp) & 3;
	}
	comp_mobility = space[32];
	oppo_mobility = space[33];
	comp_counts = space[34];
	oppo_counts = space[35];
}

void Board::print(int depth)
{
	for(int i = 0; i < 8; ++i){
		for(int k = 0; k < depth; ++k)
			putchar(32);
		for(int j = 0; j < 8; ++j){
			switch(get_color(j+1, i+1)){
				case CH_EMPTY: putchar(' '); 
					if( mobility[pp(j+1, i+1)])
						printf("%d", mobility[pp(j+1, i+1)]);
					else
						putchar('_');
					break;
				case CH_BLACK: putchar(' '); printf("x"); break;
				case CH_WHITE: putchar(' '); printf("o"); break;
			}
		}
		putchar('\n');
	}
	printf("black_num %2d, white_num %2d\n", (role == CH_BLACK)? comp_counts: oppo_counts, \
			(role == CH_WHITE)? comp_counts: oppo_counts);
	printf("black_mob %2d, white_mob %2d\n", (role == CH_BLACK)? comp_mobility: oppo_mobility, \
			(role == CH_WHITE)? comp_mobility: oppo_mobility);
}
