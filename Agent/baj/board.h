#ifndef BOARD_H
#define BOARD_H

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

class HashTable;

class Board{
	friend class Agent; 
	friend class Cost;
	
	HashTable *hash;
	char layout[65];
	char mobility[65];
	static const unsigned char possible_dir[][8];
		
	void set_color(int, int, char);
	void set_color(int, char);
	char get_color(int , int );
	bool can_put(int , int, char);
	void next_pos(int &, int &, unsigned char);
	int  stretch(int, int, unsigned char, char);
	void update_dir(int, int, unsigned char, char);
	void update_configure_all();
public:	
	Board(HashTable *h = 0){
		assert(h);
		hash = h;
	}
	char get_color(int);
	void put(int, char);
	int able_to_put(char&);	
	bool can_put(int, char&);
	int can_put_corner(char&);
	bool adjacent(int, int, char);
	void update(char*);	
	void save(char*) const;
	void recover(const char*);
	void print(int depth = 0);	
	
	int  comp_counts;
	int  oppo_counts;
	int  comp_mobility;
	int  oppo_mobility;
};

#endif
