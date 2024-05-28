#pragma once
#include "Grid.h"
#include "Boss.h"
class Rogue final : public Creature
{
public:
	Rogue(POINT gridPos, Grid* gridPtr, Boss* bossPtr);

	void Update(float elapsedSec);

	virtual Color4f GetColor() const override;
	virtual POINT GetGridPosition() const override;
	virtual Texture* GetTexturePtr() const override;


	virtual void Draw(Rectf rect) const override;

	void Move();
	void Attack();

	virtual void TakeDamage(int damage);

	bool IsAlive() const;
	virtual bool TurnDone() const override;

private:
	bool AllyInBossRange();
	void Step();

	void ChangeColorIfAllyInRange();

	void Rotate(bool clockwise = false);

	POINT	m_GridPosition;
	Grid*	m_GridPtr;
	Boss*	m_Boss;
	int		m_MaxHealth;
	int		m_CurrentHealth;
	Color4f m_Color;
	POINT   m_NextStep;

	bool    m_ExtraStep;
	float   m_ExtraStepTimer;

	bool    m_Rotating;

	Texture* m_InfoTexturePtr;
};

POINT Difference(POINT lhs, POINT rhs);
int	  OrthogonalDistance(POINT lhs, POINT rhs);