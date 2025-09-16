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
bool kt_ran_cham_tuong(int x0, int y0);
bool kt_ran_cham_duoi(int toadox[], int toadoy[]);
bool kt_ran(int toadox[], int toadoy[]);
void tao_qua(int &xqua, int &yqua);

// ham main
int main()
{
    srand(time(NULL)); // Fixed: NULL instead of Null
    int xqua = 0, yqua = 0;
    
    tao_qua(xqua, yqua);
    
    bool gameover = false;
    
    int toadox[MAX], toadoy[MAX];
    ve_tuong();
    
    khoi_tao_ran(toadox, toadoy);
    
    ve_ran(toadox, toadoy);
    
    int x = 50, y = 13; // định hướng vị trí cần di chuyển cho rắn
    int check = 2;
    
    while (gameover == false)
    {
        // xóa dữ liệu cũ
        xoa_du_lieu_cu(toadox, toadoy);
     
        // 0 : di xuống
        // 1 : di lên
        // 2 : qua phải
        // 3 : qua trái
        // điều kiện
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
                else if (kitu == 80 && check != 1) // di xuống
                {
                    check = 0;
                }
                else if (kitu == 77 && check != 3) // di phải
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
            y++; // di xuống
        }
        else if (check == 1)
        {
            y--; // di lên
        }
        else if (check == 2)
        {
            x++; // di qua phải
        }
        else if (check == 3)
        {
            x--; // di qua trái
        }
        
        // Kiểm tra va chạm - có thể bỏ phần này nếu không cần
        
        xu_ly_ran(toadox, toadoy, x, y);
        // ======== KiemTra =========
        gameover = kt_ran(toadox, toadoy); // Fixed: Missing semicolon
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
    return false; // Fixed: Missing semicolon
}

bool kt_ran_cham_duoi(int toadox[], int toadoy[]) 
{
    for(int i = 1; i < s1; i++) // Fixed: sl -> s1
    {
        if((toadox[0] == toadox[i]) && (toadoy[0] == toadoy[i]))
        {
            return true; // gameover
        }
    }
    return false; // Fixed: Missing semicolon
}

bool kt_ran(int toadox[], int toadoy[]) // ham ktra ran can duoi va dam vao tuong
{
    bool kt1 = kt_ran_cham_duoi(toadox, toadoy); // gameover = true
    bool kt2 = kt_ran_cham_tuong(toadox[0], toadoy[0]); // gameover = false
    if(kt1 == true || kt2 == true) // Fixed: Missing closing parenthesis
    {
        return true; // gameover
    }
    return false;
}

void tao_qua(int &xqua, int &yqua)
{
    xqua = rand() % (99 - 11 + 1) + 11; //11 <= xqua <= 99
    yqua = rand() % (25 - 2 + 1) + 2; // 2 <= yqua <= 25
    
    gotoXY(xqua, yqua);
    cout << "-";
}
