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
#include "board.h"
#include "cost.h"
#include "eval.h"
#include "agent.h"
#include "TCPClient.h"
#include "version.h"
#include "hash.h"
#include "history.h"

#define MSG_SIZE 	256
 
void usage(char *name)
{
	fprintf(stderr, "usage: %s [IpAddr]\n", name);
}

void version()
{
	printf(">>>> Reversi Agent version " VERSION ", by Bai Aijun\n");
	puts("my email: <baj@mail.ustc.edu.cn, baj_forever@126.com>");
}

int main( int argc, char *argv[])
{	
	version();
	if(argc > 2){
		usage(argv[0]);
		exit(1);
	}	

	char *host = "127.0.0.1";
	if(argc == 2 ){
		if( strlen(argv[1]) > 255){
			fprintf(stderr, "too long IpAddr %s\n", argv[1]);
			exit(1);
		}
		else 
			host = argv[1];
	}
	
	char msg[MSG_SIZE];
	init_reference();
	
	HashTable	*hash	= new HashTable;
	Board 		*board	= new Board(hash);
	HistoryTable	*history = new HistoryTable(board);
	Cost    	*cost	= new Cost(board);
	Evaluation 	*eval	= new Evaluation(board, cost);
	Agent   	*agent	= new Agent(board, cost, eval, hash, history);
	TCPClient 	*client	= new TCPClient(host);
	
	
	while( client->get_msg( msg, MSG_SIZE)){
		switch( agent->do_msg( msg)){
			case NEED_TO_SEND:	client->send_msg( msg, MSG_SIZE); break;
			case NO_NEED_TO_SEND:	break;
			case FINISHED:		goto OUT;
		}
	}
	
OUT:	delete client; delete board; 
	delete cost;   delete eval;
	delete agent;  delete hash;
	return 0;
}
