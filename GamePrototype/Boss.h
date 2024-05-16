#pragma once
#include "Vector2f.h"
#include "Creature.h"
#include "Grid.h"
class Knight;
enum class BossMove
{
	null,columnAttack,rowAttack,cometAttack
};

class Boss final : public Creature
{
public:
	Boss(POINT gridPos, Knight* knightPtr, Grid* gridPtr, Grid* enemyGridPtr);

	virtual void Draw(Rectf rect) const override;
	void DrawAttack() const;

	void Move(const Vector2f& change);

	bool Update(const float elapsedSec);

	virtual Color4f GetColor() const override;
	virtual POINT GetGridPosition() const override;
	virtual void TakeDamage(int damage) override;
	void Move(POINT change);

	void RowAttack();
	void ColumAttack();
	void CometAttack();

	void HitEnemies();


	bool CheckMove(POINT change) const;
	//void Stun();

	//int GetStunCounter() const;

	int GetHealth() const;

private:

	void Attack();
	void IncrementAttackIndex();

	Rectf m_Rect;
	Rectf m_HealthBarRect;
	float m_Speed;
	Color4f m_Color;
	POINT	m_GridPosition;
	int     m_Health;
	//int		m_StunCounter;
	Knight* m_KnightPtr;
	Grid* m_GridPtr;
	Grid* m_EnemyGridPtr;

	float m_AttackDuration;
	float m_AttackTimer;
	bool  m_Attacking;
	bool  m_DidDamage;

	std::vector<POINT> m_AttackLocations;
	std::vector<BossMove> m_AttackPatternVect;

	int                   m_AttackIndex;
};

