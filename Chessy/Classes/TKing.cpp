#include "../TBoard.h"

// king all positions
FMove KP[] = {{-1,0}, {-1,-1}, {0,-1}, {1,-1},  {1,0}, {1,1}, {0,1}, {-1,1}};
void TKing::CalcRP(const Position5* p){// король, дает реально возможные ходы из данной позиции
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


