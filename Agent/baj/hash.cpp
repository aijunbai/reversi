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

#include "hash.h"

#define MAX_CLOSENESS 		5
#define MAX_CLOSENESS_2 	1

/**
	POPCOUNT && GET_CLOSENESS && SET_UP referring to Gunnar Andersson's (gunnar@radagast.se) zebra
*/

#ifdef HASH_STATISTICS
int HashTable::count[HASH_SIZE];
#endif

Pos 	 HashTable::hash_key;
Pos	 HashTable::ins_h;
EvalPool HashTable::eval_pool;
HashPool HashTable::hash_pool;
HashPool HashTable::hash_copy;
BitBoard HashTable::bitboard;
bool	 HashTable::changed[HASH_SIZE];

inline int HashTable::popcount( unsigned int b ) 
{
	int n;
	for (n = 0; b != 0; ++n, b &= (b - 1) );
	return n;
}

inline int HashTable::get_closeness( unsigned int a, unsigned int b)
{
	return abs( popcount( a ^ b ) - 16 );
}

void HashTable::setup()
{	
	int count =  0;
	register int i, j;
	for(;;){
		if ((++count)%500000 == 1)
			fprintf(stderr, ".");
		
		color_mask[2] = (Pos)((my_random() << 3) + my_random() + (my_random() >> 2));
		color_mask[3] = (Pos)((my_random() << 3) + my_random() + (my_random() >> 2));
		if(get_closeness(color_mask[2], color_mask[3]) < MAX_CLOSENESS_2)
			break;
	}
	
	for(i = 1; i < 65; ++i){
		TRY_AGAIN:
		if ((++count)%500000 == 1)
			fprintf(stderr, ".");
		
		pos_mask[1][i] = (Pos)((my_random() << 3) + my_random() + (my_random() >> 2));
		pos_mask[2][i] = (Pos)((my_random() << 3) + my_random() + (my_random() >> 2));
		pos_mask[3][i] = (Pos)((my_random() << 3) + my_random() + (my_random() >> 2));	
		
		if( get_closeness(pos_mask[1][i], color_mask[2]) > MAX_CLOSENESS ||\
				  get_closeness(pos_mask[2][i], color_mask[2]) > MAX_CLOSENESS ||\
				  get_closeness(pos_mask[3][i], color_mask[2]) > MAX_CLOSENESS)
			goto TRY_AGAIN;			

		if( get_closeness(pos_mask[1][i], color_mask[3]) > MAX_CLOSENESS ||\
				  get_closeness(pos_mask[2][i], color_mask[3]) > MAX_CLOSENESS ||\
				  get_closeness(pos_mask[3][i], color_mask[3]) > MAX_CLOSENESS)
			goto TRY_AGAIN;			

		if( get_closeness(pos_mask[1][i], pos_mask[2][i]) > MAX_CLOSENESS_2 ||\
				  get_closeness(pos_mask[2][i], pos_mask[3][i]) > MAX_CLOSENESS_2 ||\
				  get_closeness(pos_mask[3][i], pos_mask[1][i]) > MAX_CLOSENESS_2)
			goto TRY_AGAIN;	
		
		for(j = 1; j < i; j++){
			if(get_closeness(pos_mask[1][i], pos_mask[1][j]) > MAX_CLOSENESS)
				goto TRY_AGAIN;			
		}
		
		for(j = 1; j < i; ++j){
			if(get_closeness(pos_mask[2][i], pos_mask[2][j]) > MAX_CLOSENESS)
				goto TRY_AGAIN;			
		}
		
		for(j = 1; j < i; ++j){
			if(get_closeness(pos_mask[3][i], pos_mask[3][j]) > MAX_CLOSENESS)
				goto TRY_AGAIN;			
		}
	}
	init_flip();
	puts("\n>>>> hash setupped");
}

inline void HashTable::init_flip()
{
	register int i, j, k;
	for(i = 1; i < 4; ++i){
		for(j = 1; j < 4; ++j){
			for(k = 1; k < 65; ++k){
				pos_flip[i][j][k] = pos_mask[j][k] - pos_mask[i][k];
			}
		}
	}
}

/**
	IN ASSERTION: a[i] == (char){1, 2, 3}
*/
inline Pos HashTable::key(char *a)
{
	static Pos key;
	key = 0;
	for(register int i = 1; i < 65; ++i)
		key += pos_mask[(int)a[i]][i];
	return key;
}

void HashTable::init_both(char *board)
{
	hash_key = key(board);
	bitboard.setup(board);
}

void HashTable::update_both(int p, char &o, char &n)	// this will flow the board all the time
{
	hash_key += pos_flip[(int)o][(int)n][p];
	bitboard.update(p, o, n);
}

void HashTable::check_update(char *board)
{
	Pos a = key(board);
	if(a != hash_key)
		puts("hash_key");
	BitBoard b(board);
	if(b != bitboard)
		puts("bitboard");
}

void HashTable::save_both(Pos &temp, BitBoard &copy)
{
	temp = hash_key;
	copy = bitboard;
}

void HashTable::recover_both(Pos &temp, BitBoard &copy)
{
	hash_key = temp;
	bitboard = copy;
}

