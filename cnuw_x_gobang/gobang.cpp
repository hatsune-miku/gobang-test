#include "gobang.h"

pointrank::pointrank(int x, int y, int origin, int combo, int tolerate, int score, bool secured) {
	this->x = x;
	this->y = y;
	this->origin = origin;
	this->combo = combo;
	this->tolerate = tolerate;
	this->score = score;
	this->secured = secured;
}
pointrank::pointrank() {
	this->x = 0;
	this->y = 0;
	this->origin = 0;
	this->combo = 0;
	this->tolerate = 0;
	this->score = 0;
	this->secured = false;
}

gobang::gobang(int width, int height) {
	this->width = width;
	this->height = height;
	this->rank_black = 0;
	this->rank_white = 0;
	this->last_b_x = 0;
	this->last_b_y = 0;
	this->last_w_x = 0;
	this->last_w_y = 0;
	this->grid_line = true;
	this->max_turns = width * height;
	board = new int*[height];
	for (int i = 0; i < height; i++) {
		board[i] = new int[width];
		for (int j = 0; j < width; j++)
			board[i][j] = 0;
	}
}
gobang::~gobang() {
	for (int i = 0; i < height; i++) {
		delete[] board[i];
		board[i] = 0;
	}
	delete[] board;
	board = 0;
}
bool gobang::canplace(int x, int y) {
	if (!verify_point(x, y)) return false;
	return (board[x][y] == 0);
}
bool gobang::place(int x, int y, int origin) {
	if (!canplace(x, y)) return false;
	board[x][y] = origin;
	if (origin == BLACK) {
		last_b_x = x;
		last_b_y = y;
	}
	else {
		last_w_x = x;
		last_w_y = y;
	}
	turns++;
}

void gobang::undo() {
	board[last_b_x][last_b_y] = 0;
	board[last_w_x][last_w_y] = 0;
	turns -= 2;
}

bool gobang::verify_point(int x, int y) {
	return (0 <= x && x < width && 0 <= y && y < height);
}
int gobang::opposite(int origin) {
	if (origin == BLACK) return WHITE;
	return BLACK;
}
pointrank gobang::combo(int origin, int x, int y, int dir_x, int dir_y, int limit, int tolerance) {
	pointrank pr(x,y,origin,1,0,0,false);
	if (origin == 0)
		origin = board[x][y];
	pr.origin = origin;
	int step = 0;
	while (verify_point(x+=dir_x, y+=dir_y) && (step++ < limit)) {
		if (board[x][y] == pr.origin)
			pr.combo++;
		else if (board[x][y] == opposite(pr.origin)) {
			pr.secured = true;
			return pr;
		}
		else if (++pr.tolerate >= tolerance) {
			if (step <= 1) pr.combo = 0;
			pr.secured = true;
			return pr;
		}
	}
	return pr;
}
int gobang::the_winner_is() {
	if (turns >= max_turns) return -3;
	int origin = 0;
	int dir[8][2] = { {-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1} };
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			for (int k = 0; k < 8; k++) {
				if (board[i][j] != 0) {
					pointrank pr = combo(0, i, j, dir[k][0], dir[k][1], 5, 0);
					if (pr.combo >= 5)
						return pr.origin;
				}
			}
		}
	}
	return 0;
}

int gobang::score_for_point(int x, int y, int origin) {
	int dir[8][2] = { { -1,0 },{ -1,1 },{ 0,1 },{ 1,1 },{ 1,0 },{ 1,-1 },{ 0,-1 },{ -1,-1 } };
	int score = 0;
	for (int k = 0; k < 8; k++) {
		pointrank pr = combo(origin, x, y, dir[k][0], dir[k][1], 6, 1);
		if (pr.tolerate == 0) {
			if (pr.secured) {
				if (pr.combo >= 5) 
					return 10000;
				else if (pr.combo == 4) score += 800;
				else if (pr.combo == 3) score += 30;
				else if (pr.combo == 2) score += 3;
				else score += 1;
			}
			else {
				if (pr.combo >= 5) 
					return 10000;
				else if (pr.combo == 4) score += 1400;
				else if (pr.combo == 3) score += 100;
				else if (pr.combo == 2) score += 6;
				else score += 1;
			}
		}
		else {
			if (pr.secured) {
				if (pr.combo >= 5) 
					return 5000;
				else if (pr.combo == 4) score += 700;
				else if (pr.combo == 3) score += 30;
				else if (pr.combo == 2) score += 2;
				else score += 1;
			}
			else {
				if (pr.combo >= 5) 
					return 5000;
				else if (pr.combo == 4) score += 1200;
				else if (pr.combo == 3) score += 90;
				else if (pr.combo == 2) score += 4;
				else score += 1;
			}
		}
	}
	return score;
}

pointrank gobang::seek_point_for(int origin) {
	pointrank best;
	int score = INT_MIN;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (canplace(i, j)) {
				int s = score_for_point(i, j, origin);
				if (s > score) {
					best.x = i;
					best.y = j;
					best.score = s;
					score = s;
				}
			}
		}
	}
	return best;
}

void gobang::computer_turn(int origin) {
	pointrank pr_b = seek_point_for(BLACK);
	pointrank pr_w = seek_point_for(WHITE);
	if (origin == BLACK) {
		if (pr_b.score > pr_w.score - turns)
			place(pr_b.x, pr_b.y, origin);
		else
			place(pr_w.x, pr_w.y, origin);
	}
	else {
		if (pr_w.score > pr_b.score - turns)
			place(pr_w.x, pr_w.y, origin);
		else
			place(pr_b.x, pr_b.y, origin);
	}
}

void gobang::disable_grid_line() {
	grid_line = !grid_line;
}

void gobang::output(std::ostream& cout, function cls) {
	cls();
	cout << " *";
	for (int i = 1; i <= width; i++)
		if(i==9) printf("%3d ", i);
		else printf("%3d", i);
	cout << std::endl;
	for (int i = 0; i < height; i++) {
		printf("%2d  ", i + 1);
		for (int j = 0; j < width; j++)
			if (board[i][j] == 0)
				if(grid_line)
					cout << "©à ";//¡¡//
				else
					cout << "¡¡ ";//¡¡//
			else if (board[i][j] == BLACK)
				cout << "¡ð ";
			else
				cout << "¡ñ ";
		cout << std::endl;
	}
}