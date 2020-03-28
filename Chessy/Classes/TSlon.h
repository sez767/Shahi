#include "TFigure.h"

class TSlon : public TFigure{
public:
	void CalcRP(const Position5* p); // дает реально возможные ходы из данной позиции
	TSlon() :TFigure(&SlonBl_txr) { color = 1; Type=BISHOP; }
	~TSlon(){Type=BISHOP;}
private:
};
