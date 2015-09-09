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


extern "C"{
#include <assert.h>
#include <stdio.h>
}

//#define DEBUG

class Board{
public:
	Board();
	char layout[66];   ///layout[65] = '\0' as end flag, layout[0] is flag of BOARD_DISCREPTION
	void init();
	char get_color(int , int );
	void set_color(int, int, char);
	bool able_to_put(char);

	void put(int, char);
	bool can_put(int , int, char);
	bool can_put(int, char);
	bool can_put(int, int, char, unsigned char);
	void next_pos(int &, int &, unsigned char);
	int stretch(int, int, unsigned char, char);
	char opposite(char );
	void show_board(int);

	bool update(int, char*);
	void update_dir(int, int, unsigned char, char);
	bool finish;
	char winner();
};

#endif
