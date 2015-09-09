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
 
#include "cost.h"
#include "board.h"

#define  A 	64
#define  B 	-4
#define  C 	2
#define  D 	0
#define  E 	-8
#define  F 	2
#define  G 	-4
#define  H 	2
#define  I 	1
#define  J 	1
#define  K	8
#define  L	4
#define  M      48

int Cost::cost[65] = {
	0,
	A, B, C, D, D, C, B, A,
	B, E, F, G, G, F, E, B,
	C, F, H, I, I, H, F, C,
	D, G, I, J, J, I, G, D,
	D, G, I, J, J, I, G, D,
	C, F, H, I, I, H, F, C,
	B, E, F, G, G, F, E, B,	
	A, B, C, D, D, C, B, A
}; 

void Cost::update()
{
	static char tmp;
	if((tmp = board->get_color(1)) != CH_EMPTY && cost[1] != M){
		cost[1] = M;
		cost[2] = K;
		cost[3] = K;
		cost[4] = K;
		cost[9] = K;
		cost[10] = K;
		cost[11] = K;
		cost[12] = L;
		cost[17] = K;
		cost[18] = K;
		cost[25] = K;
		cost[26] = L;	
	}
	else if(tmp == CH_EMPTY && cost[1] != A){
		cost[1] = A;
		cost[2] = B;
		cost[3] = C;
		cost[4] = D;
		cost[9] = B;
		cost[10] = E;
		cost[11] = F;
		cost[12] = G;
		cost[17] = C;
		cost[18] = F;
		cost[25] = D;
		cost[26] = G;
	}
	
	if((tmp = board->get_color(8)) != CH_EMPTY && cost[8] != M){
		cost[5] = K;
		cost[6] = K;
		cost[7] = K;
		cost[8] = M;
		cost[13] = L;
		cost[14] = K;
		cost[15] = K;
		cost[16] = K;
		cost[23] = K;
		cost[24] = K;
		cost[31] = L;
		cost[32] = K;
	}
	else if(tmp == CH_EMPTY && cost[8] != A){
		cost[5] = D;
		cost[6] = C;
		cost[7] = B;
		cost[8] = A;
		cost[13] = G;
		cost[14] = F;
		cost[15] = E;
		cost[16] = B;
		cost[23] = F;
		cost[24] = C;
		cost[31] = G;
		cost[32] = D;
	}
	
	if((tmp = board->get_color(57)) != CH_EMPTY && cost[57] != M){
		cost[33] = K;
		cost[34] = L;
		cost[41] = K;
		cost[42] = K;
		cost[49] = K;
		cost[50] = K;
		cost[51] = K;
		cost[52] = L;
		cost[57] = M;
		cost[58] = K;
		cost[59] = K;
		cost[60] = K;
	}
	else if(tmp == CH_EMPTY && cost[57] != A){
		cost[33] = D;
		cost[34] = G;
		cost[41] = C;
		cost[42] = F;
		cost[49] = B;
		cost[50] = E;
		cost[51] = F;
		cost[52] = G;
		cost[57] = A;
		cost[58] = B;
		cost[59] = C;
		cost[60] = D;
	}
	
	if((tmp = board->get_color(64)) != CH_EMPTY && cost[64] != M){
		cost[39] = L;
		cost[40] = K;
		cost[47] = K;
		cost[48] = K;
		cost[53] = L;
		cost[54] = K;
		cost[55] = K;
		cost[56] = K;
		cost[61] = K;
		cost[62] = K;
		cost[63] = K;
		cost[64] = M;
	}
	else if(tmp == CH_EMPTY && cost[64] != A){
		cost[39] = G;
		cost[40] = D;
		cost[47] = F;
		cost[48] = C;
		cost[53] = G;
		cost[54] = F;
		cost[55] = E;
		cost[56] = B;
		cost[61] = D;
		cost[62] = C;
		cost[63] = B;
		cost[64] = A;
	}
}

#undef  A
#undef  B
#undef  C
#undef  D
#undef  E
#undef  F
#undef  G
#undef	H
#undef  I
#undef  J
#undef  K
#undef  L
#undef  M

int Cost::get_cost(int pos)
{
	return cost[pos];
}

void Cost::print()
{
	for(int i = 1; i < 65; ++i){
		printf("%4d", cost[i]);
		if(xx[i] == 8)
			printf("\n");
	}
	printf("\n");
}

