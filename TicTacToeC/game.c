#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include <math.h>
#include <time.h>
const char SPACE = '-';
const char X_SYMBOL = 'X';
const char O_SYMBOL = 'O';

/*
	Robert Colan-O'Leary: 
*/

void play_game()
{
	printf("Xs and Os!\n\n");
	struct game* p_game_info = 0;
	//assign memory to p_game_info
	p_game_info = (struct game*)malloc(sizeof(struct game));
	
	//show the address to the user
	printf("Address of p_game_info = %p\n\n", &p_game_info);
	
	//two arrays to store both players names
	char firstArray[50];
	char secondArray[50];
	
	printf("\nEnter name of player 1:\n");
	scanf("%s",firstArray);
	printf("\nEnter name of player 2:\n");
	scanf("%s",secondArray);
	
	//assign addresses to pass to initialise_game
	char* firstName = firstArray;
	char* secondName = secondArray;
	
	//Start off game
	initialise_game(p_game_info, firstName, secondName);
	
	while(p_game_info->finished != True){
		//Outputs banner Game Status
		draw_banner();
	
		//Will detail players turn
		print_status(p_game_info);
	
		//Display current state of play
		display_board(p_game_info->board);
	
		//Gives the positions of each int value
		display_board_positions();
	
		//Validate move and update board
		process_move(p_game_info);
		
		//Checks after each move to see if there has been
		//a winner
		three_in_a_row(p_game_info);
		
	}
	
	free(p_game_info);
}

//initialise game with blank board and player names
//and starting player
void initialise_game(struct game* p_game_info, char* name1, char* name2){
	
	//fill each board place with a space
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			p_game_info->board[i][j] = SPACE;
		}
	}
	
	//copy both entered names into playerNames
	strncpy(p_game_info->playerNames[0], name1, MAX_NAME_LEN);
	strncpy(p_game_info->playerNames[1], name2, MAX_NAME_LEN);
	
	//call random number either 0 or 1 to decide who starts
	p_game_info->status = random_player_start();
	
	//print out player details and symbols associated with them
	printf("Player 1 => %s\tXs\n", p_game_info->playerNames[0]);
	printf("Player 2 => %s\tOs\n\n", p_game_info->playerNames[1]);
}

//Banner for game status
void draw_banner(){
	printf("\n");
	
	for(int i = 0; i < 15; i++){
		printf("-");
	}
	
	printf("\nGAME STATUS\n");
	
	for(int i = 0; i < 15; i++){
		printf("-");
	}
	
	printf("\n");
	
}

//display of current board
void display_board(char board[3][3]){
	
	printf("\n\n------------\n");
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			printf(" %c |", board[i][j]);
		}
		printf("\n------------\n");
	}
	
}

//Gives readout of players turns or winner
//based on the int value stored in status
int print_status(struct game* p_game_info){
	
	switch(p_game_info->status){
		case 0:
			p_game_info->finished = False;
			printf("\n%s's Turn\n", p_game_info->playerNames[0]);
			break;
		case 1:
			p_game_info->finished = False;
			printf("\n%s's Turn\n", p_game_info->playerNames[1]);
			break;
		case 2:
			p_game_info->finished = True;
			printf("\nWell done %s you have won!\n", p_game_info->playerNames[0]);
			display_board(p_game_info->board);
			break;
		case 3:
			p_game_info->finished = True;
			printf("\nWell done %s you have won!\n", p_game_info->playerNames[1]);
			display_board(p_game_info->board);
			break;
		default :
			p_game_info->finished = True;
      printf("\nGame over it's a draw!\n" );
      display_board(p_game_info->board);
	}
}

//Give users a view of the numbers on the board
//and the position they represent
void display_board_positions(){
	int count = 0;
	
	printf("\n\n------------\n");
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			printf(" %d |", count);
			count++;
		}
		printf("\n------------\n");
	}
}

//Find position on board for a move to take place
void get_row_col(int position, int* row, int* col){
	
	int count = 0;
	
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			if(count == position){
				*row = i;
				*col = j;
			}
				count++;
		}
	}
}

