#include "TFigure.h"

class TTura : public TFigure{
public:
	void CalcRP(const Position5* p); // ���� ������� ��������� ���� �� ������ �������
	TTura() :TFigure(&TuraBl_txr) { color = 1; Type = ROOK; }
	~TTura(){Type = ROOK;}
private:
};
