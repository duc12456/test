#include <iostream>
#include <windows.h>
#include <conio.h>

#define MAX 100

using namespace std;
int sl = 4;

void gotoXY(int x, int y) {
    HANDLE hConsoleOutput;
    COORD Cursor_an_Pos = { (SHORT)x, (SHORT)y };
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}

// Khai báo hàm v? tu?ng
void ve_tuong_tren();
void ve_tuong_duoi();
void ve_tuong_phai();
void ve_tuong_trai();
void ve_tuong();
void khoi_tao_ran(int toadox[],int toadoy[]);
void ve_ran(int toadox[], int toadoy[]);

int main() 
{
    int toadox[MAX], toadoy[MAX];
    ve_tuong();
    khoi_tao_ran(toadox, toadoy);
    ve_ran(toadox, toadoy);
    _getch(); // ch? nh?n phím
    return 0;
}

void ve_tuong_tren() 
{
    int x = 10, y = 1;
    while(x < 100) 
	{
        gotoXY(x,y);
        cout << "*";
        x++;
    }
}

void ve_tuong_duoi() 
{
    int x = 10,y = 26;
    while(x < 100) {
        gotoXY(x,y);
        cout << "*";
        x++;
    }
}

void ve_tuong_phai() 
{
    int x = 100, y = 1;
    while(y <= 26) {
        gotoXY(x,y);
        cout << "*";
        y++;
    }
}

void ve_tuong_trai() 
{
    int x = 10, y = 1;
    while(y <= 26) {
        gotoXY(x,y);
        cout << "*";
        y++;
    }
}

void ve_tuong()
{
	ve_tuong_tren();
	ve_tuong_duoi();
	ve_tuong_phai();
	ve_tuong_trai();
}

void khoi_tao_ran(int toadox[],int toadoy[])
{
    int x = 50, y = 13;
    for(int i = 0; i < sl; i++)
    {
        toadox[i] = x;
        toadoy[i] = y;
        x--;
    }
}

void ve_ran(int toadox[], int toadoy[])
{
    for(int i = 0; i < sl; i++)
    {
        gotoXY(toadox[i],toadoy[i]);
        if(i == 0)
        {
            cout << "0";
        }
        else
        {
            cout << "o";
        }
    }
}
