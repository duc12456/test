#include <conio.h>
#include<iostream>
#include <windows.h>
#include "mylib.h"
#define MAX 100
using namespace std;
int s1 = 4;

// Khai báo hàm vẽ tường
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

int main()
{
    int toadox[MAX], toadoy[MAX];
    ve_tuong();
    khoi_tao_ran(toadox, toadoy);
    ve_ran(toadox, toadoy);
    int x = 50, y = 13; // định hình vị trí cần di chuyển cho rắn
    int check = 2;
    
    while (true)
    {
        // xóa dữ liệu cũ
        xoa_du_lieu_cu(toadox, toadoy);
     
        // 0 : đi xuống
        // 1 : đi lên
        // 2 : qua phải
        // 3 : qua trái
        // điều kiện
        if (_kbhit())
        {
            char kitu = _getch();
            if (kitu == -32)
            {
                kitu = _getch();
                if (kitu == 72 && check != 0) //đi lên
                {
                    check = 1;
                }
                else if (kitu == 80 && check != 1) // đi xuống
                {
                    check = 0;
                }
                else if (kitu == 77 && check != 3) // đi phải
                {
                    check = 2;
                }
                else if (kitu == 75 && check != 2) // qua trái
                {
                    check = 3;
                }
            }
        }
        
        // thực hiện di chuyển
        if (check == 0)
        {
            y++; // đi xuống
        }
        else if (check == 1)
        {
            y--; // đi lên
        }
        else if (check == 2)
        {
            x++; // đi qua phải
        }
        else if (check == 3)
        {
            x--; // đi qua trái
        }
        
        // Kiểm tra va chạm
        if (kiem_tra_va_cham(toadox, toadoy, x, y))
        {
            gotoXY(30, 30);
            cout << "Game Over! Press any key to exit...";
            _getch();
            break;
        }
        
        xu_ly_ran(toadox, toadoy, x, y);
        Sleep(150);
    }
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
            cout << "0"; // đầu rắn
        }
        else
        {
            cout << "o"; // thân rắn
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
    // b1: thêm tọa độ mới vào đầu mảng
    them(toadox, x);
    them(toadoy, y);
    // b2: xóa tọa độ cuối mảng
    xoa(toadox, s1 - 1);
    xoa(toadoy, s1 - 1);
    // b3: vẽ rắn
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
    for (int i = vt; i < s1 - 1; i++) // Sửa điều kiện i < s1 - 1
    {
        a[i] = a[i + 1];
    }
    s1--;
}

// Hàm kiểm tra va chạm với tường và thân rắn
bool kiem_tra_va_cham(int toadox[], int toadoy[], int x, int y)
{
    // Kiểm tra va chạm với tường
    if (x <= 10 || x >= 100 || y <= 1 || y >= 26)
    {
        return true;
    }
    
    // Kiểm tra va chạm với thân rắn
    for (int i = 0; i < s1; i++)
    {
        if (toadox[i] == x && toadoy[i] == y)
        {
            return true;
        }
    }
    
    return false;
}
