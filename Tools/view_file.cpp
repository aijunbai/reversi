#include <stdio.h>
#include <stdlib.h>
#include "../Agent/baj/bitboard.h"
#include "../Agent/baj/common.h"
#include "../Agent/baj/myrandom.h"

#define H_EXACT		(char)1
#define H_LOW_BOUND 	(char)2
#define H_UP_BOUND	(char)4
 
struct HashEntry{
	BitBoard board;
	int	disc_best_depth_type_color;
	
	int disc(){ return (disc_best_depth_type_color >> 24) & 255;}
	int best(){ return (disc_best_depth_type_color >> 16) & 255;}
	int depth(){ return (disc_best_depth_type_color >> 8) & 255;}
	char type(){ return (disc_best_depth_type_color >> 4) & 15;}
	char color(){ return (disc_best_depth_type_color) & 15;}
};

#define SIZE (1 << 22)
static struct HashEntry hash_copy[SIZE];

void view(int i)
{
	hash_copy[i].board.print();
	printf("%2d: disc %d, best (%d, %d), depth %d, type %s, color %s\n",
	       i,
	       hash_copy[i].disc(), 
	       xx[hash_copy[i].best()],
	       yy[hash_copy[i].best()], 
	       hash_copy[i].depth(), 
		       (hash_copy[i].type() == H_EXACT)? "H_EXACT": 
				       (hash_copy[i].type() == H_LOW_BOUND)? "H_LOW_BOUND": "H_UP_BOUND", 
	       (hash_copy[i].color() == CH_BLACK)? "CH_BLACK": "CH_WHITE");	
}

int main(int argc, char *argv[])
{
	init_reference();
	long temp;
	puts("open src file...");
	FILE *fin;
	if(argc > 1  && !strcmp(argv[1], "white"))
		fin = fopen("../Conf/data.white", "rb");
	else
		fin = fopen("../Conf/data.black", "rb");
	if(fin == NULL){
		fprintf(stderr, ">>>> miss hash saved file\n");
		return 1;
	}
	size_t actual_read;
	actual_read = fread(&temp, sizeof(temp), (size_t) 1, fin);
	if(actual_read != 1)
		perror(">>>> read error"), exit(2);
	actual_read = fread(&hash_copy, sizeof(struct HashEntry), (size_t) SIZE, fin);
	fclose(fin);

	for(Pos i = 0; i < SIZE; i++){
		if(hash_copy[i].depth() == 0)
		view(i);
	}	

	return 0;
}
