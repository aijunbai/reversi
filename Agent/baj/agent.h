#ifndef AGENT_H
#define AGENT_H

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

class BitBoard;
class Board;
class Cost;
class Evaluation;
class HashTable;
class HistoryTable;

class Agent{
	Board 	*board;
	Cost  	*cost;
	Evaluation *eval;
	HashTable *hash;
	HistoryTable *history;
	
	static char	cpy[MAX_SEARCH_DEPTH][36];
	static int 	value[MAX_SEARCH_DEPTH];
	static bool 	pv_found[MAX_SEARCH_DEPTH];
	static int 	result[MAX_SEARCH_DEPTH];
	static int	cur_move[MAX_SEARCH_DEPTH];
	static int	best_move[MAX_SEARCH_DEPTH];
	static int	temp_move[MAX_SEARCH_DEPTH];
	static int	move_list_len[MAX_SEARCH_DEPTH];
	static Pos 	ins_h[MAX_SEARCH_DEPTH];
	static BitBoard ins_b[MAX_SEARCH_DEPTH];
		
	void update(char*);
	void act(char *);
	void times_left(int , char *);
	int  random_to_put();
	int  best_choice();
	int  estimation(char, int, int, int);
	int  terminal_eval(char, int, int, int);
	
	void save_all(int);
	void recover_all(int);
public:
	Agent(Board *b = 0, Cost *c = 0, Evaluation *e = 0, HashTable *h = 0, HistoryTable *hh = 0){
		assert(b && c && e && h && hh);
		board = b;
		cost = c;
		eval = e;
		hash = h;
		history = hh;
	} 
	char do_msg(char *);
};

#endif
