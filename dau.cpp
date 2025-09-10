#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
using namespace std;

// ==== Hàm ti?n ích (thay cho mylib.h) ====
void gotoXY(int x, int y) {
    HANDLE hConsoleOutput;
    COORD Cursor_an_Pos = { (SHORT)x, (SHORT)y };
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}

void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
// ==========================================

int main() {
    int i = 1;
    int x = 10, y = 0;
    int xcu = x, ycu = y;
    int check = 0;

    while (true) {
        gotoXY(xcu, ycu);
        cout << "                     ";

        gotoXY(10, 0);
        cout << "HOC LAP TRINH THAT DE";

        xcu = x;
        ycu = y;

        SetColor(i);
        i++;
        if (i > 15) i = 1;

        // Ði?u khi?n phím mui tên
        if (_kbhit()) {
            char kitu = _getch();
            if (kitu == -32) {
                kitu = _getch();
                if (kitu == 72) check = 1; // lên
                else if (kitu == 80) check = 0; // xu?ng
                else if (kitu == 75) check = 2; // trái
                else if (kitu == 77) check = 3; // ph?i
            }
        }

        // Di chuy?n
        if (check == 0) y++;
        else if (check == 1) y--;
        else if (check == 2) x--;
        else if (check == 3) x++;

        // Gi?i h?n biên
        if (y == 28) check = 1;
        else if (y == 0) check = 0;
        else if (x == 0) check = 3;
        else if (x == 100) check = 2;

        Sleep(100);
    }

    _getch();
    return 0;
}
