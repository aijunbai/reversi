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
  
#include "history.h"
#include "board.h"

#define INIT_MOVE_LIST_LEN 60

const int HistoryTable::init_priority[65] = {
	       0,
  /*A*/        1, 8, 57, 64, 
  /*C*/        3, 6, 17, 24, 41, 48, 59, 62,
  /*H*/        19, 22, 43, 46,
  /*D*/        4, 5, 25, 32, 33, 40, 60, 61,
  /*I*/        20, 21, 27, 30, 35, 38, 44, 45,
  /*G*/        12, 13, 26, 31, 34, 39, 52, 53,
  /*F*/        11, 14, 18, 23, 42, 47, 51, 54,
  /*B*/        2, 7, 9, 16, 49, 56, 58, 63,
  /*E*/        10, 15, 50, 55,
  /*J*/        28, 29, 36, 37
};

int HistoryTable::get_init_index(int move)
{
	static bool first = true;
	static int init_index[65];
	if(first == true){
		first = false;
		int n;
		for(int mov = 1; mov < 65; ++mov){
			for(n = 0; n < 65 && init_priority[n] != mov; ++n);
			init_index[mov] = n;
		}
	}
	return init_index[move];
}

void HistoryTable::init()
{
	int i, j, k;
	for(i = 0; i < 2; ++i)				//color diff
		for(j = 1; j < 65; ++j){		//disc played
			sorted_move[i][j][0] = INIT_MOVE_LIST_LEN;
			for(k = 1; k < 65; ++k)		//order index
				sorted_move[i][j][k] = init_priority[k];
		}
}

int HistoryTable::move_list_len(char &color, int disc_played)
{
	return sorted_move[(int)color - 2][disc_played][0];
}

int HistoryTable::move(char &color, int disc_played, int index)
{
	return sorted_move[(int)color - 2][disc_played][index];
}

void HistoryTable::advance_move(char &color, int disc_played, int index)
{
	static int temp;
	if(index == 1)
		return;	
	temp = sorted_move[(int)color - 2][disc_played][index-1];
	sorted_move[(int)color - 2][disc_played][index-1] = sorted_move[(int)color - 2][disc_played][index];
	sorted_move[(int)color - 2][disc_played][index] = temp;		
}

void HistoryTable::float_move(char &color, int disc_played, int index)
{
	static int temp;
	if(index == 1)
		return;	
	temp = sorted_move[(int)color - 2][disc_played][index];
	for(register int i = index; i > 1; i--){
		sorted_move[(int)color - 2][disc_played][i] = sorted_move[(int)color - 2][disc_played][i-1];
	}
	sorted_move[(int)color - 2][disc_played][1] = temp;	
}

void HistoryTable::reorder_move(int disc_played)
{
	register int i, j, k, m;
	static int move_list_len;
	for(i = 0; i < 2; ++i){
		for(j = disc_played; j < 65; ++j){
			move_list_len = sorted_move[i][j][0];
			for(k = 1; k <= move_list_len; ++k){
				if(board->get_color(sorted_move[i][j][k]) != CH_EMPTY){
					move_list_len--;
					for(m = k; m <= move_list_len; m++){
						sorted_move[i][j][m] = sorted_move[i][j][m+1];
					}
					k --;
				}				
			}
			sorted_move[i][j][0] = move_list_len;
		}
	}
}

