/* A simple server in the internet domain using TCP
   The port number is passed as an argument 

see http://www.linuxhowtos.org/C_C++/socket.htm 

gcc -o server.exe server.c

portno=port number (you can take a big number like 50000)

./server.exe portno
*/

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "ttt.h"

#include <stdlib.h>
#include <string.h>


void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n, moveC;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno =atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     /* ready ....*/




     while(1){
		  printf("\n10 Rounds of TicTacToe Begins Here!\n\n");
		  unsigned roundcount;
		  int playerwins=0, computerwins=0, drawgames=0;
		  printf("We are: O, They Are: X\nThey Go First, Then we Alternate from there.\nLets Get Started! ");
		  for(roundcount=0;roundcount<10;roundcount++){
			  printf("\nNew Game Starting\n");
			  initBoard();
			  unsigned turn;
			  turn = 0;
			  for(turn = 0; turn < 9 && win(board) == 0; ++turn) {
				  if((turn+roundcount) % 2 == 0){//A way to determine who goes first
					  /*listening ...*/
					  bzero(buffer,256);
					  n = read(newsockfd,buffer,255);
					  if (n < 0) error("ERROR reading from socket");
				 	  printf("\nClient's move was: %s\n",buffer);
				 	  sscanf(buffer,"%d", &moveC);
				 	  board[moveC] = -1;
				 	   draw('S');
				  }  else{
					  /*writing...*/
					  char move[15];
					  if (turn==0)
						  randomStart(move);
					   else 
						  computerMove(move);
					  
					  printf("Please Enter your Next Move: %s", move);

					  bzero(buffer,256);
					  fgets(buffer,255,stdin);
				
					  n = write(newsockfd,move,strlen(move));
					  if (n < 0) error("ERROR writing to socket");
					  draw('S');
				  }
			  }
			  switch(win(board)) {
					case 0:
						drawgames++;
						printf("A draw... Total Draw Count is now: %d\n", drawgames);
						break;
					case 1:
						draw('S');
						computerwins++;
						printf("They lost!... Total Win Count for us is now: %d\n", computerwins);
						break;
					case -1:
						playerwins++;
						printf("Oh No, They Won!... Their Win Count is now: %d\n", playerwins);
						break;
			  }




		  }

		  printf("\n\nFinal Score:\n=============\n");
		  printf("Their Wins: %d, Our Wins: %d, Draw Games: %d\n\n", playerwins, computerwins, drawgames);
		  break;
     }
     return 0; 
}
