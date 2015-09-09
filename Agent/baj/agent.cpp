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
 
#include "agent.h"
#include "bitboard.h"
#include "board.h"
#include "cost.h"
#include "eval.h"
#include "hash.h"
#include "history.h"
				
char	Agent::cpy[MAX_SEARCH_DEPTH][36];
int	Agent::value[MAX_SEARCH_DEPTH];
//bool 	Agent::pv_found[MAX_SEARCH_DEPTH];
int 	Agent::result[MAX_SEARCH_DEPTH];
int	Agent::cur_move[MAX_SEARCH_DEPTH];
int	Agent::best_move[MAX_SEARCH_DEPTH];
int	Agent::temp_move[MAX_SEARCH_DEPTH];
int	Agent::move_list_len[MAX_SEARCH_DEPTH];
Pos 	Agent::ins_h[MAX_SEARCH_DEPTH];
BitBoard Agent::ins_b[MAX_SEARCH_DEPTH];

char Agent::do_msg(char *msg)
{
	char reply = 0;
	switch(*msg){
		case BOARD_DESCRIPTION:
			reply = NO_NEED_TO_SEND;
			update(msg); break;
		case ACTION:
			reply = NEED_TO_SEND;
			act(msg); break;
		case WAIT:
			reply = NO_NEED_TO_SEND;
			puts(">>>> wait for one time"); break;
		case ONE_MORE_TIME:
			reply = NEED_TO_SEND;
			times_left(1, msg); break;
		case TWO_TIMES_LEFT:
			reply = NEED_TO_SEND;
			times_left(2, msg); break;
		case THREE_TIMES_LEFT:
			reply = NEED_TO_SEND;
			times_left(3,  msg); break;
		case RL_BLACK:
			reply = NO_NEED_TO_SEND; puts(">>>> I'm the Black");
			role = RL_BLACK; opp_role = opposite(role); break;
		case RL_WHITE:
			reply = NO_NEED_TO_SEND; puts(">>>> I'm the White");
			role = RL_WHITE; opp_role = opposite(role); break;
		case YOU_WIN:
			reply = FINISHED;
			puts(">>>> I win"); break;
		case YOU_LOSE:
			reply = FINISHED;
			puts(">>>> I lose"); break;
		case YOU_TIE:
			reply = FINISHED;
			puts(">>> We tie"); break;
	}
	return reply;
}

void Agent::update(char *msg)
{
	hash->load();
	
	board->update(msg);
	eval->update();
	if(!end_game)
		cost->update();
	history->reorder_move(step);
	hash->init_both(board->layout);
#ifdef VERBOSE
	board->print();
	eval->print();
	hash->print();
#endif //VERBOSE
}

inline void Agent::act( char *msg)
{
	int i = best_choice();
	sprintf(msg, "%c", (char)(i));
	printf(">>>> action sended: x %d, y %d, pos %d\n", xx[i], yy[i], i);
}

inline void Agent::times_left(int t, char *msg)
{
	printf(">>>> %d times left\n", t);
	int i = random_to_put();
	sprintf(msg, "%c", (char)(i));
	printf(">>>> action sended: x %d, y %d, pos %d\n", xx[i], yy[i], i);
}

inline int Agent::random_to_put()
{
	int pos;
	for(;;){
		pos = my_random()%64 + 1;
		if( board->can_put( pos, role))
			break;
	}
	return pos;
}

inline int  Agent::terminal_eval(char color, int depth, int alpha, int beta)
{
	save_all(depth);
	
	if(best_move[depth]){
		board->put(best_move[depth], color);
		if(!end_game)
			cost->update();
		value[depth] = eval->value();
		recover_all(depth);
		if( color == role){
			if( value[depth] > alpha){
				if( (alpha = value[depth]) >= beta){
					return alpha;
				}
			}
			result[depth] = value[depth];
		}
		else{
			if( value[depth] < beta){
				if( (beta = value[depth]) <= alpha){
					return beta;
				}
			}
			result[depth] = value[depth];
		}
	}
	else{
		result[depth] = (color == role)? -MAX_INT: MAX_INT;
	}
	
	move_list_len[depth] = history->move_list_len(color, step+depth);
	for(register int i = 1; i <= move_list_len[depth]; ++i){
		cur_move[depth] = history->move(color, step+depth, i);
		if( !board->can_put(cur_move[depth], color) || cur_move[depth] == temp_move[depth])
			continue;	
		board->put(cur_move[depth], color);
		if(!end_game)
			cost->update();
		value[depth] = eval->value();
		recover_all(depth);

		if( color == role){
			if( value[depth] > alpha){
				if( (alpha = value[depth]) >= beta){
					history->advance_move(color, step+depth, i);
					hash->insert(alpha, color, H_LOW_BOUND,
							cur_move[depth], depth, step+depth);
					return alpha;
				}
			}
			if( value[depth] > result[depth]){
				result[depth] = value[depth];
				best_move[depth] = cur_move[depth];
			}
		}
		else{
			if( value[depth] < beta){
				if( (beta = value[depth]) <= alpha){
					history->advance_move(color, step+depth, i);
					hash->insert(beta, color, H_UP_BOUND,
							cur_move[depth], depth, step+depth);
					return beta;
				}
			}
			if( value[depth] < result[depth]){
				result[depth] = value[depth];
				best_move[depth] = cur_move[depth];
			}
		}
	}
	
	hash->insert(result[depth], color, H_EXACT, best_move[depth], depth, step+depth);
	return result[depth];
}

