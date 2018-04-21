#pragma once
#include <ostream>
#include "gcc_compatible.h"
#define BLACK 2
#define WHITE 1
typedef void (*function)(void);
struct pointrank {
	int x;
	int y;
	int origin;
	int combo;
	int tolerate;
	int score;
	bool secured;
	pointrank(int x, int y, int origin, int combo, int tolerate, int score, bool secured);
	pointrank();
};
class gobang {
private:
	int rank_black;
	int rank_white;
	int width;
	int height;
	int last_b_x;
	int last_b_y;
	int last_w_x;
	int last_w_y;
	int turns;
	int max_turns;
	bool grid_line;
	int** board;
	bool verify_point(int x, int y);
	pointrank combo(int origin, int x, int y, int dir_x, int dir_y, int limit, int tolerance);
public:
	gobang(int width, int height);
	~gobang();
	bool canplace(int x, int y);
	pointrank seek_point_for(int origin);
	int opposite(int origin);
	int score_for_point(int x, int y, int origin);
	void disable_grid_line();
	bool place(int x, int y, int origin);
	void undo();
	int the_winner_is();
	void output(std::ostream& cout, function cls);
	void computer_turn(int origin);
};
