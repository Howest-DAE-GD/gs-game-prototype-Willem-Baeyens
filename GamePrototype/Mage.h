#pragma once
#include <vector>
#include "Creature.h"
#include "Fireball.h"
class Mage final : public Creature
{
public:
	Mage(POINT gridPosition, Grid* enemyGrid);

	virtual void Draw(Rectf rect) const override;

	void Update(float elapsedSec);

	void DoTurn();

	void SetLastBossPosition(POINT position);

	virtual Color4f GetColor() const override;
	virtual POINT GetGridPosition() const override;
private:
	Rectf m_Rect;
	Color4f m_Color;
	POINT m_GridPosition;
	Grid* m_EnemyGridPtr;
	Fireball m_Fireball;
	bool m_Charging;
	POINT m_LastBossPosition;
};