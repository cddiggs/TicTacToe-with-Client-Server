//Source: https://gist.github.com/MatthewSteel/3158579
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "ttt.h"


void draw(char CorS) {
    printf(" %c | %c | %c\n",gridChar(board[0],CorS),gridChar(board[1],CorS),gridChar(board[2],CorS));
    printf("---+---+---\n");
    printf(" %c | %c | %c\n",gridChar(board[3],CorS),gridChar(board[4],CorS),gridChar(board[5],CorS));
    printf("---+---+---\n");
    printf(" %c | %c | %c\n",gridChar(board[6],CorS),gridChar(board[7],CorS),gridChar(board[8],CorS));
}
 
int win() {
    //determines if a player has won, returns 0 otherwise.
    unsigned wins[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    int i;
    for(i = 0; i < 8; ++i) {
	if(board[wins[i][0]] != 0 &&
	   board[wins[i][0]] == board[wins[i][1]] &&
	   board[wins[i][0]] == board[wins[i][2]])
	    return board[wins[i][2]];
    }
    return 0;
}
 
int minimax(int player) {
    //How is the position like for player (their turn) on board?
    int winner = win();
    if(winner != 0) return winner*player;
 
    int move = -1;
    int score = -2;//Losing moves are preferred to no move
    int i;
    for(i = 0; i < 9; ++i) {//For all moves,
	if(board[i] == 0) {//If legal,
	    board[i] = player;//Try the move
	    int thisScore = -minimax(player*-1);
	    if(thisScore > score) {
	        score = thisScore;
	        move = i;
	    }//Pick the one that's worst for the opponent
	    board[i] = 0;//Reset board after try
	}
    }
    if(move == -1) return 0;
    return score;
}
 
void computerMove(char str[]) {
    int move = -1;
    int score = -2;
    int i;
    for(i = 0; i < 9; ++i) {
	if(board[i] == 0) {
	    board[i] = 1;
	    int tempScore = -minimax(-1);
	    board[i] = 0;
	    if(tempScore > score) {
	        score = tempScore;
	        move = i;
	    }
	}
    }
    //returns a score based on minimax tree at a given node.
    board[move] = 1;
    sprintf(str, "%d", move);
}
 

void randomStart(char str[]) {
    srand(time(NULL));
     int r = rand();
	int move = 2*(r%4);
    //returns an even random number 0-8
    board[move] = 1;
    sprintf(str, "%d", move);
}
 

char gridChar(int i, char CorS) {
    if (CorS=='S'){
	    switch(i) {
		case -1:
		    return 'X';
		case 0:
		    return ' ';
		case 1:
		    return 'O';
	    }
   }
   else {
	    switch(i) {
		case -1:
		    return 'O';
		case 0:
		    return ' ';
		case 1:
		    return 'X';
	    }
   }
}


void initBoard(){
int i;
	for(i=0; i<9; i++){
		board[i]=0;
	}
}
