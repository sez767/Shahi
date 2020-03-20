#include "workGL.h"
#include <stdio.h>
#include <stdlib.h>
#include "gdiplusbrush.h"
#include "gdiplusgraphics.h"
#include "gdiplusgpstubs.h"
#include "gdipluspen.h"
#include "gdipluscolor.h"

#include "Chess.h"

Color Blue(0,0,255);
Color Green(0,255,0);
Color White(255,255,255);
Color Black(0,0,0);
Color SwKorr(240,227,176);//
Color Korr(153,102,0);//
Color GrBlu(191,191,255);//
Color WinFon(200,200,200);

// тут стандарни ГДИ функции для малювання прямокутникив, лыный

void Bar(float SX,float SY,float EX,float EY, Color clr)
{
    RectF rct(SX, SY, EX-SX, EY-SY);
    HDC hdc = GetDC(hwnd);
    Graphics g(hdc);
    SolidBrush  Brsh(clr);
    g.FillRectangle(&Brsh, rct);
    ReleaseDC(hwnd, hdc); // Освобождаем контекст устройства
}

void DrawPP(float x, float y){
    Bar(x, y, x + 2, y + 2, Green);
}

void RectAng(float SX, float SY, float EX, float EY, Color clr){
    HDC hdc = GetDC(hwnd);
    Graphics g(hdc);
    Pen      pen(clr,1);
    g.DrawRectangle(&pen, SX, SY, EX-SX, EY-SY);
    ReleaseDC(hwnd, hdc); // Освобождаем контекст устройства
}
void Draw(){
    Board.DrawDisRect(0,70);
	Shess_Draw();
}

TMemo10 memo(780, 530, 470, 148); // x,y,ширина, высота
void TMemo10::ClearBar()
{
    Bar(x, y, x + w, y + h, WinFon); //WinFon

// RectAng(x, y, x + w, y + h, цвет);
}

void TMemo10::Clear()
{
    ind = 0; tp = 0;
    ClearBar();
}



void TMemo10::print(const char* Format, ...)
{
    //----printfs
    va_list ArgList;
    __crt_va_start(ArgList, Format);
    char str[300];
    vsprintf_s(str, 300, Format, ArgList);
    __crt_va_end(ArgList);
    //
    ClearBar();

    if (ind < 100)strcpy(&string[ind++][0], str);
    else {
        memcpy(&string[0][0], &string[1][0], 99 * sizeof(string[0]));
        strcpy(&string[99][0], str);
        ind = 100;
    }

    if (ind > 7) { tp = ind - 7; }
    else { if (ind == 100)tp = 92; }

    for (int i = 0; i < 7; i++) {
        MyText(x + 4, y + i * dy + 4, string[tp + i]);
    }
    offset = 0;
}

void TMemo10::Draw()
{
    if (offset < 0)offset = 0;
    if (offset > 93)offset = 93;
    if ((tp - offset) < 0)offset = tp;
    ClearBar();
    for (int i = 0; i < 7; i++) {
        MyText(x + 4, y + i * dy + 4, string[tp + i - offset]);
    }
    //4 - это смещение текста относительно лев, верх прямоугольника фона
    
}
