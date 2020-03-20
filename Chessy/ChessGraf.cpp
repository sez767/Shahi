#include "workGL.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "./ImTexture.h"
#include "Chess.h"

ImgTexture SlonBl_txr;
ImgTexture TuraBl_txr;
ImgTexture KonjBl_txr;
ImgTexture KingBl_txr;
ImgTexture KingWt_txr;

int ind;

void init_img(){
	KingBl_txr.LoadFromFile(L"img/King_bl.png");
	SlonBl_txr.LoadFromFile(L"img/Slon_bl.png");
	TuraBl_txr.LoadFromFile(L"img/Tura_bl.png");
	KonjBl_txr.LoadFromFile(L"img/Konj_bl.png");
	KingWt_txr.LoadFromFile(L"img/King_wt.png");

}//---------

void TBoard::DrawBoard()
{
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

bool TBoard::InDisRect(int mx, int my, int i)// бокова область з фигурами
{
	my -= 70;
	float num = Fig[i]->num;
	if (mx >= (x0 + 8.5 * Size) && mx < (x0 + 9.5 * Size))
		if (my >= (y0 + num * Size ) && my < (y0 + (num+1) * Size ))
			return true;
	return false;
}

bool TBoard::InBoardRect(int mx, int my) // мыша на доске
{
	if (mx > x0  &&  mx < (x0 + 8 * Size))
		if (my > y0  && my < (y0 + 8 * Size))
			return true;
	return false;

}
void TBoard::DrawDisRect(float x, float y){// бокова область з фигурами
	Bar(x+x0 + 8.5 * Size, y+y0 ,  x+x0 + 9.5 * Size, y+y0 + 5*Size, SwKorr);
}

// номер фигуры под мышей (в Fig[5])
int TBoard::FindFigOnMouse(int mx, int my){
	for(int i=0; i<5;i++){
		if (InDisRect(mx, my, i) && (Fig[i]->Enbl==0))return i;
	}
	return 7;
}

void TBoard::SetFigCoord(int mx, int my, int i){
	Fig[i]->x = (mx-x0)/Size; Fig[i]->y = (my-y0)/Size; Fig[i]->Enbl = 1;
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

Color GrBluBb(100,255,0,0);
Color GrGreen(100,0,255,0);

void TBoard::Square(int sx, int sy){
	Bar(x0+sx*Size, y0+sy*Size, x0+(sx+1)*Size, y0+(sy+1)*Size,GrBluBb);
}

void TBoard::Square1(int sx, int sy){
	Bar(x0+sx*Size, y0+sy*Size, x0+(sx+1)*Size, y0+(sy+1)*Size,GrGreen);
}


void TFigure::DrawRP(){
	if (Enbl == 0)return;
	for (int i = 0; i < Rcnt; i++)
		Board.Square(RP[i].x, RP[i].y);
}

void TFigure::DrawCF(){
	if (Enbl == 0)return;
	for (int i = 0; i < Ccnt; i++)
		Board.Square1(CF[i].x, CF[i].y);

}

TKing WKing; 
TKing BKing(&KingBl_txr,1);
TSlon BSlon;
TTura BTura;
TKonj BKonj;

TFigure* FG[] = {&WKing, &BKing, &BSlon, &BTura, &BKonj};
TBoard Board(FG, 5, 90, 10, 10);

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
	//srand(time(0));  //висне!!!
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

int PositionBad(){
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

bool Position5::IsEqual(Position5* ps)
{
	for (int i = 0; i < 5; i++){
		if (ps->x[i] > 7)continue;
		if (x[i] != ps->x[i])return false;
		if (y[i] != ps->y[i])return false;
	}
	return true;

}

Position5 Pmem[1000];
int Pmcnt;

void SavePos()
{
	Board.SetPosition(&Pmem[Pmcnt++]);
}

bool FindPos(Position5* ps)
{
	for (int i = 0; i < Pmcnt; i++){
		//if (ps->x[i] > 7)continue;
		if (ps->IsEqual(&Pmem[i]))return true;
	}
	return false;
}

void BackMove()
{
	if (Pmcnt < 2)return;
	Pmcnt -= 2;
	Board.GetPosition(Pmem[Pmcnt++]);
}

void FirstMove()
{
	Board.GetPosition(Pmem[0]);
}

void PrintHelp()//в консоль, вже не нада
{
	printf("\n\nF1 - help\nF9 - test 950 moves\nF4 - end-1 from memory");
	printf("\nF3 - first move from memory\n7 - test random 100 games\nBlank - make next move");
	printf("\nh - change color move (black-white)\nr - random position\nEsc - reset memory position");
	printf("\n1 White King moves and Control filds");
	printf("\n2 Black King moves and Control filds");
	printf("\n3 Black BISHOP moves and Control filds");
	printf("\n4 Black ROOK moves and Control filds");
	printf("\n5 Black KNIGHT moves and Control filds");
	printf("\ns - random position without ROOK");
}

void PrintHelpInWindow(int x, int y)
{
	int dy = 29; // расстояние между строками
	char t1[]="\n\nDRUG&DROP chessmans on board";  MyText(x, y+=dy, t1);
	//char t6[]="\ntest test test"; MyText(x, y+=dy, t6);
	char t8[] = "\nR - random position";  MyText(x, y += dy, t8);
	char t17[]="\nSpace - make move"; MyText(x, y+=dy, t17);
	char t3[]="\nF4 - move back";  MyText(x, y+=dy, t3);
	char t2[]= "\nF9 - test 950 moves"; MyText(x, y+=dy, t2);
	char t4[]="\nup - move log up"; MyText(x, y+=dy, t4);
	char t14[] = "\ndown - move log down"; MyText(x, y += dy, t14);
	char t5[]="\n7 - test random 100 games in console";  MyText(x, y+=dy, t5);
	
	char t7[]="\nh - change color move (black-white)"; MyText(x, y+=dy, t7);
	
	char t8a[]="\nEsc - reset memory position";  MyText(x, y+=dy, t8a);
	char t9[]="\n1  White King moves and Control filds";  MyText(x, y+=dy, t9);
	char t10[]="\n2  Black King moves and Control filds"; MyText(x, y+=dy, t10);
	char t11[]="\n3  Black BISHOP moves and Control filds"; MyText(x, y+=dy, t11);
	char t12[]="\n4  Black ROOK moves and Control filds";  MyText(x, y+=dy, t12);
	char t13[]="\n5  Black KNIGHT moves and Control filds"; MyText(x, y+=dy, t13);
	

}