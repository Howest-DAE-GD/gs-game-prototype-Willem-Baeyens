#pragma once
#include "Vector2f.h"
#include "Creature.h"
#include "Grid.h"
class Knight final : public Creature
{
public:
	Knight(POINT gridPos,Grid* gridPtr,Grid* enemyGridPtr);
	
	void Update(float elapsedSec);

	virtual Color4f GetColor() const override;
	virtual POINT GetGridPosition() const override;

	virtual void Draw(Rectf rect) const override;

	void DoTurn();

	void Stun();
private:
	Color4f m_Color;
	POINT	m_Grid_Position;
	int		m_NextMove;
	const int m_MaxRowIndex;
	bool m_Attacking;
	float m_AttackDuration;
	float m_AttackTimer;
	Grid* m_EnemyGridPtr;
	Grid* m_GridPtr;
	bool m_Stunned;
};
