#include "workGL.h"
#include <stdio.h>
#include <math.h>
#include "./ImTexture.h"
#include "Chess.h"

ImgTexture SlonBl_txr;
ImgTexture TuraBl_txr;
ImgTexture KonjBl_txr;
ImgTexture KingBl_txr;
ImgTexture KingWt_txr;
TKing WKing;
TKing BKing(&KingBl_txr, 1);
TSlon BSlon;
TTura BTura;
TKonj BKonj;
TFigure* FG[] = { &WKing, &BKing, &BSlon, &BTura, &BKonj };
TBoard Board(FG, 5, 90, 10, 10);
Position5 Pmem[1000];
int Pmcnt;
int ind;

void init_img(){
	KingBl_txr.LoadFromFile(L"src/King_bl.png");
	SlonBl_txr.LoadFromFile(L"src/Slon_bl.png");
	TuraBl_txr.LoadFromFile(L"src/Tura_bl.png");
	KonjBl_txr.LoadFromFile(L"src/Konj_bl.png");
	KingWt_txr.LoadFromFile(L"src/King_wt.png");
}

void TBoard::DrawBoard(){
	for(int i=0; i<8;i+=2){//малюем доску, 8 на 8, через одну
		for(int h=0; h<8;h+=2){
			Bar(x0 + i * Size, y0 + h * Size, x0 + (i + 1) * Size, y0 + (h + 1) * Size, color_white);
			Bar(x0 + (i+1) * Size, y0 + (h+1) * Size, x0 + (i + 2) * Size, y0 + (h + 2) * Size, color_white);
			Bar(x0 + (i+1) * Size, y0 + h * Size, x0 + (i + 2) * Size, y0 + (h + 1) * Size, color_black);
			Bar(x0 + i * Size, y0 + (h+1) * Size, x0 + (i + 1) * Size, y0 + (h + 2) * Size, color_black);

		}
	}
	RectAng(x0-2, y0-2, x0+Size*8+2, y0+Size*8+2, Korr);//рамочка =)
}

bool TBoard::InDisRect(int mx, int my, int i){ // бокова область з фигурами
	my -= 70;
	float num = Fig[i]->num;
	if (mx >= (x0 + 8.5 * Size) && mx < (x0 + 9.5 * Size))
		if (my >= (y0 + num * Size ) && my < (y0 + (num+1) * Size ))
			return true;
	return false;
}

bool TBoard::InBoardRect(int mx, int my){ // мыша на доске
	if (mx > x0  &&  mx < (x0 + 8 * Size))
		if (my > y0  && my < (y0 + 8 * Size))
			return true;
	return false;

}
void TBoard::DrawDisRect(float x, float y){// рисуем боковую область з фигурами
	Bar(x+x0 + 8.5 * Size, y+y0 ,  x+x0 + 9.5 * Size, y+y0 + 5*Size, SwKorr);
}

// номер фигуры под мышей (в Fig[5])
int TBoard::FindFigOnMouse(int mx, int my){
	for(int i=0; i<5;i++){
		if (InDisRect(mx, my, i) && (Fig[i]->Enbl==0))return i;
	}
	return 7;
}

void TBoard::SetFigCoord(int mx, int my, int i){ //ее коор на главной доске
	Fig[i]->x = (mx-x0)/Size;
	Fig[i]->y = (my-y0)/Size;
	Fig[i]->Enbl = 1;
}

// берем координаты фигур из позиции p на доску (для отображения и управления) 
void TBoard::GetPosition(Position5& p){
	for (int i = 0; i < Count; i++) {
		if (p.x[i] > 7) { Fig[i]->Enbl = 0; continue; }
		Fig[i]->x = p.x[i];
		Fig[i]->y = p.y[i];
		Fig[i]->Enbl = 1;

	}
}

// берем позицию из доски в переменную p
void TBoard::SetPosition(Position5* p){
	for (int i = 0; i < Count; i++) {
		if(Fig[i]->Enbl==0){p->x[i]=8; continue;}
		p->x[i] = Fig[i]->x;
		p->y[i] = Fig[i]->y;
	}
}

void TBoard::Square(int sx, int sy){
	Bar(x0+sx*Size, y0+sy*Size, x0+(sx+1)*Size, y0+(sy+1)*Size,GrBluBb);
}

