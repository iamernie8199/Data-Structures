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

// return the # of the neighbors
int near(int m, int n){ 
    if ((m > 0 && m < 4) && (n > 0 && n < 5)) return 8;
    else if((m == 0 && n == 0)||(m == 0 && n == 5)||(m == 4 && n == 0)||(m == 4 && n == 5)) return 3;
    else return 5;
}


// return 2d array of the neighbors
int** neighbors(int m, int n){ 
    int num = near(m, n);
    int** neighbor = new int*[num];
    int i = 0;
    for (int r = max(0, m-1); r <= min(m+1,4); r++){
        for (int c = max(0,n-1); c <= min(n+1,5); c++){
            if(r==m && c==n) continue;
            neighbor[i] = new int[2];
            neighbor[i][0] = r;
            neighbor[i][1] = c;
            i++;
        }
    }
    /*
    for (int z = 0; z < num; z++){
        cout << neighbor[z][0] << " " << neighbor[z][1] << ";";
    }
    cout << endl;
    */
    return neighbor;
}


int evaluate(Board board, Player player){
    int score = 0, my_orbs = 0, enemy_orbs = 0;
    char color = player.get_color();
    char enemy_color;
    enemy_color=(color=='r')? 'b':'r';
    bool safe;
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            if(board.get_cell_color(i, j) == color){
                my_orbs += board.get_orbs_num(i, j);
                safe = true;
                int** neighbor = neighbors(i, j);
                int num = near(i, j);
                for (int z = 0; z < num; z++) {
                    if((board.get_cell_color(neighbor[z][0], neighbor[z][1])==enemy_color)&&(board.get_orbs_num(neighbor[z][0], neighbor[z][1])==board.get_capacity(neighbor[z][0], neighbor[z][1])-1)){
                        score -= 9 - board.get_capacity(i, j);
                        safe = false;
                    }
                }

                for (int z = 0; z < num; z++){
                    delete [] neighbor[z];
                }
                delete [] neighbor;

                // no enemy
                if (safe){
                    // corner Heuristic
                    if (board.get_capacity(i, j) == 3) score += 6;
                    // edge Heuristic
                    else if (board.get_capacity(i, j) == 5) score += 4;
                    // unstability Heuristic
                    if (board.get_orbs_num(i, j) == board.get_capacity(i, j) - 1) score += 4;
                }
            }
            else{
                enemy_orbs += board.get_orbs_num(i, j);
            }
        }
    }
    score += (my_orbs-enemy_orbs);
    if (enemy_orbs == 0 && my_orbs > 1) return 10000;
    if (enemy_orbs > 1 && my_orbs == 0) return -10000;
    return score;
}

void algorithm_A(Board board, Player player, int index[]){
    srand(time(NULL)*time(NULL));
    int row, col;
    char color = player.get_color();
    char enemy_color=(color=='r')? 'b':'r';
    int corners[4][2] = {
        {0,0},{0,5},{4,0},{4,5}
    };
    int i = 0;
    bool dangerous = false;
    while(1){
        if(i<4){
            row = corners[i][0];
            col = corners[i][1];
            if (board.get_cell_color(row, col) == 'w') break;
            if (board.get_cell_color(row, col) == color){
                int** neighbor = neighbors(row,col);
                for (int n = 0; n<3; n++){
                    if(!(board.get_cell_color(neighbor[n][0], neighbor[n][1]) == color || board.get_cell_color(neighbor[n][0], neighbor[n][1]) == 'w')){
                        int warning = board.get_capacity(neighbor[n][0], neighbor[n][1]);
                        warning -= board.get_orbs_num(neighbor[n][0], neighbor[n][1]);
                        if(warning <= 3-board.get_orbs_num(row, col)){
                            dangerous = true;
                            break;
                        }
                    }
                }
                for (int n = 0; n<3; n++){
                    delete [] neighbor[n];
                }
                delete [] neighbor;
                if (dangerous) break;
            }
            i++;
        }
        else{
            int score = -10000;
            for(int i=0;i<ROW;i++){
                for(int j=0;j<COL;j++){
                    if(board.get_cell_color(i,j)!=enemy_color){
                        Board tmpround = board;
                        tmpround.place_orb(i,j,&player);
                        int tmpscore = evaluate(tmpround, player);
                        if (tmpscore==10000){
                            row=i;
                            col=j;
                            break;
                        }
                        else if (tmpscore>score){
                            row=i;
                            col=j;
                            score = tmpscore;
                        }
                    }
                }
            }
            if (board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w') break;
        }
    }
    index[0] = row;
    index[1] = col;
}