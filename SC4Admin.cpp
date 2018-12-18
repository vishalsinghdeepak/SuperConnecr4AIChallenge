#include <stdio.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
using namespace std;
#define CLEAR_ROW 1
#define CLEAR_COLUMN 2
#define CLEAR_FIRST_NEIGHBOURS 3
#define DUAL_COLOUR 4
#define NORMAL 5
#define PLAYER_1 1
#define PLAYER_2 2
/* global decalarations */
int turn = 0;/* this number will record whose turn is it */
int moves1=0,moves2=0;/* for tracking the number of moves pklayed yet by participants*/
int spmove11=0,spmove21=0,spmove31=0,spmove41=0,spmove12=0,spmove22=0,spmove32=0,spmove42=0;/* flags to keep track the special moves of players */
int move_type=0, move_new=0;
int board[7][6] = {0};

/* functions used */
int legal_move();
int add_disk();
int check_win(int team); int right_up(int x,int y); int left_up(int x,int y); int vertical(int x,int y); int horizontal(int x,int y); int write_board();
int draw_board(); int first_turn();
int board_full();
int call_code();
int move_counter();
int main() {
first_turn();/* decides first turn */
write_board();/* writes board to txt file */
draw_board();/* call graphics function to draw the board */ int match_over = 0;/* flag telling match is over or not */ while(1)/*starting the infinite loop*/
{
write_board();
match_over = board_full();/*checking if board is full*/ if(match_over==1)
break;

/*calling the participant code*/ match_over = call_code(); if(match_over == 1)
break;
else/* match_over == 0 *//* match is not over */ {
move_counter(); turn++; draw_board();
int check1 = check_win(1);/* checking winner */ int check2 = check_win(2);
if(check1 == 1 && check2 == 1)
{
printf("its a draw\n");
break; }
else if(check1 == 1) {
printf("player 1 wins\n");
break; }
else if(check2 == 1) {
printf("player 2 wins\n");
break; }

} }
return 0; }

int legal_move() {
if(move_new > 7 || move_new < 1)/*invalid column */
return 0;
else if(board[move_new-1][0] != 0)/* top most position of column is not empty*/ return 0;
if(turn%2 == 0)/* for player 2 */
{
if(move_type == CLEAR_ROW && spmove12 == 1)
return 0;
else if(move_type == CLEAR_COLUMN && spmove22 == 1)
return 0;
else if(move_type == CLEAR_FIRST_NEIGHBOURS && spmove32 == 1)
return 0;
else if(move_type == DUAL_COLOUR && spmove42 == 1)
return 0;
else if(move_type != DUAL_COLOUR && moves2 == 9 && spmove42 == 0)/* move type 4 means dual color block */
return 0; }
else {
if(move_type == CLEAR_ROW && spmove11 == 1)

return 0;
else if(move_type == CLEAR_COLUMN && spmove21 == 1)
return 0;
else if(move_type == CLEAR_FIRST_NEIGHBOURS && spmove31 == 1) return 0;
else if(move_type == DUAL_COLOUR && spmove41 == 1)
return 0;
else if(move_type != DUAL_COLOUR && moves1 == 9 && spmove41 == 0) return 0;
}
return 1; }

int add_disk() {
for(int y = 0;y<6;y++) {
if(board[move_new-1][y+1] != 0 || y+1>5)/* bottommost position in that column */ {
if(move_type == CLEAR_ROW)/* 1 means detroying complete row */ {
for(int x=0;x<7;x++) {
for(int temp = y;temp>=0;temp--) board[x][temp] = board[x][temp-1]; board[x][0] = 0;
}

}
else if(move_type == CLEAR_COLUMN)/* destroying complete column */ {
for(int temp = 0;temp<7;temp++)
board[move_new-1][temp] = 0; }
else if(move_type == CLEAR_FIRST_NEIGHBOURS)/* destroying neighbour block in every direction */
{
for(int temp = move_new-2;temp <= move_new;temp++) {
if(temp<0 || temp>6)
continue;
if(y+1<6)
board[temp][y+1]=0; board[temp][y]=0;
if(y-1>=0)
board[temp][y-1]=0;
/* now need to shift zeroes */ for(int count=0;count < 3;count++) {
for(int change = 5;change > 0;change--) {
if(board[temp][change]==0) {
board[temp][change] = board[temp][change-1];
board[temp][change-1] = 0; }

} }
} }
else if(move_type == DUAL_COLOUR)/* dual color block */ {
board[move_new-1][y] = 12;
}
else/* normal block */ {
if(turn % 2 == 1) board[move_new-1][y] = 1; else board[move_new-1][y] = 2;
}
break; }
}
return 0; }


int check_win(int team) {
int result=0,temp=0; for(int y=5;y>=0;y--) for(int x=0;x<7;x++) {

if(board[x][y] == 0)/* block is empty */ continue;
if(board[x][y] == team || board[x][y] == 12) {
result = right_up(x,y); if(result!=1)
result = left_up(x,y); if(result!=1)
result = vertical(x,y); if(result!=1)
result = horizontal(x,y);
}
if(result == 1) return 1;
}
return 0; }
int right_up(int x,int y) {
if(y<3 || x>3)/* not possible */
return 0;
else if(x-1>=0 && y+1<6 && (board[x-1][y+1] == board[x][y] || board[x-1][y+1] == 12)) return 0;/* condition was checked at x-1 and y+1 already */
else
{
int count=1;

for(int temp = 1;temp<4;temp++) {
if(board[x+temp][y-temp] == board[x][y] || board[x+temp][y-temp] == 12)
count++; }
if(count == 4)
return 1; }
return 0; }
int left_up(int x,int y) {
if(y<3 || x<3)/* not possible */
return 0;
else if(x+1<7 && y+1<6 && (board[x+1][y+1] == board[x][y] || board[x+1][y+1] == 12)) return 0;/* condition was checked at x+1 and y+1 already */
else
{
int count=1;
for(int temp = 1;temp<4;temp++) {
if(board[x-temp][y-temp] == board[x][y])
count++; }
if(count == 4) return 1;

}
return 0; }


