#include "workGL.h"
#include <stdio.h>
#include <math.h>
#include "./ImTexture.h"
#include "TBoard.h"
#include "Chess.h"

int tt_cnt;//кол ходов
int H=-1; // чей текущий ход (-1)-б, +1-ч (старт с бел != часто при рандоме мат в 1 ход)

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
		if (on){
			return true;
		}
	}
	return false;

}

// поле не защищено своими
bool Position5::NoProtected(int ix, int iy, int no){
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

bool Position5::IsHere(int ix, int iy){// есть ли здесь стоит др фигура
	for(int i=0;i<5;i++){
		if (x[i] > 7)continue;
		if (x[i] == ix && y[i] == iy) return true;
	}
	return false;
}

int Position5::WhoIs (int ix, int iy){ // кто здесь, какая фигура(доска выбора)
	for(int i=0;i<5;i++){
		if (x[i] > 7)continue;
		if (x[i] == ix && y[i] == iy) return i; // ее номер
	}
	return 8; // нет фигур
}



int Mat(){
	Position5 p5; 
	Board.SetPosition(&p5); // берем з доски
	Board.Fig[0]->CalcRP(&p5);
	Board.Fig[0]->KingCorrect(&p5);
	
	if (p5.OnOtherField(Board.Fig[0]->x, Board.Fig[0]->y, 0)){
		if (Board.Fig[0]->Rcnt == 0)return 1;// шах 
	}
	else{
		if (Board.Fig[0]->Rcnt == 0)return 2;// цейтнот
	}
	return 0;

}


TMove NextMove(int& f, int &i){
	TMove m;
	m.x = 8; //конец цыкла
	int maxF;
	if (H<0) maxF = 1;
	else maxF = 5;

	for(;f < maxF; f++)	{
		if (Board.Fig[f]->Enbl == 0)continue;
		if (Board.Fig[f]->Rcnt == 0) {
			if(f==0 && H==-1)m.x = 200; continue;  // ceitnot
			if(f==1 && H==1)m.x = 200; continue;  // ceitnot
		}
		m.i = f;
		if (i < Board.Fig[f]->Rcnt){
			m.x = Board.Fig[f]->RP[i].x;
			m.y = Board.Fig[f]->RP[i].y;
			i++; 
			if(i < Board.Fig[f]->Rcnt)
				break;
			else {
				f++;
				i = 0;
				break;
			}
		}

	}

	return m;
}

void CalcMoves(){
	if (H == 1)Board.CalcBMoves();
	else Board.CalcWMoves();
} 

/// МИН-МАКС, два уровня глубины (около 1000 позиций) полный перебор

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

 	while(1){
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
		int f2=0, i2=0;
		if(H==1)f2=1; // выставление индексов перебора на начало
		while(1){
			TMove m2 = NextMove(f2,i2);  // очередной ход второго уровня
			if (m2.x > 7){ // конец перебора всех ходов
			 	if (m2.x == 200){
					Move(&m2); 
					CalcMoves();
					Board.SetPosition(&P_x); // берем з доски
					if (P_x.OnOtherField(Board.Fig[0]->x, Board.Fig[0]->y, 0)){
						MATT = 1;
						break;
					}
					else MaxScore2 = MAX_SCOR; // ceitnot!


				}
				break;
			} 
			Move(&m2);
			CalcMoves();

			int Score = Evaluate(&Board) * H; // оценка хода второго уровня
			if (Score > MaxScore2){
				MaxScore2 = Score;
				MinMax2 = m1;
			}

			Board.GetPosition(p5_1);
			CalcMoves(); // восстанавливаем подпозицию (потомка) первого уровня
		}
		//
		if (MATT) { 
			printf("\n= MATT ="); 
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
void Move(TMove* pM){
	if (pM->x > 7)
		return; // ! некуда ходить
	int FigOp = Board.WhoIs(pM->x, pM->y); // фигура противника на этом месте
	if (FigOp < 5)Board.Fig[FigOp]->Enbl = 0; // отключаем ее
	// запись новых координат для данной фигуры
	Board.Fig[pM->i]->x = pM->x;
	Board.Fig[pM->i]->y = pM->y;
	
}
char figura[5][10] = {"W_Kor","B_Kor", "B_Slon", "B_Tura", "B_Kon"};
int count = 1;
int MakeNextMove(){
	tt_cnt++;
	char txt[100];
	TMove m = BestMove2();
	int mov = m.x;
	memo.print("%d. %s moves to %c:%d \n", count,figura[m.i], (m.x)+65, (m.y)+1);
	Move(&m);
	int mt = Mat();
	if (mt == 1){
		if (H == 1) {
			mov = mt; 
			PlaySound("src/mario.wav", NULL, SND_ASYNC | SND_FILENAME);
			memo.print("   <<< MAT! >>> in %d halfmoves", count);
			//printf("\nMAT");
			matFlg = 1;
		}
	}
	if (mt == 2) {
		if (H == 1) {
			mov = mt;
			matFlg = 1;
			//sprintf(txt, "Ceinot!   in  %d", tt_cnt);
			//MyTextBIG(850, 600, txt);
			memo.print("   <<< Ceinot! >>> in %d halfmoves", count);
			PlaySound("src/mario.wav", NULL, SND_ASYNC | SND_FILENAME);
			printf("\nCeitnot!!!");
		}
	}

	SavePos();
	H *= -1;
	count++;
	return mt;
}

int mm_cnt;
void Make500Move(){
	//DrawFonRect();
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
		//MyTextBIG(850, 600, "No end in 950 ...");
		memo.print("No end in 950 ...");
	}
	printf("\ntt_cnt=%d", tt_cnt);
}

// отладка -тест 100 партий до 950 ходов в КОНСОЛЬ
void TestShess100_950()
{
	DrawFonRect();
	mm_cnt = 0;
	for(int i=0;i<100;i++){
		printf("\n ---%d----", i + 1);
		RandPosition();
		Make500Move();
	}
	printf("\n\ncount hMoves= %d", mm_cnt);
}
/* консоль для ОТЛАДКИ
void ShowConsoleWindow() {
	AllocConsole();
	freopen("CONOUT$", "wb", stdout);
	freopen("CONIN$", "rb", stdin);
	TCHAR fullName[MAX_PATH];
	GetModuleFileName(NULL, fullName, MAX_PATH);
	HWND hWnd = FindWindow(NULL, fullName);
	SetConsoleTitle(_T("Attached to workGL.exe"));
	RECT rect;
	GetWindowRect(hWnd, &rect);
	//Shell_TrayWnd
	RECT desktopArea;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &desktopArea, 0);

	SetWindowPos(hWnd, HWND_TOP, 0, desktopArea.bottom - rect.bottom + rect.top, 0, 0, SWP_NOSIZE);
}
*/

