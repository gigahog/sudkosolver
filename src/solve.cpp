#include <iostream>
#include <array>
#include <string>
#include <stdio.h>
#include <time.h>

using namespace std;

#define BOARD(a,b)	(((b)*9)+(a)) 
#define XX(i)       ((i) % 9)
#define YY(i)       ((i) / 9)
#define M(j)  		(((j) / 3) * 3)

int found;		// Count of number of solutions for a particular board.

//*******************************************************

void show_board(const std::array<int,81> &o, int z) {
	for(int j=0; j<9; j++) {
		for(int i=0; i<9; i++) {
			if (z == BOARD(i,j))
				printf("\033[31m");
			printf("%i,", o[BOARD(i,j)] );
			printf("\033[0m");
			if (i % 3 == 2) std::cout << " ";
		}
		cout << endl;
	}
	cout << endl;
}

//*******************************************************

array<int,9> current_values(const std::array<int,81> &o, int i) {
	int a, b, c, d,x, y, val; 
	array <int,9> nbrs;
	nbrs.fill(0);

	// Horizontal row.
	b = YY(i);
	for (a=0; a<9; a++) {
		val = o[BOARD(a,b)];
		if (val > 0) {
			val--;
			nbrs[val] = 1;
		}
	}
	
	// Vertical column.
	a = XX(i);
	for (b=0; b<9; b++) {
		val = o[BOARD(a,b)];
		if (val > 0) {
			val--;
			nbrs[val] = 1;
		}
	}
	
	// Group of 3x3.
	a = XX(i);  c = M(a);
	b = YY(i);  d = M(b);
	for (y=0; y<3; y++) {
		for (x=0; x<3; x++) {
			val = o[BOARD(c+x,d+y)];
			if (val > 0) {
				val--;
				nbrs[val] = 1;
			}
		}
	}

	//cout << "  For o[" << i << "]:" << endl;
	//for (a=0; a<nbrs.size(); a++)
	//	cout << "   {" << a+1 << "} = " << nbrs[a] << endl;
	
	// Returns an array of size 9.  Each index holds either 0 or 1.
	// e.g.   
	//  ArrayIndex  0 1 2 3 4 5 6 7 8
	//  Suduko Nbr  1 2 3 4 5 6 7 8 9
	//  Value       0 1 1 1 0 1 0 1 1
	// Means that Suduko Nbr '1' is not present in the Suduko 
	// lines.  Whereas Nbr '2' is present.
	return nbrs;
}

//*******************************************************

bool solve(std::array<int, 81> &o, int i, bool findall) {
	int x;
	bool r;
	array <int,9> a;

	//cout << "solver(" << i << ") - ";
	//if (i < 9*9) cout << o[i];
	//cout << endl;
	//show_board(o, i);
	
	// Reached the end ? 
	if (i >= 81) {
		if (findall) found++;
		show_board(o, i);
		return !findall; 	// If 'true' then it signals that we are complete.
	}
	
	// Check at index position 'i', does it have a zero ?
	if (o[i] == 0) {
		// Compile a vector of current values.
		a = current_values(o, i);

		// Loop through the missing values.
		for (x=0; x<a.size(); x++) {
			if (a[x] == 0) {
				// The Suduko Nbr 'x+1' is missing and thus
				// we should add it onto a new Suduko board
				// and recurse.
				std::array<int, 81> c;
				c = o;
				c[i] = x+1;

				r = solve(c, i+1, findall);
				if (r == true)
					return true;
			}
		}

		return false;
	}

	return solve(o, i+1, findall);
}

//*******************************************************
// findall - true  - find all solutions
//           false - find first solution and return.
void solve_singleboard(std::array<int, 81> &b, bool findall) {
	bool res;
	double time_taken;
	clock_t t;
	
	std::cout << "Suduko Solver!" << std::endl;

	show_board(b, -1);

    t = clock();

	// Zero out the global.
	found = 0;
	
	// Start Recursion.
	res = solve(b, 0, findall);

	cout << "The number of solutions found is " << found << endl;

	t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
    printf("Took %f seconds to solve.\n", time_taken);

	return;
}

//*******************************************************

void solve_testboard() {

	std::cout << "Suduko Test Board" << std::endl;

	std::array<int, 81> b = {2,0,0,  0,3,1,  4,6,0,
  							 0,8,0,  0,0,0,  3,5,1,
 							 6,1,3,  5,0,0,  0,8,7,
 							 3,0,0,  4,1,0,  0,0,0,
 							 4,2,0,  0,6,3,  0,0,0,
 							 5,0,0,  0,0,8,  7,0,0,
 							 7,4,5,  3,0,0,  0,2,0,
 							 0,0,2,  0,0,7,  9,4,0,
 							 0,9,6,  2,0,0,  0,7,0 };
	solve_singleboard(b, false);
}

// ***********************************************************
// Read in unsolved boards from file.
//    sec/board
//  0.000131403  - unsolved-45.txt
//  0.0001882    - unsolved-40.txt
//  0.000407937  - unsolved-35.txt
//  0.00195612   - unsolved-30.txt
//  0.032677564  - unsolved-25.txt

int solve_multiboard(std::string fname) {
	double time_taken;
	clock_t t;
	FILE *fp;
	std::array<int, 81> c;
	int ch, puzzle = 1;


	std::cout << "Suduko Multi Board Solver!" << std::endl;
	
	fp = fopen(fname.c_str(),"r");
	if(fp == NULL) {
		printf("Error openning file %s\n", fname.c_str());   
		return -1;      
	}

	t = clock();
	while(!feof(fp)) {

		for(int i=0; i<81; i++)
			c[i] = ((char)fgetc(fp)) - '0';

		// Read EOL/EOF character.
		ch = 0;
		while (ch != 0x0A && ch != EOF) 
			ch = fgetc(fp);

		cout << "Puzzle: " << puzzle <<endl;
		show_board(c, 100);

		found = 0;
		solve(c, 0, false);

		puzzle++;
	}
	
	t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
	printf("Took %f seconds to solve %i puzzles.\n", time_taken, puzzle);
	
	fclose(fp);
	
	return 0;
}

// ***********************************************************
