#pragma once
#include "gdiplusbrush.h"
#include "gdiplusgraphics.h"
#include "gdiplusgpstubs.h"
#include "gdipluspen.h"
#include "../ImTexture.h"
#include "../workGL.h"
#include "../Chess.h"

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
	void SetPos(float xf, float yf){ x = xf,y = yf;}

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
