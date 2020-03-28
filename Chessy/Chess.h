#pragma once
#include "gdiplusbrush.h"
#include "gdiplusgraphics.h"
#include "gdiplusgpstubs.h"
#include "gdipluspen.h"
#include "./ImTexture.h"

#define PAWN			0	
#define KNIGHT			1   // конь
#define BISHOP			2	// слон
#define ROOK			3	// тура  
#define QUEEN			4	
#define KING			5	// КОРОЛЬ
#define EMPTY			6

#define width 1300 //размеры окна програмки
#define height 800

#define MIN_SCOR -19999999 //мин
#define MAX_SCOR 19999999 //макс
#define POS_WAS 10000000 // для двух ходов

extern ImgTexture SlonBl_txr;
extern ImgTexture TuraBl_txr;
extern ImgTexture KonjBl_txr;
extern ImgTexture KingBl_txr;
extern ImgTexture KingWt_txr;

extern int tt_cnt;//кол полуходов
extern int matFlg;//флаг для блока ходов после мата+
extern int count;//флаг ходов для лога
extern int ind;

struct FMove{ // координаты фигуры, также ход для известной фигуры
	int x, y;
	FMove(int ix, int iy){
		x = ix;
		y = iy;
	}
	FMove() {}; //конструктор по умолч
	void Set(int ix, int iy){ 
		x = ix; 
		y = iy;
	}
};

struct Position5{
	int x[5]; // координаты для жесткого массива фигур,
	int y[5]; // определенного в Fig[5]
	bool IsHere(int ix, int iy); // есть ли здесь стоит (пересечение с фигурами на доске)
	int WhoIs (int ix, int iy); // кто здесь, какая фигура  если >5 то нет ничего
	///// под ударом или под контролем других фигур противника
	bool OnOtherField(int ix, int iy, int no); //под контролем фигур противника
	bool NoProtected(int ix, int iy, int no); // поле не защищено своими
	bool IsEqual(Position5* ps);
};

// ход фигуры i - это ее новые координаты 
struct TMove{
	int i;
	int x, y;
};

TMove BestMove2();
void Move(TMove* pM);
int MakeNextMove();
int randInt(int min, int max);
void RandPosition();
void SavePos();
bool FindPos(Position5* ps);
void Make500Move();
void TestShess100_950();
void BackMove();
void ShowConsoleWindow();//
void PrintRow(int x, int y);
void MyText(int x, int y, const char* txt);
void MyTextBIG(int x, int y, const char* txt);
void MyTextKor(int x, int y, const char* txt);