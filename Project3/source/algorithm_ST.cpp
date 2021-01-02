#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"


#include <stack>


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

class B{
    private:
        int btmp[5][6];
        Board state;
        int new_move;
    public:
        B(Board board){
            state = board;
            new_move = 1;
            for (int i = 0; i < 5; i++){
                for (int j = 0; j < 6;j++){
                    btmp[i][j] = board.get_orbs_num(i, j);
                    if (board.get_cell_color(i, j) == 'b') btmp[i][j] = -btmp[i][j];
                }
            }
        }
        int get_num(int i, int j){
            return abs(btmp[i][j]);
        }
        void set(int i, int j, int val){
            btmp[i][j] = val;
        }
        char get_color(int i, int j){
            if(btmp[i][j]> 0) return 'r';
            else if(btmp[i][j]< 0) return 'b';
            else return 'w';
        }
        void update(int m, int n, Player * player){
            state.place_orb(m,n,player);
            for (int i = 0; i < 5; i++){
                for (int j = 0; j < 6;j++){
                    btmp[i][j] = state.get_orbs_num(i, j);
                    if (state.get_cell_color(i, j) == 'b') btmp[i][j] = -btmp[i][j];
                }
            }
        }
        Board get_board(){ return state;}
};



int* chain(Board board, Player player){
    B btmp(board);
    char color = player.get_color();
    int lengths[30];
    int tmp = 0;
    for(int i = 0; i < ROW; i++){
        for(int j = 0; j < COL; j++){
            int l = 0;
            if((btmp.get_num(i, j) == board.get_capacity(i, j)-1) && (btmp.get_color(i, j) == color)){
                stack<int*> visiting_stack;
                int position[2] = {i, j};
                visiting_stack.push(position);
                while (!visiting_stack.empty()){
                    btmp.set(visiting_stack.top()[0],visiting_stack.top()[1],0);
                    visiting_stack.pop();
                    l+=1;
                    int** neighbor = neighbors(i, j);
                    int num = near(i, j);
                    for (int z = 0; z < num; z++){
                        if ((btmp.get_num(neighbor[z][0], neighbor[z][1])==board.get_capacity(i, j)-1)&&(btmp.get_color(i, j) == color)){
                            position[0] = neighbor[z][0];
                            position[1] = neighbor[z][1];
                            visiting_stack.push(position);
                        }
                    }
                }
            }
            lengths[tmp] = l;
            tmp++;
        }
    }
    return lengths;
}


int evaluate(Board board, Player player){
    int score = 0;
    int my_orbs = 0, enemy_orbs = 0;

    char color = player.get_color();
    char enemy_color;
    if (color=='r') enemy_color='b';
    else enemy_color='r';

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
    score += my_orbs;
    if (enemy_orbs == 0 && my_orbs > 1) return 10000;
    if (enemy_orbs > 1 && my_orbs == 0) return -10000;
    int* chain_l = chain(board, player);
    for (int i = 0; i <30; i++){
        if (chain_l[i]>1) score += 2*chain_l[i]; 
    }
    return score;
}

class bestnode{
    private:
        int row;
        int col;
        int score;
    public:
        bestnode(){
            row=0; col=0; score=0;
        }
        int get_score(){return score;}
        int get_row(){ return row;}
        int get_col(){ return col;}
        void set_score(int sc){score=sc;}
        void set_position(int x,int y){row=x;col=y;}
};

bestnode minimax(Board board, int depth,bool max, Player player){
    char color = player.get_color();
    char enemy_color = (color=='r')? 'b':'r';
    if(max){
        bestnode best;
        best.set_score(-10000);

        for(int i=0;i<5;i++){
            for(int j=0;j<6;j++){
                if(board.get_cell_color(i,j)!=enemy_color){
                    Board thisround=board;
                    thisround.place_orb(i,j,&player);
                    bestnode tmp = minimax(thisround, depth+1, false, player);
                    tmp.set_position(i, j);
                    if (tmp.get_score()>best.get_score()) best = tmp;
                }
            }
        }
        return best;
    }
    else{
        bestnode best;
        best.set_score(10000);
        for(int i=0;i<5;i++){
            for(int j=0;j<6;j++){
                if(board.get_cell_color(i,j)!=color){
                    Board thisround=board;
                    Player enemy(enemy_color);
                    thisround.place_orb(i,j,&enemy);
                    bestnode tmp = minimax(thisround, depth+1, false, player);
                    tmp.set_position(i, j);
                    if (tmp.get_score()<best.get_score()) best = tmp;
                }
            }
        }
        return best;
    }
    
}

void algorithm_A(Board board, Player player, int index[]){
    srand(time(NULL)*time(NULL));
    int row, col;
    char color = player.get_color();
    /*
    Board thisround = board;
    bestnode best = minimax(thisround,0,true,player);
    index[0] = best.get_row();
    index[1] = best.get_col();
    cout << index[0] << " " << index[1] << endl;
    */
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
            /*Board thisround = board;
            thisround.place_orb(row,col,&player);
            int score = evaluate(thisround, player);*/
            for(int i=0;i<ROW;i++){
                for(int j=0;j<COL;j++){
                    if(board.get_cell_color(i,j)==color){
                        Board tmpround = board;
                        tmpround.place_orb(i,j,&player);
                        int tmpscore = evaluate(tmpround, player);
                        if (tmpscore==10000){
                            row=i;
                            col=j;
                            //score = tmpscore;
                        }
                        /*if((board.get_capacity(i,j)-board.get_orbs_num(i,j))<=4){
                            row=i;
                            col=j;
                        }*/
                    }
                }
            }
            if (board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w') break;
        }
    }
    
    index[0] = row;
    index[1] = col;
}