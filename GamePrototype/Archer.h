#pragma once
#include "Grid.h"
#include "Boss.h"
class Archer final : public Creature
{
public:
	Archer(POINT gridPos, Grid* gridPtr, Boss* bossPtr);

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
	bool Step();

	POINT	m_GridPosition;
	Grid*	m_GridPtr;
	Boss*	m_Boss;
	int		m_MaxHealth;
	int		m_CurrentHealth;
	Color4f m_Color;

	bool	m_Stepping;
	float   m_StepTimer;
	Texture* m_InfoTexturePtr;
};

