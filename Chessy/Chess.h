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