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

#include "bitboard.h"

#define BT_BLACK 0
#define BT_WHITE 1
#define BT_HIGH  0
#define BT_LOW   1
#define BT_COLOR(c)	(((c) == CH_BLACK)? BT_BLACK: BT_WHITE)
#define BT_LEVEL(pos)	((pos < 33)? BT_HIGH: BT_LOW)

const Bit32 BitBoard::BT_MASK[65] = {
	 0x00000000,
	 0x80000000, 0x40000000, 0x20000000, 0x10000000, 0x08000000, 0x04000000, 0x02000000, 0x01000000,
	 0x00800000, 0x00400000, 0x00200000, 0x00100000, 0x00080000, 0x00040000, 0x00020000, 0x00010000,
	 0x00008000, 0x00004000, 0x00002000, 0x00001000, 0x00000800, 0x00000400, 0x00000200, 0x00000100,
	 0x00000080, 0x00000040, 0x00000020, 0x00000010, 0x00000008, 0x00000004, 0x00000002, 0x00000001,
	 0x80000000, 0x40000000, 0x20000000, 0x10000000, 0x08000000, 0x04000000, 0x02000000, 0x01000000,
	 0x00800000, 0x00400000, 0x00200000, 0x00100000, 0x00080000, 0x00040000, 0x00020000, 0x00010000,
	 0x00008000, 0x00004000, 0x00002000, 0x00001000, 0x00000800, 0x00000400, 0x00000200, 0x00000100,
	 0x00000080, 0x00000040, 0x00000020, 0x00000010, 0x00000008, 0x00000004, 0x00000002, 0x00000001
};

void BitBoard::setup(char *a)
{
	BITS[0][0] = BITS[0][1] = BITS[1][0] = BITS[1][1] = 0;
	for(register int i = 1; i < 65; ++i){
		if(a[i] != CH_EMPTY)
			BITS[BT_COLOR(a[i])][BT_LEVEL(i)] |= BT_MASK[i];
	}
}

void BitBoard::update(int &p, char &o, char &n)
{
    (o == CH_EMPTY)? 0: BITS[BT_COLOR(o)][BT_LEVEL(p)] &= ~BT_MASK[p];
	BITS[BT_COLOR(n)][BT_LEVEL(p)] |= BT_MASK[p];		
}

bool BitBoard::operator==(BitBoard &o)
{
	return (BITS[0][0] == o.BITS[0][0]) &&
			(BITS[0][1] == o.BITS[0][1]) &&
			(BITS[1][0] == o.BITS[1][0]) &&
			(BITS[1][1] == o.BITS[1][1]);
}

bool BitBoard::operator!=(BitBoard &o)
{
	return !(o == *this);
}

void BitBoard::print() const
{	
	putchar('\n');
	printf("   ");
	for(int i = 1; i < 9; ++i){
		printf("%d ", i);
	}
	putchar('\n');
	for(int i = 1; i < 65; ++i){
		if(xx[i] == 1)
			printf("%d ", yy[i]);
		if(BITS[BT_BLACK][BT_LEVEL(i)] & BT_MASK[i]){
			putchar(' '); printf("●");
		}
		else if(BITS[BT_WHITE][BT_LEVEL(i)] & BT_MASK[i]){
			putchar(' '); printf("○");
		}
		else
			printf(" _");
		if(xx[i] == 8)
			printf("\n");
	}
}