int Agent::estimation(char color, int depth, int alpha, int beta)
{	
	if( !board->able_to_put(color)){
		color = opposite(color);
		if( !board->able_to_put(color)){
			if(!end_game)
				cost->update();
			return eval->value();
		}
	}
	
	temp_move[depth] = best_move[depth] = 0;
	switch(hash->exist(color, alpha, beta, depth)){
		case H_NONE_EXIST: break;
		case H_MATCH_EXIST: temp_move[depth] = best_move[depth] = hash->match_best; break;
		case H_EXACT_EXIST: return hash->match_eval;
	}

	if(depth == max_depth){		//the last search ply, no pv used
		return terminal_eval(color, depth, alpha, beta);
	}

	//pv_found[depth] = false;
	save_all(depth);
	
	if(best_move[depth]){
		board->put(best_move[depth], color);
		value[depth] = estimation(opposite(color), depth+1, alpha, beta);
		recover_all(depth);
		
		if( color == role){
			if( value[depth] > alpha){
				if( (alpha = value[depth]) >= beta){
					return alpha;
				}
				//pv_found[depth] = true;
			}
			result[depth] = value[depth];
		}
		else{
			if(value[depth] < beta){
				if( (beta = value[depth]) <= alpha){
					return beta;
				}
				//pv_found[depth] = true;
			}
			result[depth] = value[depth];
		}		
	}
	else{
		result[depth] = (color == role)? -MAX_INT: MAX_INT;
	}

	move_list_len[depth] = history->move_list_len(color, step+depth);
	for(register int i = 1; i <= move_list_len[depth]; ++i){
		cur_move[depth] = history->move(color, step+depth, i);
		if( !board->can_put(cur_move[depth], color) || cur_move[depth] == temp_move[depth])
			continue;			
		board->put(cur_move[depth], color);
		//if(pv_found[depth] == true){
		//	if(color == role){
		//		value[depth] = estimation(opposite(color), depth+1, beta-1, beta);
		//		if(value[depth] > alpha && value[depth] < beta){
		//			value[depth] = 
		//					estimation(opposite(color), depth+1, alpha, value[depth]);
		//		}
		//	}
		//	else{
		//		value[depth] = estimation(opposite(color), depth+1, alpha, alpha+1);
		//		if(value[depth] > alpha && value[depth] < beta){
		//			value[depth] = 
		//					estimation(opposite(color), depth+1, value[depth], beta);
		//		}
		//	}
		//}
		//else
			value[depth] = estimation(opposite(color), depth+1, alpha, beta);
		
		recover_all(depth);

		if( color == role){
			if( value[depth] > alpha){
				if( (alpha = value[depth]) >= beta){
					history->advance_move(color, step+depth, i);
					hash->insert(alpha, color, H_LOW_BOUND,
							cur_move[depth], depth, step+depth);
					return alpha;
				}
				//(void)((pv_found[depth])? : pv_found[depth] = true);
			}
			if(value[depth] > result[depth]){
				result[depth] = value[depth];
				best_move[depth] = cur_move[depth];
			}
		}
		else{
			if(value[depth] < beta){
				if( (beta = value[depth]) <= alpha){
					history->advance_move(color, step+depth, i);
					hash->insert(beta, color, H_UP_BOUND,
							cur_move[depth], depth, step+depth);
					return beta;
				}
				//(void)((pv_found[depth])? : pv_found[depth] = true);
			}
			if(value[depth] < result[depth]){
				result[depth] = value[depth];
				best_move[depth] = cur_move[depth];
			}
		}		
	}
	hash->insert(result[depth], color, H_EXACT, best_move[depth], depth, step+depth);
	return result[depth];
}

int Agent::best_choice()
{	
	static int base = 0;
	result[0] = -MAX_INT;
	move_list_len[0] = history->move_list_len(role, step);
	save_all(0);

	temp_move[0] = best_move[0] = 0;
	if(hash->exist(role)){
		 temp_move[0] = best_move[0] = hash->match_best;
	}
	
	if(best_move[0]){
		board->put(best_move[0], role);
		result[0] = estimation(opp_role, 1, -MAX_INT, MAX_INT);
		recover_all(0);
#ifdef VERBOSE
		printf("%d @ (%d, %d) <- %d (best)\n", result[0], xx[best_move[0]], yy[best_move[0]],
		       					history->get_init_index(best_move[0]));
#endif		
	}

	for(register int i = 1; i <= move_list_len[0]; ++i){
		cur_move[0] = history->move(role, step, i);
		if( !board->can_put(cur_move[0], role) || cur_move[0] == temp_move[0])
			continue;
		board->put(cur_move[0], role);
		value[0] = estimation(opp_role, 1, result[0], MAX_INT);
		recover_all(0);
#ifdef VERBOSE
		printf("%d @ (%d, %d) <- %d\n", value[0], xx[cur_move[0]], yy[cur_move[0]],\
 							history->get_init_index(cur_move[0]));
#endif		
		if(value[0] > result[0]){
			result[0] = value[0];
			best_move[0] = cur_move[0];
		}
	}
	hash->insert(result[0], role, H_EXACT, best_move[0], base, step);	// this is for hash history saved file
	return best_move[0];
}

inline void Agent::save_all(int depth)
{
	board->save(cpy[depth]);
	hash->save_both(ins_h[depth], ins_b[depth]);
}

inline void Agent::recover_all(int depth)
{
	board->recover(cpy[depth]);
	hash->recover_both(ins_h[depth], ins_b[depth]);
}
