#pragma once
#include "Vector2f.h"
#include "Creature.h"
#include "Grid.h"
class Knight;
enum class BossMove
{
	null,columnAttack,beamAttack,cometAttack,surroundingAttack
};

class Boss final : public Creature
{
public:
	Boss(POINT gridPos, Knight* knightPtr, Grid* gridPtr);

	virtual void Draw(Rectf rect) const override;
	void DrawAttack() const;

	void Move(const Vector2f& change);

	bool Update(const float elapsedSec);

	virtual Color4f GetColor() const override;
	virtual POINT GetGridPosition() const override;
	virtual Texture* GetTexturePtr() const override;

	virtual void TakeDamage(int damage) override;
	void Move(POINT change);

	void RowAttack();
	void ColumAttack();
	void CometAttack();
	void SurroundingAttack();

	void HitEnemies();

	bool CheckMove(POINT change) const ;

	int GetHealth() const;
	
	virtual bool IsBoss() const override;
	void Attack();

	virtual bool TurnDone() const override;
	
	std::vector<BossMove> GetPattern() const;
private:
	

	void IncrementAttackIndex();

	Rectf m_Rect;
	Rectf m_HealthBarRect;
	float m_Speed;
	Color4f m_Color;
	POINT	m_GridPosition;
	int     m_MaxHealth;
	int		m_CurrentHealth;
	Knight* m_KnightPtr;
	Grid* m_GridPtr;

	float m_AttackDuration;
	float m_AttackTimer;
	bool  m_Attacking;
	bool  m_DidDamage;

	std::vector<POINT> m_AttackLocations;
	std::vector<BossMove> m_AttackPatternVect;

	int                   m_AttackIndex;

	Texture* m_InfoTexturePtr;
};


