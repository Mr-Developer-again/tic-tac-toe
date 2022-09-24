#include <ncurses.h>
#include <iostream>

#include "tic_tac_toe.h"

using namespace std;

typedef unsigned short uint2_t;

// calculating valid positions in game board
// the positions that user can move on them
void valid_positions(WINDOW*, int*, int*);


int main()
{
	int vertical_positions[3];
	int horizon_positions[3];

	uint2_t y, x;
	uint2_t y_max_board, x_max_board;

	// initializing ncurses screen
	initscr();

	getmaxyx(stdscr, y, x);

	y_max_board = (y / 4) + 4;
	x_max_board = (x / 4);

	WINDOW* game_board = newwin(y_max_board, x_max_board, (y / 4) + (y / 8), (x / 4) + (x / 8));

	valid_positions(game_board, vertical_positions, horizon_positions);

	game* game_obj = new game(game_board, vertical_positions, horizon_positions);

	while (game_obj->movement() != 'q')
	{
		game_obj->display();		
	}

	// destructor of ncurses screen
	endwin();
}


void valid_positions(WINDOW* board, int* vertical, int* horizon)
{
	uint2_t y_max_board, x_max_board;

	getmaxyx(board, y_max_board, x_max_board);

	box(board, 0, 0);

	// fill vertical valid positions
	for (uint2_t vert_position {2}, counter {0}; vert_position < (y_max_board - 1) && counter < 3; vert_position += 4 , counter++)
	{
		try
		{
			vertical[counter] = vert_position;
		}
		catch(out_of_range & ex)
		{
			cerr << ex.what() << endl;
		}
	}

	for (uint2_t horz_position {4}, counter {0}; horz_position < (x_max_board - 1) && counter < 3; horz_position += 8, counter++)
	{
		try
		{
			horizon[counter] = horz_position;
		}
		catch(out_of_range& ex)
		{
			cerr << ex.what() << endl;
		}
	}

	wrefresh(board);
}
