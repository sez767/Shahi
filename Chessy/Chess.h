#pragma once

#include "gdiplusbrush.h"
#include "gdiplusgraphics.h"
#include "gdiplusgpstubs.h"
#include "gdipluspen.h"
#include "./ImTexture.h"

#define PAWN			0	
#define KNIGHT			1   // ����
#define BISHOP			2	// ����
#define ROOK			3	// ����  
#define QUEEN			4	
#define KING			5	// ������
#define EMPTY			6

#define width 1300 //������� ���� ���������
#define height 800

#define MIN_SCOR -19999999 //���
#define MAX_SCOR 19999999 //����
#define POS_WAS 10000000 // ��� ���� �����

extern ImgTexture SlonBl_txr;
extern ImgTexture TuraBl_txr;
extern ImgTexture KonjBl_txr;
extern ImgTexture KingBl_txr;
extern ImgTexture KingWt_txr;

extern int tt_cnt;//��� ���������
extern int matFlg;//���� ��� ����� ����� ����� ����+
extern int count;//���� ����� ��� ����
extern int ind;

struct FMove{ // ���������� ������, ����� ��� ��� ��������� ������
	int x, y;
	FMove(int ix, int iy){
		x = ix;
		y = iy;
	}
	FMove() {}; //����������� �� �����
	void Set(int ix, int iy){ 
		x = ix; 
		y = iy;
	}
};

struct Position5{
	int x[5]; // ���������� ��� �������� ������� �����,
	int y[5]; // ������������� � Fig[5]
	bool IsHere(int ix, int iy); // ���� �� ����� ����� (����������� � �������� �� �����)
	int WhoIs (int ix, int iy); // ��� �����, ����� ������  ���� >5 �� ��� ������
	///// ��� ������ ��� ��� ��������� ������ ����� ����������
	bool OnOtherField(int ix, int iy, int no); //��� ��������� ����� ����������
	bool NoProtected(int ix, int iy, int no); // ���� �� �������� ������
	bool IsEqual(Position5* ps);
};

// ��� ������ i - ��� �� ����� ���������� 
struct TMove{
	int i;
	int x, y;
};

class TFigure{
public:
	int x, y; // ���������� 0-7
	int Type; // // 5 - king, 3-tura, 2-slon, 1-kon
	int color=0; // 0 -white, 1-black
	int Enbl=0; // ���� ������ ��� �� ���
	ImgTexture* Img; // �������� ������ ������
	int num; // ����� ������ � ���������� ������� ����� ������������ �������
	FMove RP[14]; // ��������� ���������� (����)
	int Rcnt; // �� ����������
	FMove CF[3]; // control field - ���� �������� (����� ��� ������, � ��� ������ ��� ������)
	int Ccnt; // �� ����������, ��� ����� ������������� � CalcRP
	//������
	void Draw(float sz, float x0, float y0 ) { Img->Print(x0+x*sz+sz/2, y0+y*sz+sz/2); };
	void DrawDis(float sz,float x0, float y0 ) { Img->Print(x0+9*sz, y0+num*sz+sz/2); };
	void DrawRP();
	void DrawCF();
	bool On_RP_CF(int xx, int yy); 
	void KingCorrect(Position5* p);
	virtual void CalcRP(const Position5* p){} // ���� ������� ��������� ���� �� ������ �������
	int OverlapOther(); // ����� ������ ��������� � ������ (��� ������� ������������)
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
	void CalcRP(const Position5* p); // ���� ������� ��������� ���� �� ������ �������
	TKing() :TFigure(&KingWt_txr) { Type = KING; }
	TKing(ImgTexture* it, int cl):TFigure(it,cl){Type = KING;}
	~TKing(){}
private:
};

class TSlon : public TFigure{
public:
	void CalcRP(const Position5* p); // ���� ������� ��������� ���� �� ������ �������
	TSlon() :TFigure(&SlonBl_txr) { color = 1; Type=BISHOP; }
	~TSlon(){Type=BISHOP;}
private:
};

class TTura : public TFigure{
public:
	void CalcRP(const Position5* p); // ���� ������� ��������� ���� �� ������ �������
	TTura() :TFigure(&TuraBl_txr) { color = 1; Type = ROOK; }
	~TTura(){Type = ROOK;}
private:
};

class TKonj : public TFigure{
public:
	void CalcRP(const Position5* p); // ���� ������� ��������� ���� �� ������ �������
	TKonj() :TFigure(&KonjBl_txr) { color = 1; Type = KNIGHT; }
	~TKonj(){Type = KNIGHT;}
private:
};

struct TBoard{
	Color color_white=SwKorr;//���������� ��������
	Color color_black=Korr;
	float x0, y0; // ���������� ��� ���� ���� �����
	float Size; // ������ ������
	TFigure* Fig[5];
	void GetPosition(Position5 &p);
	void SetPosition(Position5 *p);
	int Count; // ����� ����� �� �����
	void setPos(float x, float y) {
		x0 = x;
		y0 = y;
	}
	bool InDisRect(int mx, int my, int i); // ���� � �������� i-� ������ �����
	bool InBoardRect(int mx, int my); // ���� �� �����
	void DrawDisRect(float x, float y);
	int FindFigOnMouse(int mx, int my); // ����� ������ ��� ����� (� Fig[5])
	void SetFigCoord(int mx, int my, int i);
	void CalcBMoves(); // ��������� ��� RP ��� ������
	void CalcWMoves(); // ��������� ��� RP ��� �����
	int WhoIs (int ix, int iy); // ��� �����, ����� ������  ���� >5 �� ��� ������
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

extern int H; // ��� ������� ��� (-1)-�����, +1-������
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