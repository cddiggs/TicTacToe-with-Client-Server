/*gcc -o client.exe client.c

portno=port number (you can take a big number like 50000)
ipaddress=Internet Protocol Address (IP) of the server (at the server, issue 'ifconfig'.  
Look for 'inet addr' under 'eth0' etc. It should be something like 192.168.11.2


./client.exe ipaddress protno


Call ./server.exe first
Then call ./client.exe next (Port Number and Server's IP address is needed)
Client will make the first move for the first game and every alternate game there after. 
Positions on the TicTacToe board are labeled 0 through 8.
A move is made by entering the position on the TicTacToe board.
(Remember, this will automatically make the move based on the Mini-Max algorithm provided by MathewSteel.
Press return key (from client side and server side) to make the move.
*/


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "ttt.h"

#include <stdlib.h>
#include <string.h>

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n, move;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
   server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    while(1){
	printf("\n10 Rounds of TicTacToe Begins Here!\n\n");
			  unsigned roundcount;
			  int playerwins=0, computerwins=0, drawgames=0;
			  printf("We are: X, They Are: O\nWe Go First, Then we Alternate from there.\nLets Get Started! ");
			  for(roundcount=0;roundcount<10;roundcount++){
				  printf("\nNew Game Starting\n");
				  initBoard();
				  unsigned turn;
				  turn = 0;
				  for(turn = 0; turn < 9 && win(board) == 0; ++turn) {
					  if((turn+roundcount) % 2 == 0){
						/*ready for inputs*/
						  char move[15];
						  if (turn==0)
							  randomStart(move);

						  else 
							  computerMove(move);
						  printf("Please enter your next position: %s", move);

						  bzero(buffer,256);
						  fgets(buffer,255,stdin);
					
						  /*writing  ...*/
						  n = write(sockfd,move,strlen(move));
						  if (n < 0) error("ERROR writing to socket");
						  draw('C');
	
					  }

					  else{
						  /*listen ...*/
						  bzero(buffer,256);
						  n = read(sockfd,buffer,255);
						  if (n < 0) error("ERROR reading from socket");
						   sscanf(buffer, "%d", &move);
						   printf("\nServer's move: %d\n", move);
						   board[move] = -1;
						   draw('C');
					  }
				  }
				  switch(win(board)) {
						case 0:
							drawgames++;
							printf("A draw... Total Draw Count is now: %d\n", drawgames);
							break;
						case 1:
							draw('C');
							computerwins++;
							printf("Oh no, they won!... Total Win Count for them is now: %d\n", computerwins);
							break;
						case -1:
							playerwins++;
							printf("They Lost!... Our Win Count is now: %d\n", playerwins);
							break;
				  }
			  }

			  printf("\n\nFinal Score:\n=============\n");
			  printf("Our Wins: %d, Their Wins: %d, Draw Games: %d\n\n", playerwins, computerwins, drawgames);
			  break;


 
    }
    return 0;
}
