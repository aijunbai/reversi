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


#include "TCPServer.h"

#define _REENTRANT
#define RECV_TIMEWAIT 180

/**
	following is all the macros to communicate with clients
	both agent and monitor, your client must conform with
	this agreement, or else there may be something happend
	unexpectly.
*/

#define ACTION		(char)70
#define WAIT		(char)71
#define BOARD_DESCRIPTION (char)69

#define YOU_WIN 	(char)74
#define YOU_LOSE	(char)75
#define YOU_TIE  	(char)77

#define BLACK_WIN 	(char)81
#define WHITE_WIN 	(char)80
static const char WIN[] = {'\0', '\0', BLACK_WIN, WHITE_WIN};

#define CH_EMPTY (char)1
#define CH_BLACK (char)2
#define CH_WHITE (char)3

#define RL_BLACK 	CH_BLACK
#define RL_WHITE 	CH_WHITE
#define RL_MONITOR     	(char)4

#define ONE_MORE_TIME	(char)72
#define TWO_TIMES_LEFT	(char)73
#define THREE_TIMES_LEFT (char)76
static const char TIMES_LEFT[] = {'\0', ONE_MORE_TIME, TWO_TIMES_LEFT, THREE_TIMES_LEFT};

static bool stop = false;
static int turn_index = RL_BLACK;		///by default Black is the first one to place chess

static pthread_mutex_t 	mutex_update = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  	tell_to_update = PTHREAD_COND_INITIALIZER;

inline static bool is_my_turn(int my_no)
{
	return turn_index == my_no;
}

inline static int the_other(int my_no)
{
	return (my_no == RL_BLACK)? RL_WHITE: (my_no == RL_WHITE)? RL_BLACK: RL_MONITOR;
}

inline static void turn_another()
{
	turn_index = the_other(turn_index);
}

static void *client_thread_start_routine(void *v)
{
	TCPServer *server = (TCPServer *)v;
	server->comunication();
	pthread_exit(NULL);
	return (void*)NULL;
}

int wait_times = 0;			///this is a flag to know wether contest is to be finished,
					///when wait_times == 2
					///it means no one has the ability to put chess any way,
					///so, I can tell the winner from the loser now.

int un_updated = 0;			///a flag works like a sem

