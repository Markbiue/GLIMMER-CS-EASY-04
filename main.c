#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "game.h"
#include "console.h"

int main() {
    system("chcp 65001 > nul");
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD infoPos = {0, 19};

    Game *game = (Game*)malloc(sizeof(Game));
    if (!game) return 0;

    init_game(game);
    Print_board(game);

    while (game -> status == GAME_RUNNING) {
        int consoleX, consoleY;
        // 等待并获取控制台鼠标坐标
        if (GetConsoleCellIndex(&consoleX, &consoleY)) {
            int row, col;
            // 将控制台坐标转换为 cells 下标
            if (ConvertToCellIndex(consoleX, consoleY, &row, &col)) {
                 char piece = game->current_player;
                //落子
                if (make_move(game, row, col) == MOVE_ACCEPTED) {
                    int drawX = 4 * col + 2;
                    int drawY = 2 * row + 1;
                    DrawCharAt(drawX,drawY,piece);

                    SetConsoleCursorPosition(hOut, infoPos); 
                    printf("当前玩家: %c 步数: %d 游戏状态：%s ", game->current_player, game->move_count, "RUNNING");
                    fflush(stdout);
                }
            }
        }
    }
     SetConsoleCursorPosition(hOut, infoPos);
    if (game->status == GAME_X_WINS) {
        printf("============= X 获胜！==============\n");
    } else if (game->status == GAME_O_WINS) {
        printf("============= O 获胜！==============\n");
    } else if (game->status == GAME_DRAW) {
        printf("============== 平局！===============\n");
    }
    char a;
    printf("\n想要悔棋吗，少年？y/n");
    scanf("%c",&a);
    printf("\n想P吃！哈哈哈");

    printf("\n按任意键退出...");
    getchar();

    free(game);
    return 0;
}