void TBoard::Square1(int sx, int sy){
	Bar(x0+sx*Size, y0+sy*Size, x0+(sx+1)*Size, y0+(sy+1)*Size,GrGreen);
}

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

void Shess_init(){
	Board.InitColor();
	BKing.SetPos(0, 3);
	BSlon.SetPos(1, 4);
	BTura.SetPos(5, 3);
	BKonj.SetPos(4,4);
}

void DrawFonRect(){
	Bar(840, 590, 1290, 650, WinFon);
}

void Shess_Draw(){
	Board.DrawBoard();
	Board.Draw(0,70);
	if(H==1){Bar(800,20,830,50,Black);}
	else {Bar(800,20,830,50,White); }
}

int randInt(int min, int max){
	//srand(time(0));  //висне!!!!!
	int r = rand() % (max - min + 1) + min;
	return r;
}

int TFigure::OverlapOther(){ // место фигуры совпадает с другой! (для рандомн тестирования)
		for(int i=0;i<5;i++){
		if (i == num)continue;
		if (x == Board.Fig[i]->x && y == Board.Fig[i]->y) return 1;
	}
	return 0;
}

int PositionBad(){ //тоже для рандома
	for(int i=0;i<5;i++){
		if (Board.Fig[i]->OverlapOther())return 1;
	}
	return 0;
}

void RandPosition(){ //рандом
	H = -1;
	do{ 
		WKing.x = randInt(0, 7); WKing.y = randInt(0, 7); WKing.Enbl = 1;
		BKing.x = randInt(0, 7); BKing.y = randInt(0, 7); BKing.Enbl = 1;
		BSlon.x = randInt(0, 7); BSlon.y = randInt(0, 7); BSlon.Enbl = 1;
		BTura.x = randInt(0, 7); BTura.y = randInt(0, 7); BTura.Enbl = 1;
		BKonj.x = randInt(0, 7); BKonj.y = randInt(0, 7); BKonj.Enbl = 1;
	}while (PositionBad());

}

bool Position5::IsEqual(Position5* ps){
	for (int i = 0; i < 5; i++){
		if (ps->x[i] > 7)continue;
		if (x[i] != ps->x[i])return false;
		if (y[i] != ps->y[i])return false;
	}
	return true;

}

void SavePos(){
	Board.SetPosition(&Pmem[Pmcnt++]);
}

bool FindPos(Position5* ps){
	for (int i = 0; i < Pmcnt; i++){
		//if (ps->x[i] > 7)continue;
		if (ps->IsEqual(&Pmem[i]))return true;
	}
	return false;
}

void BackMove(){ //отмена хода
	if (Pmcnt < 2)return;
	Pmcnt -= 2;
	Board.GetPosition(Pmem[Pmcnt++]);
}

void PrintHelpInWindow(int x, int y){
	int dy = 29; // расстояние между строками
	char t0[] = "\n                      WELCOME";  MyText(x, y += dy, t0);
	char t1[]="\nDRUG&DROP chessmans on board";  MyText(x, y+=dy, t1);
	char t17[] = "\nSPACE - make move"; MyText(x, y += dy, t17);
	char t8[] = "\nR - random position";  MyText(x, y += dy, t8);
	char t2[]= "\nQ - quick results (to 950 halfmoves)"; MyText(x, y+=dy, t2);
	char t8a[] = "\nESC - reset position";  MyText(x, y += dy, t8a);
	char t4[]="\nUP - move log up"; MyText(x, y+=dy, t4);
	char t14[] = "\nDOWN - move log down"; MyText(x, y += dy, t14);
	char t7[]="\nH - change side"; MyText(x, y+=dy, t7);
	char t9[]="\n1 - White king moves and control filds";  MyText(x, y+=dy, t9);
	char t10[]="\n2 - Black king moves and control filds"; MyText(x, y+=dy, t10);
	char t11[]="\n3 - Black bishop moves and control filds"; MyText(x, y+=dy, t11);
	char t12[]="\n4 - Black rook moves and control filds";  MyText(x, y+=dy, t12);
	char t13[]="\n5 - Black knight moves and control filds"; MyText(x, y+=dy, t13);
	char t5[]="\n\n MOVES :";  MyText(x, y+=dy, t5);

}