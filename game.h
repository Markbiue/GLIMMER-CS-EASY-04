#ifndef GAME_H  // 头文件卫士：防止该头文件被多次包含导致重复定义
#define GAME_H

#define BOARD_SIZE 9
#define WIN_LENGTH 5
#define EMPTY_CELL '.'

typedef enum {
    GAME_RUNNING, // 游戏进行中
    GAME_X_WINS, // X胜出
    GAME_O_WINS, // O胜出
    GAME_DRAW //平局
} GameStatus; // 枚举游戏状态

typedef enum {
    MOVE_ACCEPTED, // 可以下棋
    MOVE_OUT_OF_BOUNDS, // 当前位置在棋盘外
    MOVE_OCCUPIED // 当前位置已占用
} MoveResult; // 下棋结果

typedef struct {
    char board[19][37];
    char *cells[BOARD_SIZE][BOARD_SIZE]; // 棋盘信息
    char current_player;
    int move_count; // 步数统计
    GameStatus status;
} Game;

void init_game(Game *game);                      //初始化游戏状态
MoveResult make_move(Game *game, int row, int col); //尝试在指定位置落子
void check_game_status(Game *game, int row, int col); //检查落子后是否达成胜负或平局条件
void Print_board(const Game *game);

#endif 