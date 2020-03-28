#include "workGL.h"
#include "./ImTexture.h"
#include "Classes/TFigure.h"
#include "Classes/TKing.h"
#include "Classes/TSlon.h"
#include "Classes/TTura.h"
#include "Classes/TKonj.h"

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

int Evaluate(TBoard* b);