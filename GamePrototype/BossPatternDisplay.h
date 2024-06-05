#pragma once
#include "Boss.h"
enum class BossMove;
class BossPatternDisplay
{
public:
	BossPatternDisplay(Point2f pos, const std::vector<BossMove>& pattern);

	void Draw() const;

	void Turn();
private:

	Point2f m_Position;
	std::vector<BossMove>		m_Pattern;
	std::vector<Texture*>       m_Letters;
	int		m_NextMoveIndex;
};

