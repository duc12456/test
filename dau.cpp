#include <conio.h>
#include<iostream>
#include <windows.h>
#include "mylib.h"
using namespace std;

int main() {
    int i = 1;
    int x = 10, y = 0;
    int xcu = x, ycu = y;
    int check = 0;

    while (true) {
        gotoXY(xcu, ycu);
        cout << "                     ";

        gotoXY(x, y);
        cout << "HOC LAP TRINH THAT DE";

        xcu = x;
        ycu = y;

        SetColor(i);
        i++;
        if (i > 15) i = 1;

        // Ðieu khien phím mui tên
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
