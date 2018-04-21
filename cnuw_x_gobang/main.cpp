#include <iostream>
#include <ctime>
#include <math.h>
#include "gobang.h"
using namespace std;
void cls() {
#if defined(WIN32) || defined(_WIN32) || defined(WINDOWS)
	system("cls");
#else
	system("clear");
#endif
}
int main() {
	while (true) {
		cls();
		int mode, width, height, winner;
		cout << "Gobang Game by EGGTARTc\n"
			<< "[1] PVP\n"
			<< "[2] PVE\n"
			<< "[3] EVE\n\n"
			<< "Enter preferred mode: ";
		cin >> mode;
		cout << "\nEnter  width of board(usually 19): ";
		cin >> width;
		cout << "Enter height of board(usually 19): ";
		cin >> height;
		gobang game(width, height);
		if (mode == 3) {
			cls();
			cout << "\n**** WELCOME TO THE CRAZIEST PART OF THIS PROGRAM! ****";
			cout << "\n\n\t*** READY? ***\n\n";
			system("pause");
			srand(time(NULL));
			float r1 = rand() / (RAND_MAX + 1.0);
			float r2 = rand() / (RAND_MAX + 1.0);
			game.place((int)width*r1, (int)height*r2, BLACK);
			while (true) {
				int x = -2, y = -2;
				game.output(cout, cls);
				game.computer_turn(WHITE);
				if ((winner = game.the_winner_is()) != 0) break;
				game.computer_turn(BLACK);
				if ((winner = game.the_winner_is()) != 0) break;
			}
			game.output(cout, cls);
			if (winner == BLACK)
				cout << "\n--- BLACK WINS! ---\n";
			else if (winner == WHITE)
				cout << "\n--- WHITE WINS! ---\n";
			else
				cout << "\n--- The game ends in a draw! ---\n";
		}
		else if (mode == 2) {
			while (true) {
				int x = -2, y = -2;
				game.output(cout, cls);
				cout << "\n *Tips: You can enter \"-1 -1\" to take a back move.\n";
				cout << " \tEnter \"-1 -2\" to disable grid line.\n";
				while (!game.canplace(y - 1, x - 1)) {
					cout << "\n Your turn.\n"
						<< " X: ";
					cin >> x;
					cout << " Y: ";
					cin >> y;
					if (x == -1 && y == -1) break;
					if (x == -1 && y == -2) {
						game.disable_grid_line();
						game.output(cout,cls);
					}
				}
				if (x == -1 && y == -1) {
					game.undo();
				}
				else {
					game.place(y - 1, x - 1, BLACK);
					if ((winner = game.the_winner_is()) != 0) break;
					game.computer_turn(WHITE);
					if ((winner = game.the_winner_is()) != 0) break;
				}
			}
			game.output(cout, cls);
			if (winner == BLACK)
				cout << "\n--- HUMAN WINS! ---\n";
			else if(winner == WHITE)
				cout << "\n--- COMPUTER WINS! ---\n";
			else
				cout << "\n--- The game ends in a draw! ---\n";
		}
		else if (mode == 1) {
			bool turn = false;
			while (true) {
				int x = -2, y = -2;
				game.output(cout, cls);
				cout << "\n *Tips: You can enter \"-1 -1\" to take a back move.\n";
				cout << " \tEnter \"-1 -2\" to disable grid line.\n";
				while (!game.canplace(y - 1, x - 1)) {
					if (turn=!turn) 
						cout << "\n Black's turn.\n";
					else 
						cout << "\n White's turn.\n";
					cout  << " X: ";
					cin >> x;
					cout << " Y: ";
					cin >> y;
					if (x == -1 && y == -1) break;
					if (x == -1 && y == -2) {
						game.disable_grid_line();
						game.output(cout, cls);
					}
				}
				if (x == -1 && y == -1) {
					game.undo();
				}
				else {
					game.place(y - 1, x - 1, turn?BLACK:WHITE);
					if ((winner = game.the_winner_is()) != 0) break;
				}
			}
			game.output(cout, cls);
			if (winner == BLACK)
				cout << "\n--- BLACK WINS! ---\n";
			else if(winner == WHITE)
				cout << "\n--- WHITE WINS! ---\n";
			else
				cout << "\n--- The game ends in a draw! ---\n";
		}
		system("pause");
	}
	return 0;
}