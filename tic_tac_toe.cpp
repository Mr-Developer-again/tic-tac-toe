
#include "tic_tac_toe.h"

// initializing static data-members
uint2_t game::current_vertical_position = 0;
uint2_t game::current_horizon_position = 0;
uint2_t game::playing_counter = 0;
std::string game::winner = " ";


uint2_t game::create_random()
{
	srand(static_cast<uint2_t>(time(0)));

	return (rand() % 2);
}

void game::colorize()
{
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);     // for X
	init_pair(2, COLOR_BLUE, COLOR_BLACK);    // for O
	init_pair(3, COLOR_WHITE, COLOR_BLACK);	  // this color will be used in
											  // highlighting

	init_pair(4, COLOR_YELLOW, COLOR_BLACK);  // for turn section
	init_pair(5, COLOR_GREEN, COLOR_BLACK);   // print winner gamer
	init_pair(6, COLOR_MAGENTA, COLOR_BLACK); // print draw
}

// this method will initialize the array's indice to space
void game::initialize_char_array(std::array<std::array<std::string, 3>, 3> * char_in_pos)
{
	for (uint2_t i {0}; i < 3; i++)
	{
		for (uint2_t j {0}; j < 3; j++)
		{
			(*char_in_pos)[i][j] = " ";
		}
	}
}

void game::assign_to_array(const int* first_array, int* target_array)
{
	for (uint2_t i {0}; i < 3; i++)
	{
		target_array[i] = first_array[i];
	}
}

void game::initialize_game_board(WINDOW* game_board)
{
	box(this->game_board, 0, 0);


	bool check_y_end, check_x_end;

	// printing dot (.) in center of the board (in /3 of x and y)
	for (uint2_t i {(uint2_t)(this->x_max_board / 3)}; i < (this->x_max_board - 1); i += (this->x_max_board / 3))
	{
		for (uint2_t j {(uint2_t)(this->y_max_board / 3)}; j < (this->y_max_board - 1); j += (this->y_max_board / 3))
		{
			// to avoid printing in last section of x or y
			check_x_end = (i + (this->x_max_board / 3)) > this->x_max_board;
			check_y_end = (j + (this->y_max_board / 3)) > this->y_max_board;

			if (!(check_y_end || check_x_end))
				mvwprintw(this->game_board, j, i, ".");
		}
	}


	bool check_y_center;

	// printing pipe (|) in columns
	for (uint2_t i {(uint2_t)(this->x_max_board / 3)}; i < (this->x_max_board - 1); i += (this->x_max_board / 3))
	{
		for (uint2_t j {1}; j < (this->y_max_board - 1); j++)
		{
			check_y_center = (j == (this->y_max_board / 3)) || (j == (this->y_max_board / 3) * 2);

			if (!check_y_center)
				mvwprintw(this->game_board, j, i, "|");
		}
	}

	bool check_x_center;
	// printing dash (-) in rows
	for (uint2_t j {(uint2_t)(this->y_max_board / 3)}; j < (this->y_max_board - 1); j += (this->y_max_board / 3))
	{
		for (uint2_t i {1}; i < (this->x_max_board - 1); i++)
		{
			check_x_center = (i == (this->x_max_board / 3)) || (i == ((this->x_max_board / 3) * 2));

			if (!check_x_center)
				mvwprintw(this->game_board, j, i, "-");
		}
	}

	// highlighting first position
	wattron(this->game_board, A_REVERSE);
	mvwprintw(this->game_board, \
			  this->vertical_positions[this->current_vertical_position], \
			  this->horizon_positions[this->current_horizon_position], \
			  " ");
	
	wattroff(this->game_board, A_REVERSE);

	whose_turn();

	wrefresh(this->game_board);
}

game::game(WINDOW* game_win, const int input_vert_pos[], const int input_horz_pos[])
{
	this->game_board = game_win;

	// assign left array values to right array
	assign_to_array(input_vert_pos, this->vertical_positions);
	assign_to_array(input_horz_pos, this->horizon_positions);

	getmaxyx(this->game_board, this->y_max_board, this->x_max_board);

	curs_set(0);
	noecho();
	keypad(this->game_board, true);

	initialize_game_board(this->game_board);

	initialize_char_array(&(this->char_in_pos));
}

void game::print()
{
	colorize();

	// if the char is X
	if (this->char_in_pos[this->current_horizon_position][this->current_vertical_position] == "X")
	{
		wattron(this->game_board, A_BOLD | COLOR_PAIR(1));
		mvwprintw(this->game_board, this->vertical_positions[this->current_vertical_position], \
		          this->horizon_positions[this->current_horizon_position], \
		          this->char_in_pos[this->current_horizon_position][this->current_vertical_position].c_str());
		wattroff(this->game_board, A_BOLD | COLOR_PAIR(1));
	}
	// if the char is O
	else if(this->char_in_pos[this->current_horizon_position][this->current_vertical_position] == "O")
	{
		wattron(this->game_board, A_BOLD | COLOR_PAIR(2));
		mvwprintw(this->game_board, this->vertical_positions[this->current_vertical_position], \
				  this->horizon_positions[this->current_horizon_position], \
				  this->char_in_pos[this->current_horizon_position][this->current_vertical_position].c_str());
		wattroff(this->game_board, A_BOLD | COLOR_PAIR(2));
	}
	else // the character was a space
	{
		mvwprintw(this->game_board, this->vertical_positions[this->current_vertical_position], \
				  this->horizon_positions[this->current_horizon_position], \
				  this->char_in_pos[this->current_horizon_position][this->current_vertical_position].c_str());
	}
}

