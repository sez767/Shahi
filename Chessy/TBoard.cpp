#include "workGL.h"
#include "./ImTexture.h"
#include "Classes/TFigure.h"
#include "TBoard.h"

//возвращает номер(тип) фигуры, если >5 то нет ничего
int TBoard::WhoIs (int ix, int iy){   
	for(int i=0;i<5;i++){
		if (Fig[i]->Enbl==0)continue;
		if (Fig[i]->x == ix && Fig[i]->y == iy) return i; // ее номер
	}
	return 8; // нет фигур

}

void TBoard::CalcBMoves(){ // посчитать все ходы RP для черных
	Position5 p5;
	SetPosition(&p5); // берем з доски
	Fig[1]->CalcRP(&p5);
	Fig[2]->CalcRP(&p5);
	Fig[3]->CalcRP(&p5);
	Fig[4]->CalcRP(&p5);
	Fig[1]->KingCorrect(&p5);
}

void TBoard::CalcWMoves(){ // посчитать все RP для белых
	Position5 p5;
	SetPosition(&p5); // берем з доски
	Fig[0]->CalcRP(&p5);
	Fig[0]->KingCorrect(&p5);
}

// ОЦЕНКА....всегда делается относительно черных
int Evaluate(TBoard* b) {
	Position5 p5;
	b->SetPosition(&p5); // берем з доски
	// якшо эта позиция уже была
	// и для черных и для белых 
	if (FindPos(&p5)) {
		if (H == 1)return POS_WAS;
		else return -POS_WAS;
	}
	int v = 0;
	// материальный перевес
	if (b->Fig[0]->Enbl == 0)v += 10000; // нет белого короля
	if (b->Fig[1]->Enbl == 0)v -= 10000; // нет черного короля
	if (b->Fig[2]->Enbl)v += 30; // слон
	if (b->Fig[3]->Enbl)v += 50; // тура
	if (b->Fig[4]->Enbl)v += 30; // конь

	b->Fig[0]->CalcRP(&p5);
	b->Fig[0]->KingCorrect(&p5);
	// шах белому королю
	if (p5.OnOtherField(b->Fig[0]->x, b->Fig[0]->y, 0)) {
		// предвидение цейтнота белого короля для черных
		if (b->Fig[0]->Rcnt == 0)
			v += 100000; // это будет мат
		else
			v += 20;
	}
	else
	{
		// некуда ходить белому королю
		if (b->Fig[0]->Rcnt == 0) {
			if (H == -1)v = 1000000; // для хода черных
			else v = -1000000;
		}
	}
	// близость белого к черному королю
	int dd = abs(b->Fig[0]->x - b->Fig[1]->x) + abs(b->Fig[0]->y - b->Fig[1]->y);
	v += (14 - dd);//4

	// зажим белого короля черной ладьей
	if (b->Fig[3]->Enbl) {
		int S = 0;
		int xL = b->Fig[3]->x; int yL = b->Fig[3]->y; // ладья
		int xK = b->Fig[0]->x; int yK = b->Fig[0]->y; // король б
		if (xK < xL && yK < yL) {
			for (int x = 1; x < xL; x++)if (b->Fig[1]->x == x && b->Fig[1]->y == yL)break;
			for (int y = 1; y < yL; y++)if (b->Fig[1]->x == xL && b->Fig[1]->y == y)break;
			for (int x = 1; x < xL; x++)if (b->Fig[2]->x == x && b->Fig[2]->y == yL)break;
			for (int y = 1; y < yL; y++)if (b->Fig[2]->x == xL && b->Fig[2]->y == y)break;
			for (int x = 1; x < xL; x++)if (b->Fig[4]->x == x && b->Fig[4]->y == yL)break;
			for (int y = 1; y < yL; y++)if (b->Fig[4]->x == xL && b->Fig[4]->y == y)break;

			S = xL * yL; // left-top
		}
		if (xK > xL&& yK < yL) {
			for (int x = xL + 1; x < 7; x++)if (b->Fig[1]->x == x && b->Fig[1]->y == yL)break;
			for (int y = 0; y < yL; y++)if (b->Fig[1]->x == xL && b->Fig[1]->y == y)break;
			for (int x = xL + 1; x < 7; x++)if (b->Fig[2]->x == x && b->Fig[2]->y == yL)break;
			for (int y = 0; y < yL; y++)if (b->Fig[2]->x == xL && b->Fig[2]->y == y)break;
			for (int x = xL + 1; x < 7; x++)if (b->Fig[4]->x == x && b->Fig[4]->y == yL)break;
			for (int y = 0; y < yL; y++)if (b->Fig[4]->x == xL && b->Fig[4]->y == y)break;

			S = (7 - xL) * yL; // right-top
		}
		if (xK > xL&& yK > yL) {
			for (int x = xL + 1; x < 7; x++)if (b->Fig[1]->x == x && b->Fig[1]->y == yL)break;
			for (int y = yL + 1; y < 7; y++)if (b->Fig[1]->x == xL && b->Fig[1]->y == y)break;
			for (int x = xL + 1; x < 7; x++)if (b->Fig[2]->x == x && b->Fig[2]->y == yL)break;
			for (int y = yL + 1; y < 7; y++)if (b->Fig[2]->x == xL && b->Fig[2]->y == y)break;
			for (int x = xL + 1; x < 7; x++)if (b->Fig[4]->x == x && b->Fig[4]->y == yL)break;
			for (int y = yL + 1; y < 7; y++)if (b->Fig[4]->x == xL && b->Fig[4]->y == y)break;

			S = (7 - xL) * (7 - yL); // right-down
		}
		if (xK<xL && yK>yL) {
			for (int x = 1; x < xL; x++)if (b->Fig[1]->x == x && b->Fig[1]->y == yL)break;
			for (int y = yL + 1; y < 7; y++)if (b->Fig[1]->x == xL && b->Fig[1]->y == y)break;
			for (int x = 1; x < xL; x++)if (b->Fig[2]->x == x && b->Fig[2]->y == yL)break;
			for (int y = yL + 1; y < 7; y++)if (b->Fig[2]->x == xL && b->Fig[2]->y == y)break;
			for (int x = 1; x < xL; x++)if (b->Fig[4]->x == x && b->Fig[4]->y == yL)break;
			for (int y = yL + 1; y < 7; y++)if (b->Fig[4]->x == xL && b->Fig[4]->y == y)break;

			S = xL * (7 - yL); // left-down
		}
		int qq = (49 - S) / 3;//2
		if (S > 0)v += qq;
	}

	//не под ударом ли черные фигуры
	if (H == 1) {
		if (p5.OnOtherField(b->Fig[2]->x, b->Fig[2]->y, 2))// слон под ударом 
			if (p5.NoProtected(b->Fig[2]->x, b->Fig[2]->y, 2))v -= 30;// фигура не защищена

		if (p5.OnOtherField(b->Fig[3]->x, b->Fig[3]->y, 3))// тура
			if (p5.NoProtected(b->Fig[3]->x, b->Fig[3]->y, 3))v -= 50;

		if (p5.OnOtherField(b->Fig[4]->x, b->Fig[4]->y, 4))// конь
			if (p5.NoProtected(b->Fig[4]->x, b->Fig[4]->y, 4))v -= 30;
	}

	return v;
}