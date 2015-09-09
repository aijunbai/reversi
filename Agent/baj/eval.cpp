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

#include "eval.h"
#include "cost.h"
#include "board.h"
 
const int Evaluation::_depth[65] = {
	0,
	7, 7, 7, 9, 9, 9, 9, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 9, 9, 9, 9, 9, 9, 11, 11, 15,
	14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
	-1
};

void Evaluation::update()
{
	corner = 0;
	if(board->get_color(1) != CH_EMPTY)
		++corner;
	if(board->get_color(8) != CH_EMPTY)
		++corner;
	if(board->get_color(57) != CH_EMPTY)
		++corner;
	if(board->get_color(64) != CH_EMPTY)
		++corner;
		
	if( (step = board->comp_counts + board->oppo_counts) > 64)
		step = 64;		//disc played
	max_depth = _depth[step];
	if( (m_coeff = 100 - (100 * (step + max_depth - 3)) / 60) < 0)
		m_coeff = 0;
	if( (n_coeff = 100 - m_coeff) < 0)
		n_coeff = 0;
	m_coeff *= 3;
	m_coeff += (25) * (4 - corner);
	end_game = (step >= 48)? true: false;
}

int Evaluation::value()
{
	static char tmp;
	static int delta_cost;
	static const int &comp_counts = board->comp_counts;
	static const int &oppo_counts = board->oppo_counts;
	static const int &comp_mobility = board->comp_mobility;
	static const int &oppo_mobility = board->oppo_mobility;
	
	if( !oppo_counts)
		return (MAX_INT - comp_counts);		//this is really the best of all
	if( !comp_counts)
		return -MAX_INT;			//but this is the worst	
	
	if(end_game)
		return (comp_counts - oppo_counts) * 100;
	
	delta_cost = 0;
	for(register int i = 1; i < 65; ++i){
		if( (tmp = board->get_color(i)) == role){
			delta_cost += cost->get_cost(i);
#ifdef PARITY
			if( !board->adjacent(xx[i], yy[i], CH_EMPTY))
				++delta_cost;
#endif
		}
		else if( tmp == opp_role){
			delta_cost -= cost->get_cost(i);
#ifdef PARITY
			if( !board->adjacent(xx[i], yy[i], CH_EMPTY))
				--delta_cost;
#endif
		}
	}
	
	return n_coeff * (comp_counts - oppo_counts) +	m_coeff * ( delta_cost +
			(comp_mobility - oppo_mobility) * 2 - 
			board->can_put_corner(opp_role) * 32);
}

void Evaluation::print()
{
	printf("sp %3d, mc %3d, nc %3d, cn %3d, dp %3d, ev %d\n", \
			step, m_coeff, n_coeff, corner, max_depth+1, value());
}