void game::reprint_or_highlight()
{
	colorize();

	if (!this->game_finished)
	{
		wattron(this->game_board, COLOR_PAIR(3) | A_BOLD | A_REVERSE);
		mvwprintw(this->game_board, this->vertical_positions[this->current_vertical_position], \
			  this->horizon_positions[this->current_horizon_position], \
			  char_in_pos[this->current_horizon_position][this->current_vertical_position].c_str());
		wattroff(this->game_board, COLOR_PAIR(3) | A_BOLD | A_REVERSE);
	}
	else
	{
		print();
	}
}

uint2_t game::movement()
{
	uint2_t pressed_key = wgetch(this->game_board);

	// reprint the highlighted section (to unhighlight that)
	// it means we want to clear highlight
	print();

	if (!this->game_finished)
	{	
		switch(pressed_key)
		{
			case KEY_UP:
			case 'w':
				move_up();
				break;

			case KEY_DOWN:
			case 's':
				move_down();
				break;

			case KEY_LEFT:
			case 'a':
				move_left();
				break;

			case KEY_RIGHT:
			case 'd':
				move_right();
				break;

			case ' ': // space key
				space_pressed();
				break;
			default:
				break;
		}
	}

	// to print and highlight the char in changed position
	reprint_or_highlight();

	return pressed_key;
}

void game::move_up()
{
	// position must be a number between 0 through 2
	if (this->current_vertical_position != 0)
		current_vertical_position--;
}

void game::move_down()
{
	// position must be a number between 0 through 2
	if (this->current_vertical_position != 2)
		this->current_vertical_position++;
}

void game::move_right()
{
	// position must be a number between 0 through 2
	if (this->current_horizon_position != 2)
		this->current_horizon_position++;
}

void game::move_left()
{
	// position must be a number between 0 through 2
	if (this->current_horizon_position != 0)
		this->current_horizon_position--;
}

void game::space_pressed()
{
	uint2_t response {0}; // this will capture win_or_what() method

	// if X_or_O is even, we must print X otherwise we must print O
	if (X_or_O % 2 == 0 && \
	    this->char_in_pos[this->current_horizon_position][this->current_vertical_position] == " ")
	{
		X_or_O++;
		this->char_in_pos[this->current_horizon_position][this->current_vertical_position] = "X";
		whose_turn();

		playing_counter++;

		if (playing_counter >= 5)
		{
			response = win_or_what();

			if (response == 0)
			{
				colorize();

				wattron(this->game_board, COLOR_PAIR(5) | A_BOLD);
				mvwprintw(this->game_board, this->y_max_board - 1, this->x_max_board / 2 - 6, "*** %s won ***", this->winner.c_str());
				wattroff(this->game_board, COLOR_PAIR(5) | A_BOLD);

				this->game_finished = true;
			}
			else if (response == 1)
			{
				colorize();

				wattron(this->game_board, COLOR_PAIR(6) | A_BOLD);
				mvwprintw(this->game_board, this->y_max_board - 1, this->x_max_board / 2 - 1, "DRAW");
				wattroff(this->game_board, COLOR_PAIR(6) | A_BOLD);

				this->game_finished = true;
			}
		}
	}
	else if (X_or_O % 2 == 1 && \
			 this->char_in_pos[this->current_horizon_position][this->current_vertical_position] == " ")
	{
		X_or_O++;
		this->char_in_pos[this->current_horizon_position][this->current_vertical_position] = "O";
		whose_turn();

		playing_counter++;

		if (playing_counter >= 5)
		{
			response = win_or_what();

			if (response == 0)
			{
				colorize();

				wattron(this->game_board, COLOR_PAIR(5) | A_BOLD);
				mvwprintw(this->game_board, this->y_max_board - 1, this->x_max_board / 2 - 6, "*** %s won ***", this->winner.c_str());
				wattroff(this->game_board, COLOR_PAIR(5) | A_BOLD);

				this->game_finished = true;
			}
			else if (response == 1)
			{
				colorize();

				wattron(this->game_board, COLOR_PAIR(6) | A_BOLD);
				mvwprintw(this->game_board, this->y_max_board - 1, this->x_max_board / 2 - 1, "DRAW");
				wattroff(this->game_board, COLOR_PAIR(6) | A_BOLD);

				this->game_finished = true;
			}
		}
	}
}

