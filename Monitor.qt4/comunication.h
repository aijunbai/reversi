#include <qthread.h> 
#include "board.h"
#include "TCPClient.h"

#define MSG_SIZE 256

class Comunication: public QThread{
		TCPClient *client;
		Board     *board;
		char      msg[MSG_SIZE];
	public:
		Comunication(Board *b = 0, TCPClient *c = 0){
			board  = b;
			client = c;
		}
		
		virtual ~Comunication(){
			if(client)
				delete client;
		}
		
		virtual void run(void);
};