TCPServer::TCPServer( Board *b, int call_port){
	assert(b);
	board = b;
	clients_n = 0;
	port = call_port;
	if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		perror("create socket"), exit(1);

	bzero(&server_addr, sizeof(sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port   = htons(port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if((bind(server_socket, (struct sockaddr *)&server_addr, sizeof(sockaddr))) < 0)
		perror("bind to port"),	exit(1);

	if((listen(server_socket, DEFAULT_QUEUE_SIZE)) < 0)
		perror("listern to port"), exit(1);

	while(!stop){
		socklen_t  client_addr_length = sizeof(client_addr);
		client_socket = accept(server_socket, (struct sockaddr *)&client_addr,\
				&client_addr_length);		//may block here

		if(client_socket < 0)
			perror("accept from client"), exit(1);
		time_limit(client_socket, RECV_TIMEWAIT);
		clients_n++;
		if( pthread_create( &client_thread_id, NULL,\
				  client_thread_start_routine, (void *)this) != 0)
			perror("Initialize thread"), exit(2);
		pthread_detach(client_thread_id);
		usleep(200000);	//to enable the newest client-server-comunication be well ready 
	}
}

void TCPServer::comunication()
{
	char message[BUFFER_SIZE];

	//to establish the fully infomation of the newest accpected client
	client_ds *this_client = new client_ds(client_socket, inet_ntoa(client_addr.sin_addr),\
					(int)ntohs(client_addr.sin_port), \
					(clients_n == 1)? RL_BLACK: \
					(clients_n == 2)? RL_WHITE: RL_MONITOR, \
					client_thread_id);
	printf("<<<< <connection from client, IP: %s, Port: %d, Role: %s>\n", \
			this_client->IP, ntohs(client_addr.sin_port), \
			(this_client->role == RL_BLACK)? "Agent_Black": \
			(this_client->role == RL_WHITE)? "Agent_White":"Monitor" );


	if( !authentication(this_client->socket)){
		fprintf(stderr, "illegal invasion\n");
		close(this_client->socket);
		delete this_client;
		return;
	}
	send_msg( this_client->socket, message, this_client->role);
	usleep(400000);
	send_msg( this_client->socket, message, board->layout);

	while(!stop)
		main_loop(this_client->role, this_client->socket, message);
	
	send_rslt(this_client->socket, this_client->role, board->winner());

	close(this_client->socket);
	delete this_client;
	
	if( !(--clients_n)){
		pthread_mutex_destroy( &mutex_update);
		pthread_cond_destroy(  &tell_to_update);
		close(server_socket);
		printf(">>>> Contest finished - ");
		switch(board->winner()){
			case RL_BLACK: printf("winner is BLACK\n"); break;
			case RL_WHITE: printf("winner is WHITE\n"); break;
			case '\0': printf("winner is NONE\n"); break;
		}
		exit(0);
	}
}

inline void TCPServer::main_loop(char my_no, int socket, char *const msg)
{
	if( is_my_turn(my_no)){					//it's my turn, now. < never turn to a monitor, but>
		do{
			sleep(1);					//wait to let client be ready
		}while(un_updated);

		if( board->able_to_put(my_no) && !stop){   	//no need to wait
			wait_times = 0;
			send_msg(socket, msg, ACTION);
			recv_msg(socket, msg, BUFFER_SIZE);

			int times = 3;
			while( !board->update(my_no, msg)){	//maybe some mistakes made by client
				printf(">>>> %s made a mistake\n", (my_no == RL_BLACK)? "Black": "White");
				if(times == 0){
					stop = true;		//the limit is three times
					break;
				}
				send_msg( socket, msg, TIMES_LEFT[times--]);
				recv_msg(socket, msg, BUFFER_SIZE);
			}
			send_msg( socket, msg, board->layout);

		} else{						//can not put any way, so wait for one more time
			printf(">>>> %s waits for one time\n", (my_no == RL_BLACK)? "Black": "White");
			if(++wait_times >= 2)
				stop = true;
			else 
				send_msg( socket, msg, WAIT);
		}
		turn_another();					//It's turn of the other one
		pthread_cond_broadcast( &tell_to_update);   	//brodcast all to update

	} else{							//wait for signal of update, now
		pthread_mutex_lock(&mutex_update);
		pthread_cond_wait( &tell_to_update, &mutex_update);
		un_updated++;
		send_msg( socket, msg, board->layout);
		pthread_mutex_unlock( &mutex_update);
		un_updated--;
	}
}

bool TCPServer::authentication( int socket)
{
	unsigned char key0, key1;
	const pthread_t a = pthread_self();
	key0 = rand_r((unsigned *)&a) % (255) + 1;

	if((send(socket, &key0, sizeof(key0), 0)) < 0)
		perror("send key to client"), pthread_exit(NULL);
	if((recv(socket, &key1, BUFFER_SIZE, 0)) < 0)
		perror("reveive key from client"), pthread_exit(NULL);

	return key1 == key0? true: false;
}

inline void TCPServer::recv_msg( int socket, char *msg, size_t size)
{
	if((recv(socket, msg, size, 0)) < 0)
		perror("reveive from client"), pthread_exit(NULL);
}

inline void TCPServer::send_msg( int socket, char *msg, char c)
{
	char *p = msg;
	*p++ = c;
	*p = '\0';
	if((send(socket, msg, strlen(msg), 0)) < 0)
		perror("send to client"), pthread_exit(NULL);
}

inline void TCPServer::send_msg( int socket, char *msg, char *s)
{
	strcpy(msg, s);
	if((send(socket, msg, strlen(msg), 0)) < 0)
		perror("send to client"), pthread_exit(NULL);
}

inline	void TCPServer::send_rslt(int socket, char my_no, char winner)
{
	char msg[3];
	usleep(200000);					//wait to let client be ready
	if( winner == my_no)
		send_msg( socket, msg, YOU_WIN);
	else if( winner == the_other(my_no))
		send_msg( socket, msg, YOU_LOSE);
	else if(winner == '\0')
		send_msg( socket, msg, YOU_TIE);
	else
		send_msg( socket, msg, WIN[(int)winner]);  //this is for monitor to know who's winner
}

