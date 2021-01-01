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
/*
template <class T> const T& max (const T& a, const T& b) {
    return (a<b)?b:a;
}

template <class T> const T& min (const T& a, const T& b) {
    return !(b<a)?a:b;
}
*/
int** neighbors(int m, int n){ 
    int num;
    if ((m>0 && m<4) && (n>0 && n<5)) num = 8;
    else if((m==0&&n==0)||(m==0 && n==5)||(m==4 && n==0)||(m==4 && n==5)) num = 3;
    else num = 5;
    int** neighbor = new int*[num];
    int i = 0;
    for (int r = max(0, m-1); r <= min(m+1,4); r++){
        for (int c = max(0,n-1); c <= min(n+1,5); c++){
            if(r==m && c==n) continue;
            neighbor[i] = new int[2];
            neighbor[i][0] = r;
            neighbor[i][1] = c;
            cout << r << " " << c << ";";
            i++;
        }
    }
    cout << endl;
    return neighbor;
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
            if(board.get_cell_color(row, col) == 'w') break;
            i++;
        }
        else{
            row = rand() % 5;
            col = rand() % 6;
            if(board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w') break;
        }
    }

    int** neighbor = neighbors(row,col);

    index[0] = row;
    index[1] = col;
}