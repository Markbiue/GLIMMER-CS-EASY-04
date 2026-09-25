#include <windows.h>
#include <stdio.h>

#include "console.h"
#include "game.h"

void DrawCharAt(int consoleX, int consoleY, char c) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {(SHORT)consoleX, (SHORT)consoleY};
    SetConsoleCursorPosition(hOut, pos); // 瞬移光标
    printf("%c", c);                     // 直接覆盖打印
}

//获取控制台鼠标点击坐标
int GetConsoleCellIndex(int *outX, int *outY) {
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD inputRecord;
    DWORD numRead;

    // 开启鼠标输入，关闭快速编辑模式（防止点击变成文本选择）
    SetConsoleMode(hInput, ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);

    while (1) { // 使用循环持续读取，直到捕获到有效点击
        ReadConsoleInput(hInput, &inputRecord, 1, &numRead);

        // 必须是鼠标事件
        if (inputRecord.EventType == MOUSE_EVENT) {
            // 核心过滤条件：
            // 1. dwEventFlags == 0 表示普通的按钮状态改变（排除了鼠标移动、双击等）
            // 2. dwButtonState != 0 表示有按键被按下（排除了按键松开的事件）
            if (inputRecord.Event.MouseEvent.dwEventFlags == 0 && 
                inputRecord.Event.MouseEvent.dwButtonState != 0) {
                
                *outX = inputRecord.Event.MouseEvent.dwMousePosition.X;
                *outY = inputRecord.Event.MouseEvent.dwMousePosition.Y;
                
                //防抖：点击成功后，休眠 150 毫秒，并清空缓冲区里多余的鼠标事件
                Sleep(150); 
                FlushConsoleInputBuffer(hInput); 
                return 1; 
            }
        }
    }
}
    
// 计算离鼠标点击最近的交叉点
int ConvertToCellIndex(int consoleX, int consoleY, int *outRow, int *outCol) {
    // 1. 计算行下标 (Y = 2*row + 1)
    // 真实公式：row = (Y - 1) / 2.0
    // 范围内距离最近的原理：加上 0.5 后强制转换为 int，实现四舍五入
    float trueRow = (consoleY - 1) / 2.0f;
    *outRow = (int)(trueRow + 0.5f);
    
    // 2. 计算列下标 (X = 4*col + 2)
    // 真实公式：col = (X - 2) / 4.0
    // 范围内距离最近的原理：加上 0.5 后强制转换为 int，实现四舍五入
    float trueCol = (consoleX - 2) / 4.0f;
    *outCol = (int)(trueCol + 0.5f);
    
    // 3. 边界检查：确保计算出的下标在 9x9 范围内
    if (*outRow < 0 || *outRow >= BOARD_SIZE || *outCol < 0 || *outCol >= BOARD_SIZE) {
        return 0; 
    }
    
    // 4. 范围内距离最近的交叉点识别成功，返回 1
    return 1; 
}