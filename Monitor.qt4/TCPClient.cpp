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

	if((send(client_socket, buff, strlen(buff), 0)) < 0)
		perror("send to server"), exit(1);
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
