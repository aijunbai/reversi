#ifndef TCPServer_h
#define TCPServer_h

/***************************************************************************
 *   Copyright (C) 2006 by Bai Aijun   *
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

#include "board.h"
extern "C"{
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <pthread.h>
#include <assert.h>
#include <sys/time.h>
#include <signal.h>
}

#define DEFAULT_PORT 20000 
#define BUFFER_SIZE  256
#define DEFAULT_QUEUE_SIZE 10


struct client_ds{
	int socket;
	char IP[20];
	unsigned long port;
	char  role;
	pthread_t thread_id;
	client_ds(int n, const char *s, short i, char r, pthread_t id){
		socket = n;
		assert(strlen(s) < 20);
		strcpy(IP, s);
		port = i;
		role = r;
		thread_id = id;
	}
};

class TCPServer{	
	int  port;
	int  server_socket;
	int  client_socket;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	pthread_t client_thread_id;
	int clients_n;
	Board *board;
public:
	TCPServer( Board *b = NULL, int call_port = DEFAULT_PORT);
	void comunication();
	bool authentication(int );
	void time_limit(int socket, int sec){
		struct timeval timev;
		timev.tv_sec = sec;
		timev.tv_usec = 0;
		setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &timev, sizeof(timev));		
	}
	void main_loop(char, int, char*);
	void recv_msg( int , char *, size_t );
	void send_msg( int , char *, char );
	void send_msg( int , char *, char *);
	void send_rslt(int , char , char );

};
#endif