int vertical(int x,int y) {
if(y<3)
return 0;
else if( y+1 < 6 && (board[x][y+1] == board[x][y] || board[x][y+1] == 12)) return 0;
int count = 1;
for(int temp=1;temp<4;temp++)
{
if(board[x][y-temp] == board[x][y] || board[x][y-temp] == 12 )
count++; }
if(count == 4) return 1; return 0;
}
int horizontal(int x,int y) {
if(x>3)
return 0;
else if( x-1>=0 && (board[x-1][y] == board[x][y] || board[x-1][y]==12)) return 0;

int count=1;
for(int temp=1;temp<4;temp++) {
if(board[x+temp][y] == board[x][y] || board[x+temp][y] == 12 )
count++; }
if(count == 4) return 1; return 0;
}
int write_board() {
ofstream pt ("board.txt"); for(int y = 5 ; y>=0 ; y--)
{
for(int x = 0 ; x < 7 ;x++) {
pt<<board[x][y]<<" "; }
} pt.close(); return 0;
}
int draw_board() {

printf("\n");
for(int y = 0 ; y<6;y++) {
for(int x = 0 ; x < 7 ; x++)
printf("%d ",board[x][y]);
printf("\n"); }
printf("\n\n");
return 0; }
int first_turn() {
srand ( time(NULL) );
/* initialization to generate a random number*/
/* deciding the first player whose turn is it */
ifstream pt("turn.txt");
/* opening the file in which the player who got the first turn is written */
if(pt == NULL)/* no such file exists*/
{
turn = rand()%2 + 1;
/* generating a random number to decide the first player to play */ ofstream ptr ("turn.txt");
ptr<<turn;
ptr.close();
}
else/* file exists */ {
pt>>turn; pt.close(); if(turn%2==0)
turn = 1; else turn = 2;
ofstream file ("turn.txt"); file<<turn;
file.close();
} }
int board_full() {
for(int y=0;y<6;y++) {
int empty=0;/* flag for board empty */ for(int x=0;x<7;x++)
{
if(board[x][y]==0)
{
empty = 1; break;
}
}

if(empty==1)
return 0; else/*board is full*/
{
printf("this match is a draw\n"); return 1;
}
} }

int call_code() {
for(int temp=0;temp<=3;temp++)/* at max 3 times */ {
remove("output.txt");/* deleting the output.txt file */
ofstream ptr ("moves.txt");/* writing moves played yet and powers used by player*/ ifstream abc ("turn.txt");
int record; abc>>record; if(record%2==1)
ptr<<(turn-1)/2<<" "; else
ptr<<turn/2-1<<" "; if(turn%2==0)
ptr<<spmove12<<" "<<spmove22<<" "<<spmove32<<" "<<spmove42; else
ptr<<spmove11<<" "<<spmove21<<" "<<spmove31<<" "<<spmove41; ptr.close();

ofstream file ("team_no.txt");/* creating the team_no.txt file */
if(turn%2==0) file<<"2"; else file<<"1"; file.close();
ofstream pt ("opponent_move.txt");
if(turn%2==0)
pt<<spmove11<<" "<<spmove21<<" "<<spmove31<<" "<<spmove41; else
pt<<spmove12<<" "<<spmove22<<" "<<spmove32<<" "<<spmove42; pt.close();
int legal = 1; if(temp == 3)
{
printf("game over\n"); if(turn%2 == 1) printf("player 2 wins\n"); else
printf("player 1 wins\n"); return 1;
}

pid_t pid;
pid = fork();/* to start a child process*/ if(pid == 0)/* child process */
{
// team codes will be called from here
// respective commands according to the language of the code will be used
// for eg to run c++ program named "abc.cpp" first copy the code in the same directory as this code then write
// system("g++ abc.cpp -o abc");
// system("./abc"); if(turn%2 == 1)
{
/*call first player code */
// write command (system function) here to call code
} else {
/* call second participant code*/
// write command (system function) here to call code
}
exit(0);/* exit the child process */
}
else
{/* main program sleeps for 2s*/ sleep(2);/* execution time limit */ kill(pid,SIGKILL);
}
ifstream out ("output.txt");/*reading output*/

if(out == NULL) continue;
else
{
out>>move_type; out>>move_new; out.close();
}
int check = legal_move();/* check if move is legal */
if(check == 0)/* move is illegal */
continue;
if(legal == 1)/* this means code reaches succesfully i.e. move is legal */ break;
}
return 0; }
int move_counter() {
add_disk(); if(turn%2 == 0) moves2++;
else
moves1++; if(move_type==1) {
if(turn%2==0)

spmove12++; else spmove11++;
}
else if(move_type==2) {
if(turn%2==0) spmove22++; else spmove21++;
}
else if(move_type==3) {
if(turn%2==0) spmove32++; else spmove31++;
}
else if(move_type==4) {
if(turn%2==0) spmove42++; else spmove41++;
} }