//Check if space is empty or already used
void process_move(struct game* p_game_info){
	int num = -1;
	int row = -1;
	int col = -1;
	char playerSymbol;
	
	//Symbol to be used based on game status
	if(p_game_info->status == 0){
		playerSymbol = X_SYMBOL;
	}
	else{
		playerSymbol = O_SYMBOL;
	}
	
	//If number entered is outside 0-8 keep asking
	while(num < 0 || num > 8){
		
		printf("\nEnter your choice from 0-8\n");
		scanf("%d", &num);
		
		//call function to determine position of value entered
		get_row_col(num, &row, &col);
	
		//if space does not have another char in it, place players Symbolthere
		//else output message and set num to outside range to loop through again
		if(p_game_info->board[row][col] == SPACE){
			p_game_info->board[row][col] = playerSymbol;
			break;
		}
		else{
			num = -1;
			printf("Place has already been used");
			display_board(p_game_info->board);
			display_board_positions();
		}
	}
}

//function to switch between players
void change_player(struct game* p_game_info){
	if(p_game_info->status == 0){
		p_game_info->status = 1;
	}
	else{
		p_game_info->status = 0;
	}
}

void three_in_a_row(struct game* p_game_info){
	/*
		check after each turn to see if the player has achieved a three in a row. 
		change player if no winner and space still available on the board, 
		if no spaces left declare a draw.
	*/
	char symbolCheck;
	//Look for Symbol of player that has just taken a turn
	if(p_game_info->status == 0){
		symbolCheck = X_SYMBOL;
	}
	else{
		symbolCheck = O_SYMBOL;
	}
	
	//Count Spaces left on the board
	int count = 0;
		for(int i = 0; i < 3; i++){
			for(int j = 0; j < 3; j++){
				if(p_game_info->board[i][j] == SPACE){
					count++;
				}
			}
		}
	
	//If else to check for a winner
	if(p_game_info->board[0][0] == symbolCheck && p_game_info->board[0][1] == symbolCheck && p_game_info->board[0][2] == symbolCheck){
		p_game_info->status = p_game_info->status + 2;
		print_status(p_game_info);
	}
	else if(p_game_info->board[1][0] == symbolCheck && p_game_info->board[1][1] == symbolCheck && p_game_info->board[1][2] == symbolCheck){
		p_game_info->status = p_game_info->status + 2;
		print_status(p_game_info);
	}
	else if(p_game_info->board[2][0] == symbolCheck && p_game_info->board[2][1] == symbolCheck && p_game_info->board[2][2] == symbolCheck){
		p_game_info->status = p_game_info->status + 2;
		print_status(p_game_info);
	}
	else if(p_game_info->board[0][0] == symbolCheck && p_game_info->board[1][0] == symbolCheck && p_game_info->board[2][0] == symbolCheck){
		p_game_info->status = p_game_info->status + 2;
		print_status(p_game_info);
	}
	else if(p_game_info->board[0][1] == symbolCheck && p_game_info->board[1][1] == symbolCheck && p_game_info->board[2][1] == symbolCheck){
		p_game_info->status = p_game_info->status + 2;
		print_status(p_game_info);
	}
	else if(p_game_info->board[0][2] == symbolCheck && p_game_info->board[1][2] == symbolCheck && p_game_info->board[2][2] == symbolCheck){
		p_game_info->status = p_game_info->status + 2;
		print_status(p_game_info);
	}
	else if(p_game_info->board[0][0] == symbolCheck && p_game_info->board[1][1] == symbolCheck && p_game_info->board[2][2] == symbolCheck){
		p_game_info->status = p_game_info->status + 2;
		print_status(p_game_info);
	}
	else if(p_game_info->board[0][2] == symbolCheck && p_game_info->board[1][1] == symbolCheck && p_game_info->board[2][0] == symbolCheck){
		p_game_info->status = p_game_info->status + 2;
		print_status(p_game_info);
	}
	else if(count == 0){
		p_game_info->status = 4;
		print_status(p_game_info);
	}
	else{
			change_player(p_game_info);
	}
}

//generate a random number either 0 or 1
int random_player_start(){
	srand(time(0));
	int r = rand() % 2;
	
	return r;
}