#define MAX_NAME_LEN 50
enum Bool { False, True };
enum status { P1_TURN, P2_TURN, P1_WON, P2_WON, DRAW };
typedef enum Bool boolean;

/*
	Robert Colan-O'Leary: 
*/

struct game {
	char board[3][3];
	char playerNames[2][MAX_NAME_LEN];
	int status;
	boolean finished;
};

void play_game();

void initialise_game(struct game*, char* name1, char* name2);

void draw_banner();

void display_board(char board[3][3]);

int print_status(struct game*);

void display_board_positions();

void get_row_col(int position, int* row, int* col);

void process_move(struct game* game_info);

void change_player(struct game* game_info);

void three_in_a_row(struct game* game_info);

int random_player_start();