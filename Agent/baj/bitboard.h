 
#ifndef BITBOARD_H
#define BITBOARD_H

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

#include "common.h"

typedef unsigned int Bit32;

class BitBoard{
	static const Bit32 BT_MASK[65];
public:
	Bit32 BITS[2][2];

	BitBoard(){
		BITS[0][0] = BITS[0][1] = BITS[1][0] = BITS[1][1] = 0;
	}
	BitBoard(char *board){
		setup(board);
	}
	void setup(char*);
	void update(int&, char&, char&);
	bool operator==(BitBoard&);
	bool operator!=(BitBoard&);
	void print() const;
};

#endif
