#ifndef CONSOLE_H
#define CONSOLE_H

void DrawCharAt(int consoleX, int consoleY, char c);
int GetConsoleCellIndex(int *outX, int *outY);
int ConvertToCellIndex(int consoleX, int consoleY, int *outRow, int *outCol);

#endif