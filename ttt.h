//Source: https://gist.github.com/MatthewSteel/3158579
#ifndef TTT_H
#define TTT_H	

int board[9];

void draw(char CorS);
 
int win() ; 

int minimax(int player) ;
 
void computerMove(char str[]);

char gridChar(int i, char CorS);

void initBoard();

void randomStart(char str[]);


#endif

