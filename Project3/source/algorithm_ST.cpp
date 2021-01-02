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

int critical(Board board, int m, int n){
    return board.get_capacity(m, n) - 1;
}

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

class node{
    private:
        Board state;
        int mn[2];
    public:
        node(Board board){
            this->state = board;
        }
        Board get_board(){ return this->state;}
        int get_row(){ return this->mn[0];}
        int get_col(){ return this->mn[1];}
};

class gametree{
    private:
        node *root;
        Player *player;
        Player *enemy;
    public:
        gametree(Board board, Player *player){
            this->root = new node(board);
            this->player = player;
            if(player->get_color()=='r') this->enemy = new Player('b');
            else this->enemy = new Player('r');
        }
        int evaluator(node *n){
            Board board = n->get_board();
            char color = player->get_color();
            char enemy_color = enemy->get_color();
            int score = 0;
            int orb = 0, enemy_orb = 0;
            for (int i = 0; i < 5; i++){
                for (int j = 0; j < 6; j++){
                    if (board.get_cell_color(i,j)==enemy_color) enemy_orb += board.get_orbs_num(i,j);
                    else if (board.get_cell_color(i,j)==color){
                        orb += board.get_orbs_num(i,j);
                        bool safe = true;
                        int** neighbor = neighbors(i, j);
                        int num = near(i, j);

                        for (int z = 0; z < num; z++) {
                            if((board.get_cell_color(neighbor[z][0], neighbor[z][1])==enemy_color)&&(board.get_orbs_num(neighbor[z][0], neighbor[z][1])==board.get_capacity(neighbor[z][0], neighbor[z][1])-1)){
                                score -= 9 - board.get_capacity(i, j);
                                safe = false;
                            }
                        }
                        delete [] neighbor;

                        // no enemy
                        if (safe){
                            // corner
                            if (board.get_capacity(i, j) == 3) score += 6;
                            // edge
                            else if (board.get_capacity(i, j) == 5) score += 4;
                            // unstability
                            if (board.get_orbs_num(i, j) == board.get_capacity(i, j) - 1) score += 4;
                        }
                    } 
                }
            }
            score += (orb-enemy_orb);
            if (enemy_orb == 0 && orb > 1) return 10000;
            else if (enemy_orb > 1 && orb == 0) return -10000;
            else return score;
        }
};

void algorithm_A(Board board, Player player, int index[]){
    srand(time(NULL)*time(NULL));
    int row, col;
    char color = player.get_color();
    int corners[4][2] = {
        {0,0},{0,5},{4,0},{4,5}
    };
    int i = 0;
    bool dangerous = false;
    while(1){
        if(i<4){
            row = corners[i][0];
            col = corners[i][1];
            int** neighbor = neighbors(row,col);
            if (board.get_cell_color(row, col) == 'w') break;
            if (board.get_cell_color(row, col) == color){
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
                if (dangerous) break;
            }
            delete [] neighbor;
            i++;
        }
        else{
            row = rand() % 5;
            col = rand() % 6;
            for (int i = 0; i < 4; i++){
                while(row==corners[i][0] && col==corners[i][1]){
                    row = rand() % 5;
                    col = rand() % 6;
                }
            }
            if (board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w') break;
        }
    }

    index[0] = row;
    index[1] = col;
}