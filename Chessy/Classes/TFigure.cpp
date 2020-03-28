#include "../workGL.h"
#include <stdio.h>
#include <math.h>
#include "../TBoard.h"

void TFigure::DrawRP(){ //рисуем поля ходов
	if (Enbl == 0)return;
	for (int i = 0; i < Rcnt; i++)
		Board.Square(RP[i].x, RP[i].y);
}

void TFigure::DrawCF(){ //рисуем поля контроля
	if (Enbl == 0)return;
	for (int i = 0; i < Ccnt; i++)
		Board.Square1(CF[i].x, CF[i].y);

}

bool TFigure::On_RP_CF(int xx, int yy){
	for(int i=0;i<Rcnt;i++){
		if(xx==RP[i].x && yy==RP[i].y)
			return true;
	}
	for(int i=0;i<Ccnt;i++){
		if(xx==CF[i].x && yy==CF[i].y)
			return true;
	}
	return false;
}

// выкидание полей, контролируемых вражескими фигурами (RP+CF) 
void TFigure::KingCorrect(Position5* p){
	if (Enbl == 0)return;
	int c = 0;
	for(int i=0;i<Rcnt;i++){
		if (p->OnOtherField(RP[i].x, RP[i].y, num))
			RP[i].x = 8;// маркер, что выкинуть
	}
	FMove RP_t[14];
	for(int i=0;i<Rcnt;i++)
		RP_t[i]=RP[i];
	for(int i=0;i<Rcnt;i++)
		if(RP_t[i].x!=8){
			RP[c].Set(RP_t[i].x, RP_t[i].y);
			c++;
		} 
	Rcnt=c;
}

int TFigure::OverlapOther(){ // место фигуры совпадает с другой! (для рандомн тестирования)
	for(int i=0;i<5;i++){
		if (i == num)continue;
		if (x == Board.Fig[i]->x && y == Board.Fig[i]->y) return 1;
	}
	return 0;
}

