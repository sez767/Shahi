#include "workGL.h"
#include <stdio.h>
#include <math.h>
#include "./ImTexture.h"
#include "Chess.h"

//////////////мозг програмы////////

int tt_cnt;//кол ходов
int H=-1; // чей текущий ход (-1)-белых, +1-черных

// king all positions
FMove KP[] = {{-1,0}, {-1,-1}, {0,-1}, {1,-1},  {1,0}, {1,1}, {0,1}, {-1,1}};

// король, дае реально возможные ходы из данной позиции
void TKing::CalcRP(const Position5* p)
{
	Rcnt = 0;
	Ccnt = 0;
	if (Enbl == 0)return;
	Position5 p5 = *p;

	for(int i=0;i<8;i++){
		int px = x + KP[i].x;
		int py = y + KP[i].y;
		if( px >=0 && px<8 && py>=0 && py<8){ // только на доске
		
			int f = p5.WhoIs(px, py);
			if( f<5){ // тут стоит фигура
				if(color==Board.Fig[f]->color){  //своя
					CF[Ccnt].x=px;
					CF[Ccnt].y=py;
					Ccnt++;continue;
				}
			}
			RP[Rcnt].x=px; RP[Rcnt].y=py; Rcnt++;
		}
	}

}

// выкидание полей, контролируемых вражескими фигурами (RP+CF) 
void TFigure::KingCorrect(Position5* p)
{
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

bool Position5::OnOtherField(int ix, int iy, int no){
	Position5 pos;
	Board.SetPosition(&pos); // mem
	Board.GetPosition(this[0]);

	for(int i=0;i<5;i++){
		if (Board.Fig[no]->Enbl == 0)continue;
		if (Board.Fig[i]->Enbl == 0)continue;
		if (Board.Fig[i]->color == Board.Fig[no]->color)continue; // свои не считать
		if (i == no)continue;
		if(Board.Fig[i]->Type==KNIGHT || Board.Fig[i]->Type==KING)
			Board.Fig[i]->CalcRP(this);
		if(Board.Fig[i]->Type==BISHOP || Board.Fig[i]->Type==ROOK){
			int mem = this->x[no];
			this->x[no] = 8; // отключаем себя
			Board.Fig[i]->CalcRP(this); // считаем поля контроля без себя
			this->x[no] = mem; // восстановление
		}
		int on = Board.Fig[i]->On_RP_CF(ix, iy);
		Board.GetPosition(pos); // назад
		if (on){return true;}
	}
	return false;

}

// поле не защищено своими
bool Position5::NoProtected(int ix, int iy, int no) 
{
	Position5 pos; Board.SetPosition(&pos); // mem
	Board.GetPosition(this[0]);

	for(int i=0;i<5;i++){
		if (Board.Fig[no]->Enbl == 0)continue;
		if (Board.Fig[i]->Enbl == 0)continue;
		if (Board.Fig[i]->color != Board.Fig[no]->color)continue; // чужие не считать
		if (i == no)continue;
		if(Board.Fig[i]->Type==KNIGHT || Board.Fig[i]->Type==KING)
			Board.Fig[i]->CalcRP(this);
		if(Board.Fig[i]->Type==BISHOP || Board.Fig[i]->Type==ROOK){
			int mem = this->x[no];
			this->x[no] = 8; // отключаем себя
			Board.Fig[i]->CalcRP(this); // считаем поля сонтроля без себя
			this->x[no] = mem; // восстановление
		}
		int on = Board.Fig[i]->On_RP_CF(ix, iy);
		Board.GetPosition(pos); // назад
		if (on){return false;}
	}
	return true;

}


bool TFigure::On_RP_CF(int xx, int yy)
{
	for(int i=0;i<Rcnt;i++){
		if(xx==RP[i].x && yy==RP[i].y)return true;
	}
	for(int i=0;i<Ccnt;i++){
		if(xx==CF[i].x && yy==CF[i].y)return true;
	}
	return false;
}

FMove LP[] = {{-2,-1}, {-1,-2}, {1,-2},  {2,-1},  {2,1}, {1,2}, {-1,2}, {-2,1}};
void TKonj::CalcRP(const Position5* p) // конь, дает реально возможные ходы из данной позиции
{
	Rcnt = 0;
	Ccnt = 0;
	if (Enbl == 0)return;
	Position5 p5 = *p;
	for(int i=0;i<8;i++){
		int px = x + LP[i].x;
		int py = y + LP[i].y;
		if( px >=0 && px<8 && py>=0 && py<8) // только на доске
		{
			int f = p5.WhoIs(px, py);
			if( f<5){ // здесь стоит фигура
				if(color==Board.Fig[f]->color){
					CF[Ccnt].x=px;
					CF[Ccnt].y=py;
					Ccnt++;
					continue;
				}
				
			}
			RP[Rcnt].x=px; RP[Rcnt].y=py; Rcnt++;
		}
	}

}

void TTura::CalcRP(const Position5* p){ // тура, дает реально возможные ходы из данной позиции
	Rcnt = 0;
	Ccnt = 0;
	if (Enbl == 0) return;
	Position5 p5 = *p;

	for(int i=y-1;i>=0;i--){ // движение вверх
		int f = p5.WhoIs(x, i);
		if( f<5){ // здесь стоит фигура
			if(color==Board.Fig[f]->color){
				CF[Ccnt].x=x;
				CF[Ccnt].y=i;
				Ccnt++;}
			break;
		}
		RP[Rcnt].x=x; RP[Rcnt].y=i; Rcnt++;
	}

	for(int i=y+1;i<8;i++){ // движение вниз
		int f = p5.WhoIs(x, i);
		if( f<5){ // здесь стоит фигура
			if(color==Board.Fig[f]->color){CF[Ccnt].x=x; CF[Ccnt].y=i; Ccnt++;}
			break;
		}
		RP[Rcnt].x=x; RP[Rcnt].y=i; Rcnt++;
	}

	for(int i=x+1;i<8;i++){ // движение вправо
		int f = p5.WhoIs(i, y);
		if( f<5){ // здесь стоит фигура
			if(color==Board.Fig[f]->color){CF[Ccnt].x=i; CF[Ccnt].y=y; Ccnt++;}
			break;
		}
		RP[Rcnt].x=i; RP[Rcnt].y=y; Rcnt++;
	}

	for(int i=x-1;i>=0;i--){ // движение вливо
		int f = p5.WhoIs(i, y);
		if( f<5){ // здесь стоит фигура
			if(color==Board.Fig[f]->color){CF[Ccnt].x=i; CF[Ccnt].y=y; Ccnt++;}
			break;
		}
		RP[Rcnt].x=i; RP[Rcnt].y=y; Rcnt++;
	}

}
// Слон, дает реально возможные ходы из данной позиции
void TSlon::CalcRP(const Position5* p)
{
	Rcnt = 0;
	Ccnt = 0;
	Position5 p5 = *p;
	if (Enbl == 0)return;
	for(int i=1;i<8;i++){ // движение to left top
		if((x-i)<0 || (y-i)<0)break;
		int f = p5.WhoIs(x-i, y-i);
		if( f<5){ // здесь стоит фигура
			if(color==Board.Fig[f]->color){CF[Ccnt].x=x-i; CF[Ccnt].y=y-i; Ccnt++;}
			break;
		}
		RP[Rcnt].x=x-i; RP[Rcnt].y=y-i; Rcnt++;
	}

	for(int i=1;i<8;i++){ // движение to right top
		if((x+i)>7 || (y-i)<0)break;
		int f = p5.WhoIs(x+i, y-i);
		if( f<5){ // здесь стоит фигура
			if(color==Board.Fig[f]->color){CF[Ccnt].x=x+i; CF[Ccnt].y=y-i; Ccnt++;}
			break;
		}
		RP[Rcnt].x=x+i; RP[Rcnt].y=y-i; Rcnt++;
	}

	for(int i=1;i<8;i++){ // движение to right down
		if((x+i)>7 || (y+i)>7)break;
		int f = p5.WhoIs(x+i, y+i);
		if( f<5){ // здесь стоит фигура
			if(color==Board.Fig[f]->color){CF[Ccnt].x=x+i; CF[Ccnt].y=y+i; Ccnt++;}
			break;
		}
		RP[Rcnt].x=x+i; RP[Rcnt].y=y+i; Rcnt++;
	}

	for(int i=1;i<8;i++){ // движение to left down
		if((x-i)<0 || (y+i)>7)break;
		int f = p5.WhoIs(x-i, y+i);
		if( f<5){ // здесь стоит фигура
			if(color==Board.Fig[f]->color){CF[Ccnt].x=x-i; CF[Ccnt].y=y+i; Ccnt++;}
			break;
		}
		RP[Rcnt].x=x-i; RP[Rcnt].y=y+i; Rcnt++;
	}

}

// есть ли здесь с (то есть пересечение с фигурами на доске)
bool Position5::IsHere(int ix, int iy)
{
	for(int i=0;i<5;i++){
		if (x[i] > 7)continue;
		if (x[i] == ix && y[i] == iy) return true;
	}
	return false;
}

int Position5::WhoIs (int ix, int iy) // кто здесь, какая фигура
{
	for(int i=0;i<5;i++){
		if (x[i] > 7)continue;
		if (x[i] == ix && y[i] == iy) return i; // ее номер
	}
	return 8; // нет фигур
}

int TBoard::WhoIs (int ix, int iy) // кто здесь, какая фигура  если >5 то нет ничего
{
	for(int i=0;i<5;i++){
		if (Fig[i]->Enbl==0)continue;
		if (Fig[i]->x == ix && Fig[i]->y == iy) return i; // ее номер
	}
	return 8; // нет фигур

}

void TBoard::CalcBMoves() // посчитать все RP для черных
{
	Position5 p5;
	SetPosition(&p5); // берем з доски
	Fig[1]->CalcRP(&p5);
	Fig[2]->CalcRP(&p5);
	Fig[3]->CalcRP(&p5);
	Fig[4]->CalcRP(&p5);
	Fig[1]->KingCorrect(&p5);
}

void TBoard::CalcWMoves() // посчитать все RP для белых
{
	Position5 p5;
	SetPosition(&p5); // берем з доски
	Fig[0]->CalcRP(&p5);
	Fig[0]->KingCorrect(&p5);
}

int Mat()
{
	Position5 p5; 
	Board.SetPosition(&p5); // берем з доски
	Board.Fig[0]->CalcRP(&p5);
	Board.Fig[0]->KingCorrect(&p5);
	// шах белому королю 
	if (p5.OnOtherField(Board.Fig[0]->x, Board.Fig[0]->y, 0)){
		if (Board.Fig[0]->Rcnt == 0)return 1;
	}
	else{
		if (Board.Fig[0]->Rcnt == 0)return 2;// цейтнот
	}
	return 0;

}

int Evaluate()
{
	return 0;
}

// всегда делается относительно черных !
int Evaluate(TBoard* b)
{
	Position5 p5; 
	b->SetPosition(&p5); // берем з доски

	// якшо эта позиция уже была ---
	// и для черных и для белых 
	if (FindPos(&p5)){
		if(H==1)return POS_WAS; 
		else return -POS_WAS;
	}
	//---

	int v = 0;
	// материальный перевес
	if (b->Fig[0]->Enbl == 0)v += 10000; // нет белого короля
	if (b->Fig[1]->Enbl == 0)v -= 10000; // нет черного короля
	if (b->Fig[2]->Enbl)v += 30; // слон
	if (b->Fig[3]->Enbl)v += 50; // тура
	if (b->Fig[4]->Enbl)v += 30; // конь
	

	b->Fig[0]->CalcRP(&p5);
	b->Fig[0]->KingCorrect(&p5);
	// шах белому королю )
	if (p5.OnOtherField(b->Fig[0]->x, b->Fig[0]->y, 0)){
		// предвидение цейтнота белого короля для черных
		if (b->Fig[0]->Rcnt == 0)
			v += 100000; // это будет мат
		else 
			v += 20; 
	}
	else
	{
		// некуда ходить белому королю
		if (b->Fig[0]->Rcnt ==0){
			if (H == -1)v = 1000000; // для хода черных
			else v = -1000000;
		}
	}
	
	// близость белого к черному королю
	int dd = abs(b->Fig[0]->x - b->Fig[1]->x) + abs(b->Fig[0]->y - b->Fig[1]->y);
	v += (14-dd);//4
	
	// зажим белого короля черной турой
	int S = 0;
	int xL = b->Fig[3]->x; int yL = b->Fig[3]->y; // ладья
	int xK = b->Fig[0]->x; int yK = b->Fig[0]->y; // король бел
	if(xK<xL && yK<yL){
		for(int x=1;x<xL;x++)if(b->Fig[1]->x==x && b->Fig[1]->y==yL)break;
		for(int y=1;y<yL;y++)if(b->Fig[1]->x==xL && b->Fig[1]->y==y)break;
		for(int x=1;x<xL;x++)if(b->Fig[2]->x==x && b->Fig[2]->y==yL)break;
		for(int y=1;y<yL;y++)if(b->Fig[2]->x==xL && b->Fig[2]->y==y)break;
		for(int x=1;x<xL;x++)if(b->Fig[4]->x==x && b->Fig[4]->y==yL)break;
		for(int y=1;y<yL;y++)if(b->Fig[4]->x==xL && b->Fig[4]->y==y)break;

		S=xL*yL; // left-top
	}

	if(xK>xL && yK<yL){
		for(int x=xL+1;x<7;x++)if(b->Fig[1]->x==x && b->Fig[1]->y==yL)break;
		for(int y=0;y<yL;y++)if(b->Fig[1]->x==xL && b->Fig[1]->y==y)break;
		for(int x=xL+1;x<7;x++)if(b->Fig[2]->x==x && b->Fig[2]->y==yL)break;
		for(int y=0;y<yL;y++)if(b->Fig[2]->x==xL && b->Fig[2]->y==y)break;
		for(int x=xL+1;x<7;x++)if(b->Fig[4]->x==x && b->Fig[4]->y==yL)break;
		for(int y=0;y<yL;y++)if(b->Fig[4]->x==xL && b->Fig[4]->y==y)break;

		S=(7-xL)*yL; // right-top
	}

	if(xK>xL && yK>yL){
		for(int x=xL+1;x<7;x++)if(b->Fig[1]->x==x && b->Fig[1]->y==yL)break;
		for(int y=yL+1;y<7;y++)if(b->Fig[1]->x==xL && b->Fig[1]->y==y)break;
		for(int x=xL+1;x<7;x++)if(b->Fig[2]->x==x && b->Fig[2]->y==yL)break;
		for(int y=yL+1;y<7;y++)if(b->Fig[2]->x==xL && b->Fig[2]->y==y)break;
		for(int x=xL+1;x<7;x++)if(b->Fig[4]->x==x && b->Fig[4]->y==yL)break;
		for(int y=yL+1;y<7;y++)if(b->Fig[4]->x==xL && b->Fig[4]->y==y)break;

		S=(7-xL)*(7-yL); // right-down
	}
	if(xK<xL && yK>yL){
		for(int x=1;x<xL;x++)if(b->Fig[1]->x==x && b->Fig[1]->y==yL)break;
		for(int y=yL+1;y<7;y++)if(b->Fig[1]->x==xL && b->Fig[1]->y==y)break;
		for(int x=1;x<xL;x++)if(b->Fig[2]->x==x && b->Fig[2]->y==yL)break;
		for(int y=yL+1;y<7;y++)if(b->Fig[2]->x==xL && b->Fig[2]->y==y)break;
		for(int x=1;x<xL;x++)if(b->Fig[4]->x==x && b->Fig[4]->y==yL)break;
		for(int y=yL+1;y<7;y++)if(b->Fig[4]->x==xL && b->Fig[4]->y==y)break;

		S=xL*(7-yL); // left-down
	}
	int qq=(49 - S)/3;//2
	if (S > 0 )v += qq;


	//===== не под ударом ли черные фигуры------
	if(H==1)
	{
		if (p5.OnOtherField(b->Fig[2]->x, b->Fig[2]->y, 2))// слон под ударом 
			if(p5.NoProtected(b->Fig[2]->x, b->Fig[2]->y, 2))v -= 30;// фигура не защищена

		if (p5.OnOtherField(b->Fig[3]->x, b->Fig[3]->y, 3))// тура
			if (p5.NoProtected(b->Fig[3]->x, b->Fig[3]->y, 3))v -= 50;

		if (p5.OnOtherField(b->Fig[4]->x, b->Fig[4]->y, 4))// конь
			if (p5.NoProtected(b->Fig[4]->x, b->Fig[4]->y, 4))v -= 30;
	}

	return v;
}
int dbg;

TMove NextMove(int& f, int &i){

	TMove m;
	m.x = 8; // end cycle
	int maxF;
	if (H<0) maxF = 1;
	else maxF = 5;

	for(;f < maxF; f++)	{
		if (Board.Fig[f]->Enbl == 0)continue;
		if (Board.Fig[f]->Rcnt == 0) {
			if(f==0 && H==-1)m.x = 200; continue;  // ceitnot!!!!
			if(f==1 && H==1)m.x = 200; continue;  // ceitnot!!!!
		}
		m.i = f;
		if (i < Board.Fig[f]->Rcnt)
		{
			m.x = Board.Fig[f]->RP[i].x;
			m.y = Board.Fig[f]->RP[i].y;
			i++; 
			if(i < Board.Fig[f]->Rcnt)break;
			else { f++; i = 0; break; }
		}

	}

	return m;
}

void CalcMoves()
{
	if (H == 1)Board.CalcBMoves();
	else Board.CalcWMoves();
} 

//НЕ ГОТОВО***********************
// три уровня глубины (около 10000 позиций) полный перебор
TMove BestMove()
{
	int H0 = H; // H (цвет хода )
	CalcMoves();
	Position5 p5; // запоминаем исходную позицию
	Board.SetPosition(&p5); // берем з доски

	H = H0;
	int MaxScore1 = MIN_SCOR;
	TMove MaxMove1; MaxMove1.x = 9;
	int f1=0, i1=0; if(H==1)f1=1;

	Position5 MaxP_2; // отвечает позиции после MinMove2!!
	Position5 MaxP_3;

	while(1)
	{

		TMove m1 = NextMove(f1,i1);  // посунули фигуру
		if (m1.x > 7){Board.GetPosition(p5); break;} // конец перебора всех ходов
		Move(&m1); CalcMoves(); // очередной child

		//---------------------------------------------------
		Position5 p5_1;
		Board.SetPosition(&p5_1); // берем з доски
		int MinScore2 = MAX_SCOR;
		TMove MinMove2; MinMove2.x = 9;

		H = -H0;
		int f2=0, i2=0; if(H==1)f2=1;
		while(1)
		{
			TMove m2 = NextMove(f2,i2); 
			if (m2.x > 7){Board.GetPosition(p5_1); break;} // конец перебора всех ходов
			Move(&m2); CalcMoves();

		    //---------------------------------------------------
			Position5 p5_2;
			Board.SetPosition(&p5_2); // берем з доски
			int MaxScore3 = MIN_SCOR;
			TMove MaxMove3; MaxMove3.x = 9;

			H = H0;
			int f3=0, i3=0; if(H==1)f3=1;
			while(1)
			{
				TMove m3 = NextMove(f3,i3); 
				if (m3.x > 7){Board.GetPosition(p5_2); break;} // ? re-- конец перебора всех ходов
				Move(&m3); CalcMoves();
				int Score = Evaluate(&Board)*H; // оценка хода
				if (Score > MaxScore3){MaxScore3 = Score; MaxMove3 = m3; Board.SetPosition(&MaxP_3);} 
				Board.GetPosition(p5_2); CalcMoves(); // восстанавливаем
		
			}
			//---------------------------------------------------


			if (MaxScore3 < MinScore2){MinScore2 = MaxScore3; MinMove2 = MaxMove3;} 
			Board.GetPosition(p5_1); CalcMoves(); // восстанавливаем

		}
		//----------------------------------------------------

		if (MinScore2 > MaxScore1){MaxScore1 = MinScore2; MaxMove1 = MinMove2; Board.SetPosition(&MaxP_2);} 
		Board.GetPosition(p5); CalcMoves(); // восстанавливаем
	}

	return MaxMove1;
}

// МИН-МАКС  ,два уровня глубины (около тысяч позиций) полный перебор
TMove BestMove2(){
	int MATT = 0;
	int H0 = H; // H (цвет хода )
	CalcMoves();
	Position5 p5; // запоминаем исходну позицию
	Board.SetPosition(&p5); // берем з доски

	H = H0;
	int MinMax2_Score = MAX_SCOR;
	TMove MinMax2;
	MinMax2.x = 9; // минимальное из максимумов груп (детей) второго уровня
	int f1=0, i1=0;
	if(H==1) f1=1;
	TMove MaxMove;
	MaxMove.x = 9; //- это просто инициализация
	Position5 P_x; // tmp

 	while(1)
	{
		// первый уровень
		H = H0;
		TMove m1 = NextMove(f1,i1);  // посунули фигуру (ход первого уровня)
		if (m1.x > 7){
			break;
		} // конец перебора всех ходов
		Move(&m1); // очередной child

		//другий уровень
		Position5 p5_1; // очередная подпозиция после хода первого уровня
		Board.SetPosition(&p5_1); // берем з доски
		int MaxScore2 = MIN_SCOR;

		H = -H0; // меняем цвет 
		CalcMoves(); // просчет ходов
		int f2=0, i2=0; if(H==1)f2=1; // выставление индексов перебора на начало
		while(1)
		{
			TMove m2 = NextMove(f2,i2);  // очередной ход второго уровня
			if (m2.x > 7)// конец перебора всех ходов
			{ 
				if (m2.x == 200){
					Move(&m2); 
					CalcMoves();
					Board.SetPosition(&P_x); // берем з доски
					if (P_x.OnOtherField(Board.Fig[0]->x, Board.Fig[0]->y, 0)){
						MATT = 1;
						break;}
					else MaxScore2 = MAX_SCOR; // ceitnot!


				}
				break;
			} 
			Move(&m2); CalcMoves();

			int Score = Evaluate(&Board) * H; // оценка хода второго уровня
			if (Score > MaxScore2){
				MaxScore2 = Score;
				MinMax2 = m1; }

			Board.GetPosition(p5_1);
			CalcMoves(); // восстанавливаем подпозицию (потомка) первого уровня
		}
		//---------------------------------------------------------------------------------------
		if (MATT) { 
			printf("\n= MATT = !!!"); 
			MaxMove = m1; 
			H = H0;
			Board.GetPosition(p5);
			CalcMoves(); // восстанавливаем
			break; 
		}
		// нахождение минимального из максимумов групп второго уровня
		if (MaxScore2 < MinMax2_Score) {
			MinMax2_Score = MaxScore2;
			MaxMove = m1;
		}

		H = H0;
		Board.GetPosition(p5); 
		CalcMoves(); // восстанавливаем исходную позицию

	}

	return MaxMove;
}


// сделать ход (просто перемещение фигуры)
void Move(TMove* pM)
{
	if (pM->x > 7)
		return; // !!!!!!! некуда ходить
	int FigOp = Board.WhoIs(pM->x, pM->y); // фигура противника на этом месте
	if (FigOp < 5)Board.Fig[FigOp]->Enbl = 0; // отключаем ее
	// запись новых координат для данной фигуры
	Board.Fig[pM->i]->x = pM->x;
	Board.Fig[pM->i]->y = pM->y;
	
}

char* figura[] = {"W_Kor","B_Kor", "B_Slon", "B_Tura", "B_Kon"};

int MakeNextMove(){
	tt_cnt++;
	char txt[100];
	TMove m = BestMove2();
	int mov = m.x;
	memo.print("%s to %c:%d \n", figura[m.i], (m.x)+65, (m.y)+1);
	//sprintf(txt, "%c:%d", (m.x) + 65, (m.y) + 1);
	//MyTextBIG(850, 600, txt);
	Move(&m);
	int mt = Mat();
	if (mt == 1)
		if (H == 1) {
		mov = mt; 
			
		sprintf(txt,"MAT!!! in %d",tt_cnt); //halfways
		MyTextBIG(950, 600, txt);
		PlaySound("mario.wav", NULL, SND_ASYNC | SND_FILENAME);
		printf("\nMAT"); // сделан ход черными
	}
	if (mt==2)
		if(H == 1){
		mov = mt;
		sprintf(txt, "Ceinot!   in  %d", tt_cnt);
		MyTextBIG(850, 600, txt);
		PlaySound("mario.wav", NULL, SND_ASYNC | SND_FILENAME);
		printf("\nCeitnot!!!");} 

	SavePos();
	H *= -1;
	return mt;
}

int mm_cnt;

void Make500Move(){
	DrawFonRect();
	SavePos();
	Pmcnt = 0; // сброс индекса записи позиций
	tt_cnt = 0;
	int m;
	for(m=0;m<950;m++){

		int mt = MakeNextMove();
		if (mt > 7)
		{
			printf("\nCeITNOT!!!");
			break;
		}

		if (mt == 1)//мат
			break;

		if (WKing.Enbl == 0){
			printf("MAT was!!!");
			break;
		}
	}

	if(tt_cnt==950){
		MyTextBIG(850, 600, "No end in 950 ...");
	}
	printf("\ntt_cnt=%d", tt_cnt);
}

// тест 100 партий до 950 ходов в КОНСОЛЬ
void TestShess100_950()
{
	DrawFonRect();
	mm_cnt = 0;
	for(int i=0;i<100;i++){
		printf("\n ---%d----", i + 1);
		RandPosition();
		Make500Move();
	}
	printf("\n\nmm_cnt=%d", mm_cnt);
}
