#ifndef COMMON_H
#define COMMON_H
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
 
extern "C"{
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
}

#include "myrandom.h"

#define MAX_INT		9999999

#define ACTION		(char)70
#define WAIT		(char)71
#define BOARD_DESCRIPTION (char)69

#define ONE_MORE_TIME	(char)72
#define TWO_TIMES_LEFT	(char)73
#define THREE_TIMES_LEFT (char)76

#define YOU_WIN 	(char)74
#define YOU_LOSE 	(char)75
#define YOU_TIE  	(char)77

#define RL_BLACK 	(char)2
#define RL_WHITE 	(char)3
#define RL_MONITOR	(char)4

#define NEED_TO_SEND 	(char)255
#define NO_NEED_TO_SEND (char)254
#define FINISHED 	(char)253

#define CH_EMPTY 	1
#define CH_BLACK 	RL_BLACK
#define CH_WHITE 	RL_WHITE

#define MAX_SEARCH_DEPTH	25

typedef unsigned int Pos;

extern int xx[65];
extern int yy[65];

extern const int _depth[65];
extern int max_depth;
extern int step;
extern bool end_game;

extern char role;
extern char opp_role;

extern void init_reference();

#define MAX(a, b)  ((a) > (b)? (a): (b))
#define MIN(a, b)  ((a) < (b)? (a): (b))
#define opposite(c) (CH_BLACK + CH_WHITE - (c))
#define pp(x, y) ((x) + ((y) - 1) * 8)

#define PRINT(name) do {\
			printf(#name " %d\n", name);\
		    }while(0)
		    
#define TRACK(string) do {\
			printf("%s: %s\n", __FUNCTION__ , string);\
		   }while(0)

#define 	VERBOSE
#define  	PARITY
 
#endif
