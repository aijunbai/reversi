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

#include "TCPClient.h"
#define RECV_TIMEWAIT 180
 
 
TCPClient::TCPClient( char *host){
		
	host_name = host;

	if((server_host = ::gethostbyname(host_name)) == 0)
		perror("get host by name"), exit(1);

	if((client_socket = ::socket(AF_INET, SOCK_STREAM, 0)) < 0)
		perror("create socket"), exit(1);
	
	time_limit(client_socket, RECV_TIMEWAIT);
	
	bzero(&server_addr,sizeof(sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = ((struct in_addr *)(server_host->h_addr))->s_addr;
	server_addr.sin_port = htons(SERVER_PORT);
	
	if(::connect(client_socket, (struct sockaddr *)&server_addr, sizeof(sockaddr)) < 0)
		perror("connect to server"), exit(1);
	
	char buff[BUFFER_SIZE];
	bzero(buff, sizeof(buff));
	if((recv(client_socket, buff, BUFFER_SIZE, 0)) < 0)
		perror("receive from server"), exit(1);
	
	printf(">>>> key <%d> got\n", int(*buff));

	if((send(client_socket, buff, strlen(buff), 0)) < 0)
		perror("send to server"), exit(1);
	
	printf(">>>> key <%d> sended\n", int(*buff));
}


bool TCPClient::get_msg( char *msg, size_t size)
{
	bzero(msg, size);
	if((recv(client_socket, msg, size, 0)) < 0){
		if(errno == 11)	
			return false;
		else
			perror("receive from server"), exit(1);
	}
	if(!strlen(msg))
		return false;
	return true;
}

bool TCPClient::send_msg( char *msg, size_t size)
{
	assert(strlen(msg) <= size);
	if((send(client_socket, msg, strlen(msg), 0)) < 0)
		perror("send to server"), exit(1);
	return true;
}
