
#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <stdio.h>
#include <time.h>

#include "solve.h"

using namespace std;

// ***********************************************************

void shift_to_board(array<int, 81> &b, array<int, 9> &row, int start, int shift) {
	int i;

	for (i=0; i<9; i++) {
		b[start+i] = row[(i+shift) % 9];
	}
	return;
}

// ***********************************************************

int random(int lower, int upper) {
	return (rand() % (upper - lower + 1)) + lower;
}

// ***********************************************************
// On a fully complete board, randomly remove tiles until we
// have only 'left' number of tiles remaining.
//
int random_remove(array<int, 81> &b, int left) {
	vector<int> available;
	int i, r;
	
	// Create a vector of 0-80 available index numbers.
	for (i=0; i<81; i++) available.push_back(i);

	// Select (81-left) indexes from the avvailable vector, and set them to '0'. 
	srand(time(NULL));
	for (i=0; i<(81-left); i++) {
		r = random(0, available.size()-1);
		b[r] = 0;
		available.erase(available.begin() +  r);
	}

	return 0;
}

// ***********************************************************
// This method is described in this post:
//   https://gamedev.stackexchange.com/a/138228
//
void generate_board_method1() {
	std::array<int, 81> b;
	vector<int> available;
	array<int, 9> row;
	int i, r;

	std::cout << "Suduko Board Gen #1 (Shift Method)" << std::endl;

	b.fill(0);
	
	// Create a vector of 1-9 available numbers.
	for (i=1; i<10; i++) available.push_back(i);

	// Put the available numbers in a random order in an array. 
	srand(time(NULL));
	for (i=0; i<9; i++) {
		r = random(0, available.size()-1);
		row[i] = available[r];
		available.erase(available.begin() +  r);
	}


	array<int, 9> shift = {0, 3, 6, 7, 10, 13, 14, 17, 20};
	for (i=0; i<9; i++) {
		shift_to_board(b, row, (i*9), shift[i]);
	}

	show_board(b, -1);

	// Randomly remove tiles from the board.
	random_remove(b, 25);

	solve_singleboard(b, true);
}

// ***********************************************************
