#include <conio.h>
#include<iostream>
#include <windows.h>
#include "mylib.h"
#define MAX 100

using namespace std;

int s1 = 7;

// khu vuc khai bao ham nguyen mau

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
bool kiem_tra_va_cham(int toadox[], int toadoy[], int x, int y);
bool kt_ran_cham_tuong(int x0, int y0);
bool kt_ran_cham_duoi(int toadox[],int toadoy[]);
bool kt_ran(int toadox[], int toadoy[]);
void tao_qua(int xqua, int yqua);

// ham main

int main()
{
	srand(time(Null));
	int xqua = 0, yqua = 0;
	
	tao_qua(xqua,yqua);
	
    bool gameover = false;
    
    int toadox[MAX], toadoy[MAX];
    ve_tuong();
    
    khoi_tao_ran(toadox, toadoy);
    
    ve_ran(toadox, toadoy);
    
    int x = 50, y = 13; // d?nh hình v? trí c?n di chuy?n cho r?n
    int check = 2;
    
    while (gameover == false)
    {
        // xóa d? li?u cu
        xoa_du_lieu_cu(toadox, toadoy);
     
        // 0 : di xu?ng
        // 1 : di lên
        // 2 : qua ph?i
        // 3 : qua trái
        // di?u ki?n
        if (_kbhit())
        {
            char kitu = _getch();
            if (kitu == -32)
            {
                kitu = _getch();
                if (kitu == 72 && check != 0) //di lên
                {
                    check = 1;
                }
                else if (kitu == 80 && check != 1) // di xu?ng
                {
                    check = 0;
                }
                else if (kitu == 77 && check != 3) // di ph?i
                {
                    check = 2;
                }
                else if (kitu == 75 && check != 2) // qua trái
                {
                    check = 3;
                }
            }
        }
        
        // th?c hi?n di chuy?n
        if (check == 0)
        {
            y++; // di xu?ng
        }
        else if (check == 1)
        {
            y--; // di lên
        }
        else if (check == 2)
        {
            x++; // di qua ph?i
        }
        else if (check == 3)
        {
            x--; // di qua trái
        }
        
        // Ki?m tra va ch?m
        if (kiem_tra_va_cham(toadox, toadoy, x, y))
        {
            gotoXY(30, 30);
            cout << "Game Over! Press any key to exit...";
            _getch();
            break;
        }
        
        xu_ly_ran(toadox, toadoy, x, y);
        // ======== KiemTra =========
        gameover = kt_ran(toadox, toadoy)
        Sleep(150);
    }
    return 0;
}

// khu vuc cac ham cua game

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
            cout << "0"; // d?u r?n
        }
        else
        {
            cout << "o"; // thân r?n
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

void xu_ly_ran(int toadox[], int toadoy[], int x, int y)
{
    // b1: thêm t?a d? m?i vào d?u m?ng
    them(toadox, x);
    them(toadoy, y);
    // b2: xóa t?a d? cu?i m?ng
    xoa(toadox, s1 - 1);
    xoa(toadoy, s1 - 1);
    // b3: v? r?n
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
    for (int i = vt; i < s1 - 1; i++) // S?a di?u ki?n i < s1 - 1
    {
        a[i] = a[i + 1];
    }
    s1--;
}

// Hàm ki?m tra va ch?m v?i tu?ng và thân r?n
bool kiem_tra_va_cham(int toadox[], int toadoy[], int x, int y)
{
    // Ki?m tra va ch?m v?i tu?ng
    if (x <= 10 || x >= 100 || y <= 1 || y >= 26)
    {
        return true;
    }
    
    // Ki?m tra va ch?m v?i thân r?n
    for (int i = 0; i < s1; i++)
    {
        if (toadox[i] == x && toadoy[i] == y)
        {
            return true;
        }
    }
    
    return false;
}

bool kt_ran_cham_tuong(int x0, int y0)
{
    if(y0 == 1 && (x0 >= 10 && x0 <= 100)) // ran cham tuong tren
    {
    	return true; // gameover
	}
	else if(y0 == 26 && (x0 >= 10 && x0 <= 100)) // va cham tuong duoi
	{
		return true;
	}
	else if(x0 == 100 && (y0 >= 1 && y0 <= 26)) // va cham tuong trai
	{
		return true;
	}
	else if(x0 == 10 && (y0 >=1 && y0 <=26)) // va cham tuong phai
	{
		return true;
	}
	return false
}

bool kt_ran_cham_duoi(int toadox[],int toadoy[]) 
{
	for(int i = 1 ;i < sl; i++)
	{
		if((toadox[0] == toadox[i]) && (toadoy[0] == toadoy[i]))
		{
			return true; // gameover
		}
	}
	return false
}

bool kt_ran(int toadox[], int toadoy[]) // ham ktra ran can duoi va dam vao tuong
{
	bool kt1 = kt_ran_cham_duoi(toadox, toadoy); // gameover = true
    bool kt2 = kt_ran_cham_tuong(toadox[0], toadoy[0]); // gameover = false
    if(kt1 == true || kt2 == true
    {
        return true; // gameover
	}
	return false;
}

// srand(time(null)); reset seed random ngau nhien;
// rand()%(b - a + 1)+a; = random 1 so trong doan tu a den b;
// pham vi qua 11 <= xqua <= 99 va 2 <= yqua <= 25
// doi voi xqua = rand() % (99 - 11 + 1) + 11;
// doi voi yqua = rand() % (25 - 2 + 1) + 2;

void tao_qua(int &xqua, int &yqua)
{
	xqua = rand() % (99 - 11 + 1) + 11; //11 <= xqua <= 99
	yqua = rand() % (25 - 2 + 1) + 2; // 2 <= yqua <= 25
	
	gotoXY(xqua,yqua);
	cout << "-";
}
