#include "TFigure.h"

class TKonj : public TFigure{
public:
	void CalcRP(const Position5* p); // дает реально возможные ходы из данной позиции
	TKonj() :TFigure(&KonjBl_txr) { color = 1; Type = KNIGHT; }
	~TKonj(){Type = KNIGHT;}
private:
};
