#include "../TBoard.h"

// Слон, дает реально возможные ходы из данной позиции
void TSlon::CalcRP(const Position5* p){
	Rcnt = 0;
	Ccnt = 0;
	Position5 p5 = *p;
	if (Enbl == 0)return;
	for(int i=1;i<8;i++){ // движение to left top
		if((x-i)<0 || (y-i)<0)break;
		int f = p5.WhoIs(x-i, y-i);
		if( f<5){ // здесь стоит фигура
			if(color==Board.Fig[f]->color){
				CF[Ccnt].x=x-i;
				CF[Ccnt].y=y-i;
				Ccnt++;
			}
			break;
		}
		RP[Rcnt].x=x-i;
		RP[Rcnt].y=y-i;
		Rcnt++;
	}

	for(int i=1;i<8;i++){ // движение to right top
		if((x+i)>7 || (y-i)<0)break;
		int f = p5.WhoIs(x+i, y-i);
		if( f<5){ // здесь стоит фигура
			if(color==Board.Fig[f]->color){
				CF[Ccnt].x=x+i;
				CF[Ccnt].y=y-i;
				Ccnt++;
			}
			break;
		}
		RP[Rcnt].x=x+i; 
		RP[Rcnt].y=y-i;
		Rcnt++;
	}

	for(int i=1;i<8;i++){ // движение to right down
		if((x+i)>7 || (y+i)>7)break;
		int f = p5.WhoIs(x+i, y+i);
		if( f<5){ // здесь стоит фигура
			if(color==Board.Fig[f]->color){
				CF[Ccnt].x=x+i;
				CF[Ccnt].y=y+i;
				Ccnt++;
			}
			break;
		}
		RP[Rcnt].x=x+i;
		RP[Rcnt].y=y+i;
		Rcnt++;
	}

	for(int i=1;i<8;i++){ // движение to left down
		if((x-i)<0 || (y+i)>7)break;
		int f = p5.WhoIs(x-i, y+i);
		if( f<5){ // здесь стоит фигура
			if(color==Board.Fig[f]->color){
				CF[Ccnt].x=x-i;
				CF[Ccnt].y=y+i;
				Ccnt++;}
			break;
		}
		RP[Rcnt].x=x-i;
		RP[Rcnt].y=y+i;
		Rcnt++;
	}

}
