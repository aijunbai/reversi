#include "comunication.h" 
 
#define NEED_TO_SEND 	(char)255
#define NO_NEED_TO_SEND (char)254
#define FINISHED 	(char)253
 
void Comunication::run(void)
{
	while( client->get_msg( msg, MSG_SIZE)){
		switch( board->do_msg( msg, MSG_SIZE)){
			case NEED_TO_SEND:	client->send_msg( msg, MSG_SIZE);break;
			case NO_NEED_TO_SEND:	break;
			case FINISHED:		return;
		}
	}
}
