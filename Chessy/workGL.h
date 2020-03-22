#pragma once

//#include "targetver.h"
#include <windows.h>
/*Основные функции OpenGL*/
///#include <GL/gl.h>
/*Отключение дополнительные функции OpenGL*/
///#include <GL/glu.h>
/*подключение библиотеки OpenGL*/
///#pragma comment(lib,"OpenGL32.lib")

#pragma comment(lib,"Gdi32.lib")

// Файлы заголовков C RunTime
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <gdiplus.h>

extern HWND hwnd;

using namespace Gdiplus;

void Draw();

extern Color Blue;
extern Color White;
extern Color Black;
extern Color Green;
extern Color Red;
extern Color Korr;
extern Color SwKorr;
extern Color GrBlu;
extern Color WinFon;
extern Color ClrRamka;
extern Color ClrRmkFon;
extern Color GrBluBb;
extern Color GrGreen;

extern int Stop;

void Bar(float SX, float SY, float EX, float EY, Color clr);
void RectAng(float SX, float SY, float EX, float EY, Color clr);
void DrawPP(float x, float y);

void Shess_init();
void init_img();
void Shess_Draw();
void MyText(int x, int y, const char* txt);
void MyTextBIG(int x, int y, const char* txt);
void MyTextKor(int x, int y, const char* txt);
void MyTextBl(int x, int y, const char* txt);
void PrintHelpInWindow(int x, int y);
void DrawFonRect();
void PrintRow(int x, int y);




