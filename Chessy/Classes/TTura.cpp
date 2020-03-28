#include "../TBoard.h"

// ����, ���� ������� ��������� ���� �� ������ �������
void TTura::CalcRP(const Position5* p){ 
	Rcnt = 0;
	Ccnt = 0;
	if (Enbl == 0) return;
	Position5 p5 = *p;

	for(int i=y-1;i>=0;i--){ // �������� �����
		int f = p5.WhoIs(x, i);
		if( f<5){ // ����� ����� ������
			if(color==Board.Fig[f]->color){
				CF[Ccnt].x=x;
				CF[Ccnt].y=i;
				Ccnt++;}
			break;
		}
		RP[Rcnt].x=x; RP[Rcnt].y=i;
		Rcnt++;
	}

	for(int i=y+1;i<8;i++){ // �������� ����
		int f = p5.WhoIs(x, i);
		if( f<5){ // ����� ����� ������
			if(color==Board.Fig[f]->color){
				CF[Ccnt].x=x;
				CF[Ccnt].y=i;
				Ccnt++;}
			break;
		}
		RP[Rcnt].x=x;
		RP[Rcnt].y=i;
		Rcnt++;
	}

	for(int i=x+1;i<8;i++){ // �������� ������
		int f = p5.WhoIs(i, y);
		if( f<5){ // ����� ����� ������
			if(color==Board.Fig[f]->color){
				CF[Ccnt].x=i;
				CF[Ccnt].y=y;
				Ccnt++;
			}
			break;
		}
		RP[Rcnt].x=i; RP[Rcnt].y=y; Rcnt++;
	}

	for(int i=x-1;i>=0;i--){ // �������� �����
		int f = p5.WhoIs(i, y);
		if( f<5){ // ����� ����� ������
			if(color==Board.Fig[f]->color){
				CF[Ccnt].x=i;
				CF[Ccnt].y=y;
				Ccnt++;
			}
			break;
		}
		RP[Rcnt].x=i;
		RP[Rcnt].y=y;
		Rcnt++;
	}
}