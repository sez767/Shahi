#include "workGL.h"

#include "gdiplusbrush.h"
#include "gdiplusgraphics.h"
#include "gdiplusgpstubs.h"
#include "gdipluspen.h"
#include "gdipluscolor.h"

#include <stdio.h>
#include <stdlib.h>

#include "Chess.h"

// тут стандарни ГДИ функции для малювання прямокутникив, текста, и поля МЕМО (для лога)

Color Blue(0,0,255);
Color Green(0,255,0);
Color White(255,255,255);
Color Black(0,0,0);
Color SwKorr(240,227,176);//
Color Korr(153,102,0);//
Color GrBlu(191,191,255);//
Color WinFon(200,200,200);
Color ClrRamka(255, 100, 100, 255);
Color ClrRmkFon(255, 240, 240, 240);
Color GrBluBb(100, 255, 0, 0);
Color GrGreen(100, 0, 255, 0);

void Bar(float SX,float SY,float EX,float EY, Color clr){
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
    ReleaseDC(hwnd, hdc);
}
void Draw(){
    Board.DrawDisRect(0,70);
	Shess_Draw();
}

TMemo10 memo(897, 520, 360, 128); // x,y,ширина,высота

void TMemo10::ClearBar(){
    Bar(x, y, x + w, y + h, ClrRmkFon); //WinFon
RectAng(x, y, x + w, y + h, SwKorr);
}

void TMemo10::Clear(){
    ind = 0; tp = 0;
    ClearBar();
}

void TMemo10::print(const char* Format, ...){
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

    if (ind > 5) { 
        tp = ind - 5;
    }
    else {
        if (ind == 100) tp = 92; 
    }

    for (int i = 0; i < 7; i++) {
        MyText(x + 4, y + i * dy + 4, string[tp + i]);
    }
    offset = 0;
}

void TMemo10::Draw(){
    if (offset < 0)offset = 0;
    if (offset > 93)offset = 93;
    if ((tp - offset) < 0)offset = tp;
    ClearBar();
    for (int i = 0; i < 5; i++) {
        MyText(x + 4, y + i * dy + 4, string[tp + i - offset]);
    }
    //4 - это смещение текста относительно лев, верх
    
}
void PrintRow(int x, int y) {
    int dy = 91;
    MyTextKor(x, y, "1");
    MyTextKor(x, y += dy, "2");
    MyTextKor(x, y += dy, "3");
    MyTextKor(x, y += dy, "4");
    MyTextKor(x, y += dy, "5");
    MyTextKor(x, y += dy, "6");
    MyTextKor(x, y += dy, "7");
    MyTextKor(x, y += dy, "8");
}
void MyText(int x, int y, const char* txt) {
    HDC hDC = GetDC(hwnd);
    SelectObject(hDC, font);
    SetTextColor(hDC, RGB(0, 0, 0)); //цвет текста
    //SetBkColor(hDC,RGB(0,255,255));// фон, если не использовать SetBkMode !
    SetBkMode(hDC, TRANSPARENT);
    TextOutA(hDC, x, y, txt, strlen(txt));
    ReleaseDC(hwnd, hDC); 
}
void MyTextBl(int x, int y, const char* txt) {
    HDC hDC = GetDC(hwnd);
    SelectObject(hDC, font);
    SetTextColor(hDC, RGB(13, 86, 166)); //цвет текста
    //SetBkColor(hDC,RGB(0,255,255));// фон, если не использовать SetBkMode !
    SetBkMode(hDC, TRANSPARENT);
    TextOutA(hDC, x, y, txt, strlen(txt));
    ReleaseDC(hwnd, hDC); 
}
void MyTextBIG(int x, int y, const char* txt) {
    HDC hDC = GetDC(hwnd);
    SelectObject(hDC, fontBIG);
    SetTextColor(hDC, RGB(13, 86, 166));// цвет текста
                                 //SetBkColor(hDC,RGB(0,255,255));// фон, если не использовать SetBkMode !
    SetBkMode(hDC, TRANSPARENT);
    TextOutA(hDC, x, y, txt, strlen(txt));
    ReleaseDC(hwnd, hDC); 

}
void MyTextKor(int x, int y, const char* txt) {
    HDC hDC = GetDC(hwnd);
    SelectObject(hDC, fontKor);
    SetTextColor(hDC, RGB(153, 102, 0));//цвет текста
                                 //SetBkColor(hDC,RGB(0,255,255));// фон, если не использовать SetBkMode !
    SetBkMode(hDC, TRANSPARENT);
    TextOutA(hDC, x, y, txt, strlen(txt));
    ReleaseDC(hwnd, hDC); // Освобождаем контекст устройства

}
// фоны
HFONT font = CreateFont(20, 0, 0, 0,
    FW_BOLD, FALSE, FALSE, FALSE,
    ANSI_CHARSET, OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
    DEFAULT_PITCH | FF_ROMAN,
    "Arial");

HFONT fontBIG = CreateFont(55, 0, 0, 0,
    FW_BOLD, FALSE, FALSE, FALSE,
    ANSI_CHARSET, OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
    DEFAULT_PITCH | FF_ROMAN,
    "Arial");

HFONT fontKor = CreateFont(16, 16, 0, 0,
    FW_BOLD, FALSE, FALSE, FALSE,
    ANSI_CHARSET, OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
    DEFAULT_PITCH | FF_ROMAN,
    "Arial");