void game::whose_turn()
{
	colorize();

	wattron(this->game_board, COLOR_PAIR(4));
	if (X_or_O % 2 == 0) // this means it's X turn
		mvwprintw(this->game_board, this->turn_y_pos, this->turn_x_pos, \
				  "turn : X");

	else // this means it's O turn
		mvwprintw(this->game_board, this->turn_y_pos, this->turn_x_pos, \
				  "turn : O");

	wattroff(this->game_board, COLOR_PAIR(4));
}

uint2_t game::win_or_what()
{
	/*
	 *  NOTE: 
	 *  	return 0 -> means someone has won
	 *  	return 1 -> no one has won (draw)
	 *  	return 2 -> no win no draw
	 */


	/* for testing ->
	 *	  X X X
	 *    . . .
	 *    . . .
	 */ 
	if (
			(this->char_in_pos[0][0] == this->char_in_pos[1][0]) && \
			(this->char_in_pos[1][0] == this->char_in_pos[2][0]) && \
			((this->char_in_pos[2][0] == "X") || (this->char_in_pos[2][0] == "O"))
	   )
	{
		this->winner = this->char_in_pos[this->current_horizon_position][this->current_vertical_position];

		return 0;
	}

	/*
	 *	for testing ->
	 *	  . . .
	 *	  X X X
	 *	  . . .
	 */
	else if (
				(this->char_in_pos[0][1] == this->char_in_pos[1][1]) && \
				(this->char_in_pos[1][1] == this->char_in_pos[2][1]) && \
				((this->char_in_pos[2][1] == "X") || (this->char_in_pos[2][1] == "O"))
			)
	{
		this->winner = this->char_in_pos[this->current_horizon_position][this->current_vertical_position];

		return 0;
	}

	/* 
	 * for testing -> 
	 *    . . .
	 *    . . .
	 *    x x x
	 */
	else if (
				(this->char_in_pos[0][2] == this->char_in_pos[1][2]) && \
				(this->char_in_pos[1][2] == this->char_in_pos[2][2]) && \
				((this->char_in_pos[2][2] == "X") || (this->char_in_pos[2][2] == "O"))
			)
	{
		this->winner = this->char_in_pos[this->current_horizon_position][this->current_vertical_position];

		return 0;
	}

	/*
	 *	for testing ->
	 *	   X . .
	 *	   X . .
	 *	   X . .
	 */
	else if (
				(this->char_in_pos[0][0] == this->char_in_pos[0][1]) && \
				(this->char_in_pos[0][1] == this->char_in_pos[0][2]) && \
				((this->char_in_pos[0][2] == "X") || (this->char_in_pos[0][2] == "O"))
			)
	{
		this->winner = this->char_in_pos[this->current_horizon_position][this->current_vertical_position];

		return 0;
	}

	/*
	 * for testing ->
	 * 	 . X .
	 * 	 . X .
	 * 	 . X .
	 */
	else if (
				(this->char_in_pos[1][0] == this->char_in_pos[1][1]) && \
				(this->char_in_pos[1][1] == this->char_in_pos[1][2]) && \
				((this->char_in_pos[1][2] == "X") || (this->char_in_pos[1][2] == "O"))
			)
	{
		this->winner = this->char_in_pos[this->current_horizon_position][this->current_vertical_position];

		return 0;
	}

	/*
	 *	for testing ->
	 *	   . . X
	 *	   . . X
	 *	   . . X
	 */
	else if (
				(this->char_in_pos[2][0] == this->char_in_pos[2][1]) && \
				(this->char_in_pos[2][1] == this->char_in_pos[2][2]) && \
				((this->char_in_pos[2][2] == "X") || (this->char_in_pos[2][2] == "O"))
			)
	{
		this->winner = this->char_in_pos[this->current_horizon_position][this->current_vertical_position];

		return 0;
	}

	/*
	 *	for testing ->
	 *	   X . .
	 *	   . X .
	 *	   . . X
	 */
	else if (
				(this->char_in_pos[0][0] == this->char_in_pos[1][1]) && \
				(this->char_in_pos[1][1] == this->char_in_pos[2][2]) && \
				((this->char_in_pos[2][2] == "X") || (this->char_in_pos[2][2] == "O"))
			)
	{
		this->winner = this->char_in_pos[this->current_horizon_position][this->current_vertical_position];

		return 0;
	}

	/*
	 *	for testing ->
	 *	   . . X
	 *	   . X .
	 *	   X . .
	 */
	else if (
				(this->char_in_pos[2][0] == this->char_in_pos[1][1]) && \
				(this->char_in_pos[1][1] == this->char_in_pos[0][2]) && \
				((this->char_in_pos[0][2] == "X") || (this->char_in_pos[0][2] == "O"))
			)
	{
		this->winner = this->char_in_pos[this->current_horizon_position][this->current_vertical_position];

		return 0;
	}

	// draw
	else if (this->playing_counter == 9)
	{
		return 1;
	}

	return 2;
}

void game::display()
{
	wrefresh(this->game_board);
}
