#include "TFigure.h"


class TKing : public TFigure{
public:
	void CalcRP(const Position5* p); // ���� ������� ��������� ���� �� ������ �������
	TKing() :TFigure(&KingWt_txr) { Type = KING; }
	TKing(ImgTexture* it, int cl):TFigure(it,cl){Type = KING;}
	~TKing(){}
private:
};
