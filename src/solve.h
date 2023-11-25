#ifndef _SOLVE_H_
#define _SOLVE_H_

#include <array>
#include <string>


void show_board(const std::array<int,81> &o, int z);

void solve_singleboard(std::array<int, 81> &o, bool findall);

void solve_testboard(void);
int solve_multiboard(std::string filename);

#endif
