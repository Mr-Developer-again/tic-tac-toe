#include <ncurses.h>
#include <string>
#include <array>
#include <ctime> // time()
#include <cstdlib> // rand() , srand()


typedef unsigned short uint2_t;


class game
{
private:
	WINDOW* game_board;

	uint2_t x_max_board, y_max_board;

	int vertical_positions[3];
	int horizon_positions[3];

	// characters that deployed in positions
	std::array<std::array<std::string, 3>, 3> char_in_pos;

	// initialize with space (all indice)
	void initialize_char_array(std::array<std::array<std::string, 3>, 3> *);

	void initialize_game_board(WINDOW*);

	// this method will assign values of a builtin array
	// to another
	void assign_to_array(const int*, int*);

	static uint2_t current_vertical_position;
	static uint2_t current_horizon_position;

	// private member functions for movement
	void move_up();
	void move_down();
	void move_right();
	void move_left();
	void space_pressed();

	bool X_is_on = false;
	bool O_is_on = false;

	uint2_t create_random(); // creates a random number between 0 and 1
	uint2_t X_or_O = create_random();

	void print();
	void reprint_or_highlight();

	// now is X turn or O turn
	void whose_turn();

	// position of turn section in game_board
	const static uint2_t turn_x_pos {1};
	const static uint2_t turn_y_pos {0};

	void colorize();

	static uint2_t playing_counter;

	uint2_t win_or_what();
	static std::string winner;
	bool game_finished {false}; // when the game will be finished that 
								// someone has won or the game has been drawed
public:
	game(WINDOW* game_win, const int vertical_positions[], const int horizon_positions[]);
	uint2_t movement();
	void display();
};