bool HashTable::exist(char &color)
{
	ins_h = (hash_key + color_mask[(int)color]) & hash_mask;
	INC(count[ins_h]);
	
	if(match(hash_copy+ins_h, color)){
		match_best = (hash_copy+ins_h)->best();
		INC(matched);
		return true;
	}
	if(match(hash_pool+ins_h, color)){
		match_best = (hash_pool+ins_h)->best();
		INC(matched);
		return true;
	}
	return false;
}

char HashTable::exist(char &color, int &alpha, int &beta, int &depth)
{
	ins_h = (hash_key + color_mask[(int)color]) & hash_mask;
	INC(count[ins_h]);
		
	if(match(hash_pool+ins_h, color)) {
		match_eval = eval_pool[ins_h];
		match_type = (hash_pool+ins_h)->type();
		match_depth = (hash_pool+ins_h)->depth();
		
		if(exactly_exist(alpha, beta, depth)){
			INC(matched);
			return H_EXACT_EXIST;
		}
		if(match(hash_copy+ins_h, color))
			match_best = (hash_copy+ins_h)->best();
		else
			match_best = (hash_pool+ins_h)->best();
		INC(matched);
		return H_MATCH_EXIST;		   
	}
	INC(missed);
	return H_NONE_EXIST;
}

inline bool HashTable::exactly_exist(int &alpha, int &beta, int &depth) const
{
	return (
		changed[ins_h] &&
		((char)depth == match_depth) &&
		(
		 	(match_type == H_EXACT) ||
		 	(match_type == H_LOW_BOUND && match_eval >= beta) ||
		 	(match_type == H_UP_BOUND && match_eval <= alpha)
		)		
	       );	
}

void HashTable::insert(int &e, char &c, int t, int &s, int &d, int i)
{
	static BitBoard &b = bitboard;
	ins_h = (hash_key + color_mask[(int)c]) & hash_mask;	
	(hash_pool+ins_h)->setup(b, c, t, s, d, i);
	eval_pool[ins_h] = e;	
	(void) ((changed[ins_h])? : changed[ins_h] = true);
	
	if (
		   (hash_pool+ins_h)->disc() <= (hash_copy+ins_h)->disc() 
#if 0
		  	||
		   (hash_copy[ins_h].disc_best_depth_type_color == 0)
#endif 
	   ){
		hash_copy[ins_h] = hash_pool[ins_h];
		++save_updated;
	}
	INC(inserted);
}

inline bool HashTable::match(HashEntry *ph, char &color) const
{
	return (ph->color() == color && ph->board() == bitboard);
}

#define HASH_SAVE_BLACK	"./Conf/data.black"
#define HASH_SAVE_WHITE	"./Conf/data.white"

void HashTable::save()
{
	strcpy(save_path, (role == CH_BLACK)? HASH_SAVE_BLACK: HASH_SAVE_WHITE);
	FILE *fout = fopen(save_path, "wb");
	if(fout == NULL)
		perror(">>>> create file error"), exit(1);
	
	if (fwrite(&save_flag, sizeof(save_flag), 1, fout) != 1)
		perror(">>>> write to file error"), exit(2);
	
	if (fwrite(&hash_copy, sizeof(HashEntry), hash_size, fout) != hash_size)
		perror(">>>> write to file error"), exit(3);
	fclose(fout);
	printf(">>>> hash file saved\n");
	printf(">>>> this time updated %d\n", save_updated);
}

void HashTable::load()
{
	static bool first = true;
	if(first == true){
		first = false;
		re_load();
	}
}

inline void HashTable::re_load()
{
	strcpy(save_path, (role == CH_BLACK)? HASH_SAVE_BLACK: HASH_SAVE_WHITE);
	FILE *fin = fopen(save_path, "rb");
	if(fin == NULL){
		fprintf(stderr, ">>>> miss hash saved file\n");
		return;
	}
	long temp;
	size_t actual_read;
	actual_read = fread(&temp, sizeof(temp), (size_t) 1, fin);
	if(actual_read != 1)
		perror(">>>> read error"), exit(2);
	if(temp != save_flag){
		fprintf(stderr, ">>>> wrong hash saved file\n");
		return;
	}
	actual_read = fread(&hash_copy, sizeof(HashEntry), (size_t) hash_size, fin);
	fclose(fin);
	
	for(Pos i = 0; i < hash_size; ++i){
		hash_pool[i] = hash_copy[i];
	}
	printf(">>>> hash saved file loaded, actual_read %ld\n", actual_read);
}

void HashTable::view_file()
{
	for(Pos i = 0; i < hash_size; ++i){
		hash_copy[i].board().print();
		printf("%2d: disc %d, best (%d, %d), depth %d, type %s, color %s\n",
		       i,
		       (hash_copy+i)->disc(), 
		       xx[(hash_copy+i)->best()],
		       yy[(hash_copy+i)->best()], 
		       (hash_copy+i)->depth(), 
		       ((hash_copy+i)->type() == H_EXACT)? "H_EXACT": 
				       ((hash_copy+i)->type() == H_LOW_BOUND)? "H_LOW_BOUND": "H_UP_BOUND", 
		       ((hash_copy+i)->color() == CH_BLACK)? "CH_BLACK": "CH_WHITE");
		if(i%20 == 0)
			getchar();
	}	
	exit(0);
}


