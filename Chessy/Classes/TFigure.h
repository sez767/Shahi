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
