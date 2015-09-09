#ifndef TCPClient_h
#define TCPClient_h

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
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <assert.h>
}

#define SERVER_PORT  20000
#define BUFFER_SIZE  256

class TCPClient{
	int client_socket;
	struct sockaddr_in client_addr;
	struct sockaddr_in server_addr;
	char *host_name;
	struct hostent *server_host;

	void time_limit(int socket, int sec){
		struct timeval timev;
		timev.tv_sec = sec;
		timev.tv_usec = 0;
		setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &timev, sizeof(timev));
	}
public:
	TCPClient( char *host);

	virtual ~TCPClient(){
		close(client_socket);
	}
	bool get_msg( char*, size_t);
	bool send_msg( char*, size_t);

};

#endif
