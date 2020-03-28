#include "workGL.h"
#include "./ImTexture.h"
#include "Classes/TFigure.h"
#include "TBoard.h"

//���������� �����(���) ������, ���� >5 �� ��� ������
int TBoard::WhoIs (int ix, int iy){   
	for(int i=0;i<5;i++){
		if (Fig[i]->Enbl==0)continue;
		if (Fig[i]->x == ix && Fig[i]->y == iy) return i; // �� �����
	}
	return 8; // ��� �����

}

void TBoard::CalcBMoves(){ // ��������� ��� ���� RP ��� ������
	Position5 p5;
	SetPosition(&p5); // ����� � �����
	Fig[1]->CalcRP(&p5);
	Fig[2]->CalcRP(&p5);
	Fig[3]->CalcRP(&p5);
	Fig[4]->CalcRP(&p5);
	Fig[1]->KingCorrect(&p5);
}

void TBoard::CalcWMoves(){ // ��������� ��� RP ��� �����
	Position5 p5;
	SetPosition(&p5); // ����� � �����
	Fig[0]->CalcRP(&p5);
	Fig[0]->KingCorrect(&p5);
}

// ������....������ �������� ������������ ������
int Evaluate(TBoard* b) {
	Position5 p5;
	b->SetPosition(&p5); // ����� � �����
	// ���� ��� ������� ��� ����
	// � ��� ������ � ��� ����� 
	if (FindPos(&p5)) {
		if (H == 1)return POS_WAS;
		else return -POS_WAS;
	}
	int v = 0;
	// ������������ �������
	if (b->Fig[0]->Enbl == 0)v += 10000; // ��� ������ ������
	if (b->Fig[1]->Enbl == 0)v -= 10000; // ��� ������� ������
	if (b->Fig[2]->Enbl)v += 30; // ����
	if (b->Fig[3]->Enbl)v += 50; // ����
	if (b->Fig[4]->Enbl)v += 30; // ����

	b->Fig[0]->CalcRP(&p5);
	b->Fig[0]->KingCorrect(&p5);
	// ��� ������ ������
	if (p5.OnOtherField(b->Fig[0]->x, b->Fig[0]->y, 0)) {
		// ����������� �������� ������ ������ ��� ������
		if (b->Fig[0]->Rcnt == 0)
			v += 100000; // ��� ����� ���
		else
			v += 20;
	}
	else
	{
		// ������ ������ ������ ������
		if (b->Fig[0]->Rcnt == 0) {
			if (H == -1)v = 1000000; // ��� ���� ������
			else v = -1000000;
		}
	}
	// �������� ������ � ������� ������
	int dd = abs(b->Fig[0]->x - b->Fig[1]->x) + abs(b->Fig[0]->y - b->Fig[1]->y);
	v += (14 - dd);//4

	// ����� ������ ������ ������ ������
	if (b->Fig[3]->Enbl) {
		int S = 0;
		int xL = b->Fig[3]->x; int yL = b->Fig[3]->y; // �����
		int xK = b->Fig[0]->x; int yK = b->Fig[0]->y; // ������ �
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

	//�� ��� ������ �� ������ ������
	if (H == 1) {
		if (p5.OnOtherField(b->Fig[2]->x, b->Fig[2]->y, 2))// ���� ��� ������ 
			if (p5.NoProtected(b->Fig[2]->x, b->Fig[2]->y, 2))v -= 30;// ������ �� ��������

		if (p5.OnOtherField(b->Fig[3]->x, b->Fig[3]->y, 3))// ����
			if (p5.NoProtected(b->Fig[3]->x, b->Fig[3]->y, 3))v -= 50;

		if (p5.OnOtherField(b->Fig[4]->x, b->Fig[4]->y, 4))// ����
			if (p5.NoProtected(b->Fig[4]->x, b->Fig[4]->y, 4))v -= 30;
	}

	return v;
}