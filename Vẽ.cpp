#include <conio.h>
#include<iostream>
#include <windows.h>
#include "mylib.h"
#define MAX 100
using namespace std;
int s1 = 4;


// Khai báo hàm v? tu?ng
void ve_tuong_tren();
void ve_tuong_duoi();
void ve_tuong_phai();
void ve_tuong_trai();
void ve_tuong();
void khoi_tao_ran(int toadox[], int toadoy[]);
void xoa_du_lieu_cu(int toadox[], int toadoy[]);
void ve_ran(int toadox[], int toadoy[]);
void xu_ly_ran(int toadox[], int toadoy[], int x, int y);
void them(int a[], int x);
void xoa(int a[], int vt);
int main()
{
    int toadox[MAX], toadoy[MAX];
    ve_tuong();
    khoi_tao_ran(toadox, toadoy);
    ve_ran(toadox, toadoy);
    int x = 50, y = 13;// ding hinh vi tri can di chuyen cho ran
    int check = 2;
    while (true)
    {
        
       // system("cls")
        // ==== backspace
        xoa_du_lieu_cu(toadox, toadoy);
     
        // 0 : di xuong
        // 1 : di len
        // 2 : qua phai
        // 3 : qua trai
        // ===== dieu kien
        if (_kbhit())
        {
            char kitu = _getch();
            if (kitu == -32)
            {
                kitu = _getch();
                if (kitu == 72 && check!=0) //di len
                {
                    check = 1;
                }
                else if (kitu == 80 && check!=1) // di xuong
                {
                    check = 0;
                }
                else if (kitu == 77 && check!=3)// di phai
                {
                    check = 2;
                }
                else if (kitu == 75 && check!=2)// qua trai
                {
                    check = 3;
                }
            }
        }
        // ====== thuc hien di chuyen
        if (check == 0)
        {
            y++;//di xuong
        }
        else if (check == 1)
        {
            y--;//di len
        }
        else if (check == 2)
        {
            x++; //di qua phai
        }
        else if (check == 3)
        {
            x--;//di qua trai
        }
        xu_ly_ran(toadox, toadoy, x, y);
        Sleep(150);
    }
    _getch(); // ch? nh?n phím
    return 0;
}

void ve_tuong_tren()
{
    int x = 10, y = 1;
    while (x <= 100)
    {
        gotoXY(x, y);
        cout << "*";
        x++;
    }
}

void ve_tuong_duoi()
{
    int x = 10, y = 26;
    while (x <= 100)
    {
        gotoXY(x, y);
        cout << "*";
        x++;
    }
}

void ve_tuong_phai()
{
    int x = 100, y = 1;
    while (y <= 26)
    {
        gotoXY(x, y);
        cout << "*";
        y++;
    }
}

void ve_tuong_trai()
{
    int x = 10, y = 1;
    while (y <= 26)
    {
        gotoXY(x, y);
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

void khoi_tao_ran(int toadox[], int toadoy[])
{
    int x = 50, y = 13;
    for (int i = 0; i < s1; i++)
    {
        toadox[i] = x;
        toadoy[i] = y;
        x--;
    }
}

void ve_ran(int toadox[], int toadoy[])
{
    for (int i = 0; i < s1; i++)
    {
        gotoXY(toadox[i], toadoy[i]);
        if (i == 0)
        {
            cout << "0";
        }
        else
        {
            cout << "o";
        }
    }
}
void xoa_du_lieu_cu(int toadox[], int toadoy[])
{
    for (int i = 0; i < s1; i++)
    {
        gotoXY(toadox[i], toadoy[i]);
        cout << " ";
    }
}
void xu_ly_ran(int toadox[],int toadoy[],int x,int y)
{
    //b1: them toa do moi vao dau mang
    them(toadox, x);
    them(toadoy, y);
    //b2: xoa toa do cuoi mang
    xoa(toadox, s1 - 1);
    xoa(toadoy, s1 - 1);
    //b3: ve ran
    ve_ran(toadox, toadoy);
}
void them(int a[], int x)
{
    for (int i = s1; i > 0; i--)
    {
        a[i] = a[i - 1];
    }
    a[0] = x;
    s1++;
}
void xoa(int a[], int vt)
{
    for (int i = vt; i < s1; i++)
    {
        a[i] = a[i + 1];
    }
    s1--;
}
