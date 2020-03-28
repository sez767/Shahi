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

class TFigure{
public:
	int x, y; // координаты 0-7
	int Type; // // 5 - king, 3-tura, 2-slon, 1-kon
	int color=0; // 0 -white, 1-black
	int Enbl=0; // есть фигура или ее нет
	ImgTexture* Img; // картинка данной фигуры
	int num; // номер фигуры в условленом массиве фигур условленного порядка
	FMove RP[14]; // возможные координаты (ходы)
	int Rcnt; // их количество
	FMove CF[3]; // control field - поля контроля (нужно для короля, у нас только для белого)
	int Ccnt; // их количество, они будут высчитываться в CalcRP
	//малюем
	void Draw(float sz, float x0, float y0 ) { Img->Print(x0+x*sz+sz/2, y0+y*sz+sz/2); };
	void DrawDis(float sz,float x0, float y0 ) { Img->Print(x0+9*sz, y0+num*sz+sz/2); };
	void DrawRP();
	void DrawCF();
	bool On_RP_CF(int xx, int yy); 
	void KingCorrect(Position5* p);
	virtual void CalcRP(const Position5* p){} // дает реально возможные ходы из данной позиции
	int OverlapOther(); // место фигуры совпадает с другой (для рандомн тестирования)
	void SetPos(float xf, float yf){ 
		x = xf,
		y = yf;
	}
	TFigure(){
	num = ind++;
	};
	TFigure(ImgTexture* it){ 
		Img = it;
		x = y = 0;
		num = ind++; 
	};
	TFigure(ImgTexture* it, int col){
		Img = it;
		x = y = 0;
		color = col;
		num = ind++;};
	~TFigure(){};
private:
};


class TKing : public TFigure{
public:
	void CalcRP(const Position5* p); // дает реально возможные ходы из данной позиции
	TKing() :TFigure(&KingWt_txr) { Type = KING; }
	TKing(ImgTexture* it, int cl):TFigure(it,cl){Type = KING;}
	~TKing(){}
private:
};

class TSlon : public TFigure{
public:
	void CalcRP(const Position5* p); // дает реально возможные ходы из данной позиции
	TSlon() :TFigure(&SlonBl_txr) { color = 1; Type=BISHOP; }
	~TSlon(){Type=BISHOP;}
private:
};

class TTura : public TFigure{
public:
	void CalcRP(const Position5* p); // дает реально возможные ходы из данной позиции
	TTura() :TFigure(&TuraBl_txr) { color = 1; Type = ROOK; }
	~TTura(){Type = ROOK;}
private:
};

class TKonj : public TFigure{
public:
	void CalcRP(const Position5* p); // дает реально возможные ходы из данной позиции
	TKonj() :TFigure(&KonjBl_txr) { color = 1; Type = KNIGHT; }
	~TKonj(){Type = KNIGHT;}
private:
};

struct TBoard{
	Color color_white=SwKorr;//коричневая красивее
	Color color_black=Korr;
	float x0, y0; // координаты лев верх края доски
	float Size; // размер клетки
	TFigure* Fig[5];
	void GetPosition(Position5 &p);
	void SetPosition(Position5 *p);
	int Count; // колич фигур на доске
	void setPos(float x, float y) {
		x0 = x;
		y0 = y;
	}
	bool InDisRect(int mx, int my, int i); // мыша в квадрате i-й фигуры сбоку
	bool InBoardRect(int mx, int my); // мыша на доске
	void DrawDisRect(float x, float y);
	int FindFigOnMouse(int mx, int my); // номер фигуры под мышей (в Fig[5])
	void SetFigCoord(int mx, int my, int i);
	void CalcBMoves(); // посчитать все RP для черных
	void CalcWMoves(); // посчитать все RP для белых
	int WhoIs (int ix, int iy); // кто здесь, какая фигура  если >5 то нет ничего
	void OnStart() { 
		for (int i = 0; i < Count; i++)
			Fig[i]->Enbl = 0;
	}
	void InitColor(){
		color_white=SwKorr;
		color_black=Korr;
	}
	void DrawBoard();
	int Koroli();
	void Square(int sx, int sy);
	void Square1(int sx, int sy);
	void Draw(float dx, float dy){
		for (int i = 0; i < Count; i++)
			if (Fig[i]->Enbl)Fig[i]->Draw(Size, x0, y0);
			else Fig[i]->DrawDis(Size,x0+dx,y0+dy);
	};

	TBoard() {
		x0 = y0 = 10;
		Size = 90;
	}
	TBoard(TFigure* fg[], int cnt) { 
		Count = cnt;
		for (int i = 0; i < Count; i++)
			Fig[i] = fg[i]; }

	TBoard(TFigure* fg[], int cnt, float sz, float x, float y){
		Count = cnt;
		for (int i = 0; i < Count; i++)
			Fig[i]=fg[i];
		Size = sz;
		x0 = x; y0 = y;
		color_white=SwKorr;
		color_black=Korr;
	}

};

extern int H; // чей текущий ход (-1)-белых, +1-черных
extern TBoard Board;
extern TKing WKing; 
extern TKing BKing;
extern TSlon BSlon;
extern TTura BTura;
extern TKonj BKonj;
extern int Pmcnt;
TMove BestMove2();
int Evaluate(TBoard* b);
void Move(TMove* pM);
int MakeNextMove();
void RandPosition();
void SavePos();
bool FindPos(Position5* ps);
void Make500Move();
void TestShess100_950();
void BackMove();
void ShowConsoleWindow();//
void PrintRow(int x, int y);
int randInt(int min, int max);
void MyText(int x, int y, const char* txt);
void MyTextBIG(int x, int y, const char* txt);
void MyTextKor(int x, int y, const char* txt);