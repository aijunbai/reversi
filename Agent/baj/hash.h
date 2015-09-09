#ifndef HASH_H
#define HASH_H
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
#include "common.h"

#define H_EXACT		1
#define H_LOW_BOUND 	2
#define H_UP_BOUND	4

#define H_NONE_EXIST	(char)0
#define H_MATCH_EXIST	(char)1
#define H_EXACT_EXIST	(char)2

#define HASH_BITS	22
#define HASH_SIZE	((unsigned) 1 << HASH_BITS)
#define HASH_MASK 	(HASH_SIZE-1)
#define POOL_SIZE	HASH_SIZE

//#define HASH_STATISTICS

typedef struct HashEntry{
	void setup(BitBoard &bitboard, char &color, int &type, int &best, int &depth, int &disc){		
		disc_best_depth_type_color = (disc << 24) | 
				(best << 16) |
				(depth << 8) |
				(type << 4)  |
				(color);
		bits = bitboard;
	}
	int disc() const { return (disc_best_depth_type_color >> 24) & 255;}
	int best() const { return (disc_best_depth_type_color >> 16) & 255;}
	int depth() const { return (disc_best_depth_type_color >> 8) & 255;}
	char type() const { return (disc_best_depth_type_color >> 4) & 15;}
	char color() const { return (disc_best_depth_type_color) & 15;}
	BitBoard &board() { return bits;}
	
private:
	BitBoard bits;
	int	disc_best_depth_type_color;	
} HashEntry, HashPool[POOL_SIZE];

typedef int EvalPool[POOL_SIZE];
 
class HashTable{
	Pos hash_mask;
	Pos hash_size;
	Pos pool_size;

	static EvalPool eval_pool;
	static HashPool hash_pool;
	static HashPool hash_copy;
	static Pos  hash_key;
	static Pos ins_h;
	static BitBoard bitboard;
	static bool changed[HASH_SIZE];
	
	Pos pos_mask[4][65];	///pos_mask[0] not used
	Pos pos_flip[4][4][65];
	Pos color_mask[4];
	
	int  popcount(unsigned int b );
	int  get_closeness(unsigned int a, unsigned int b );
	void setup();
	void init_flip();
	bool match(HashEntry*, char&) const;
	
	long save_flag;
	char save_path[20];
	int  save_updated;
	void re_load();
	void save();
	
#ifdef HASH_STATISTICS
	static int count[HASH_SIZE];
	int matched;
	int missed;
	int inserted;
#define INC(x) ++x
#else
#define INC(x)
#endif
public:
	HashTable(){
		hash_mask = HASH_MASK;
		hash_size = HASH_SIZE;
		pool_size = POOL_SIZE;
		
		hash_key = 0;
		setup();
		save_flag = my_random();
		save_updated = 0;
	
#ifdef HASH_STATISTICS
		matched = 0;
		missed = 0;
		inserted = 0;
#endif
	}
	~HashTable(){
		this->save();
#ifdef HASH_STATISTICS
		int max = -1;
		int min = hash_size;
		for(unsigned int i = 0; i < hash_size; ++i){
			max = MAX(max, count[i]);
			min = MIN(min, count[i]);	
		}
		int *a = new int[max+1-min];
		float  mean = 0.0;
		for(int i = min; i < max+1; ++i){
			a[i-min] = 0;
		}
		for(unsigned int i = 0; i < hash_size; ++i){
			++a[count[i]-min] ;
		} 
		printf(">>>> multi min %d, max %d\n", min, max);
		for(int i = min; i < max+1; ++i){
			mean += (float)(i * a[i-min]);
		}
		if(min == 0)
			hash_size -= a[0];
		mean /= (float)(hash_size);
		printf(">>>> multi weighted mean %f\n", mean);
		delete a;
#endif
	}
	Pos key(char*);
	char exist(char&, int&, int&, int&);
	bool exist(char&);
	bool exactly_exist(int&, int&, int&) const;
	void insert(int&, char&, int, int&, int&, int);
	void print(){
#ifdef HASH_STATISTICS
		printf("hash inserted %d, matched %d, missed %d\n", inserted, matched, missed);
#endif
	}
	/**
		MATCH_EVAL is always accessed after EXIST is called, and so are ohers
	*/
	int match_eval;	
	char match_type;
	char match_best;
	char match_depth;
	
	void init_both(char*);
	void update_both(int, char&, char&);
	void check_update(char*);
	void save_both(Pos&, BitBoard&);
	void recover_both(Pos&, BitBoard&);
	
	void load();
	void view_file();
};

#endif
