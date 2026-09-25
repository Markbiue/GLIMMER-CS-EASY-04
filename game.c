#include <stdio.h>
#include <windows.h>
#include <string.h>  
#include "game.h"

void init_game(Game *game) {
    memset(game->board, 0, sizeof(game->board));

    game->current_player = 'X'; 
    game->move_count = 0;       
    game->status = GAME_RUNNING; 

    for (int i = 0; i < BOARD_SIZE + 1; i++) {
        for (int j = 0; j < BOARD_SIZE + 1; j++) {
            game->board[2*i][4*j] = '+';
            if (j < BOARD_SIZE) {
                game->board[2*i][4*j+1] = '-';
                game->board[2*i][4*j+2] = '-';
                game->board[2*i][4*j+3] = '-';
            }
            if (i < BOARD_SIZE) {
                game->board[2*i+1][4*j] = '|';
                if (j < BOARD_SIZE) {
                    game->board[2*i+1][4*j+2] = '.'; // 默认空位是 '.'
                }
            }
        }
    }

    // 3. 将 cells 指针数组指向 board 中的落子点 (消除冗余数据)
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            game->cells[i][j] = &game->board[2*i+1][4*j+2];
        }
    }
}

void Print_board(const Game *game){
    int i,j;
    for(i = 0; i < 19; i++) {
        for(j = 0; j < 37; j++) {
            if(game->board[i][j] == 0){ 
                printf(" ");
            }else { 
                printf("%c", game->board[i][j]);
            }
        }
        printf("\n");
    }
    return;
}

void check_game_status(Game *game, int row, int col) {

    int directions[4][2] = {
        {0, 1}, // 向右
        {1, 0}, // 向下
        {1, 1}, //向右下
        {1, -1} //向左下
    };

    //遍历八个方向
    for (int d = 0; d < 4; d++) {
        int dr = directions[d][0]; // 获取行增量
        int dc = directions[d][1]; // 获取列增量

        int count = 1; 

        //正方向
        for (int i = 1; i < WIN_LENGTH; i++) {
            int r = row + i * dr;
            int c = col + i * dc;
            if (r < 0 || r >= BOARD_SIZE || 
                c < 0 || c >= BOARD_SIZE || 
                *(game->cells[r][c]) != *(game->cells[row][col])) {
                break;
            }
            count++;
        }

        //反方向
        for (int i = 1; i < WIN_LENGTH; i++) {
            int r = row - i * dr;
            int c = col - i * dc;
            if (r < 0 || r >= BOARD_SIZE ||
                 c < 0 || c >= BOARD_SIZE ||
                  *(game->cells[r][c]) != *(game->cells[row][col])) {
                break;
            }
            count++;
        }

        //如果正反两个方向加起来不小于5，获胜
        if (count >= WIN_LENGTH) {
            if(*(game->cells[row][col]) == 'X'){
                game->status = GAME_X_WINS;
            }else {
                game->status = GAME_O_WINS;
            }
            return; 
        }
    }

    //如果 4 个方向都没赢，且步数满了，则为平局
    if (game->move_count == BOARD_SIZE * BOARD_SIZE) {
        game->status = GAME_DRAW;
    }else {
        game->status = GAME_RUNNING;
    }
    return;
}

MoveResult make_move(Game *game, int row, int col) {
    //检查边界
    if (row < 0 || row >= BOARD_SIZE ||
         col < 0 || col >= BOARD_SIZE) {
        return MOVE_OUT_OF_BOUNDS;
    }
    //检查该位置是否已有子
    if (*(game->cells[row][col]) != EMPTY_CELL) {
        return MOVE_OCCUPIED;
    }
    //落子
    *(game->cells[row][col]) = game->current_player;
    game->move_count++;
    //检查胜负
    check_game_status(game, row, col);
    //切换
    if (game->status == GAME_RUNNING) {
        if(game->current_player == 'X'){
            game->current_player = 'O';
        }else {
            game->current_player = 'X';
        }
    }
    return MOVE_ACCEPTED;
}