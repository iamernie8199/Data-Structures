#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"

using namespace std;

/******************************************************
 * In your algorithm, you can just use the the funcitons
 * listed by TA to get the board information.(functions 
 * 1. ~ 4. are listed in next block)
 * 
 * The STL library functions is not allowed to use.
******************************************************/

/*************************************************************************
 * 1. int board.get_orbs_num(int row_index, int col_index)
 * 2. int board.get_capacity(int row_index, int col_index)
 * 3. char board.get_cell_color(int row_index, int col_index)
 * 4. void board.print_current_board(int row_index, int col_index, int round)
 * 
 * 1. The function that return the number of orbs in cell(row, col)
 * 2. The function that return the orb capacity of the cell(row, col)
 * 3. The function that return the color fo the cell(row, col)
 * 4. The function that print out the current board statement
*************************************************************************/

int sgn(int n){
    if(n == 0) return 0;
    else return n/abs(n);
}


void algorithm_A(Board board, Player player, int index[]){
    srand(time(NULL)*time(NULL));
    int row, col;
    int color = player.get_color();
    int corners[4][2] = {
        {0,0},{0,5},{4,0},{4,5}
    };
    int i = 0;

    while(1){
        if(i<4){
            row = corners[i][0];
            col = corners[i][1];
            i++;
            if(board.get_cell_color(row, col) == 'w') break;
        }
        else{
            row = rand() % 5;
            col = rand() % 6;
        }
        if(board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w') break;
    }

    index[0] = row;
    index[1] = col;
}