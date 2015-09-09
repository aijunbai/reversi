#include <stdio.h>
#include <stdlib.h>

typedef unsigned int Bit32;
struct BitBoard{
	Bit32 BITS[2][2];
};
 
struct HashEntry{
	int	eval;
	struct BitBoard board;
	int	disc_best_depth_type_color;
};

struct ConvertCell{
	struct BitBoard board;
	int	disc_best_depth_type_color;
};

#define SIZE (1 << 22)
static struct HashEntry a[SIZE];
static struct ConvertCell b[SIZE];

int main()
{
	
	long temp;
	puts("open src file...");
	FILE *fin = fopen("../Conf/hash.white", "rb");
	if(fin == NULL){
		fprintf(stderr, ">>>> miss hash saved file\n");
		return 1;
	}
	size_t actual_read;
	actual_read = fread(&temp, sizeof(temp), (size_t) 1, fin);
	if(actual_read != 1)
		perror(">>>> read error"), exit(2);
	actual_read = fread(&a, sizeof(struct HashEntry), (size_t) SIZE, fin);
	fclose(fin);
	
	puts("convert...");
	int i;
	for(i = 0; i < SIZE; i++){
		//...
	}
	
	puts("open dest file...");
	FILE *fout = fopen("white", "wb");
	if(fout == NULL)
		perror(">>>> create file error"), exit(1);
	
	if (fwrite(&temp, sizeof(temp), 1, fout) != 1)
		perror(">>>> write to file error"), exit(2);
	
	if (fwrite(&b, sizeof(struct ConvertCell), SIZE, fout) != SIZE)
		perror(">>>> write to file error"), exit(3);
	fclose(fout);	
	return 0;
}

