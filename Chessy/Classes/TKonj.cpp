#include "../TBoard.h"

// конь, дает реально возможные ходы из данной позиции
FMove LP[] = {{-2,-1}, {-1,-2}, {1,-2},  {2,-1},  {2,1}, {1,2}, {-1,2}, {-2,1}};
void TKonj::CalcRP(const Position5* p){ 
	Rcnt = 0;
	Ccnt = 0;
	if (Enbl == 0)return;
	Position5 p5 = *p;
	for(int i=0;i<8;i++){
		int px = x + LP[i].x;
		int py = y + LP[i].y;
		if( px >=0 && px<8 && py>=0 && py<8){ // только на доске
			int f = p5.WhoIs(px, py);
			if( f<5){ // здесь стоит фигура
				if(color==Board.Fig[f]->color){
					CF[Ccnt].x=px;
					CF[Ccnt].y=py;
					Ccnt++;
					continue;
				}
			}
			RP[Rcnt].x=px;
			RP[Rcnt].y=py;
			Rcnt++;
		}
	}

}


