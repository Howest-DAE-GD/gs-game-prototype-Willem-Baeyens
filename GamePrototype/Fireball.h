#pragma once
#include "Grid.h"
class Fireball final
{
public:
	Fireball(POINT gridPos,Grid* grid);
	Fireball() = default;

	void Draw() const;
	bool Update(float elapsedSec);

	void Explode();
private:
	POINT m_GridPosition;
	Grid*  m_GridPtr;
	Color4f m_Color;
	float m_ExplosionDuration;
	float m_ExplostionTimer;
	bool m_Exploding;
};

