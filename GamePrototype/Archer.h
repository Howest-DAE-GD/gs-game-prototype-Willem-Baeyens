#pragma once
#include "Grid.h"
#include "Boss.h"
#include "Arrow.h"
class Archer final : public Creature
{
public:
	Archer(POINT gridPos, Grid* gridPtr, Boss* bossPtr,int health);

	virtual void Update(float elapsedSec) override;

	virtual Color4f GetColor() const override;
	virtual POINT GetGridPosition() const override;
	virtual Texture* GetTexturePtr() const override;


	virtual void Draw(Rectf rect) const override;

	virtual void Attack() override;

	virtual void TakeDamage(int damage);

	bool IsAlive() const;
	virtual bool TurnDone() const override;

	virtual int GetAttackPriority() const override;
	virtual int GetMovePriority() const override;
	virtual bool IsDamagingHero() const override;

	virtual int GetHealth() const override;

	virtual void PrintInfo() const override;
private:
	void ArcherMove();
	bool Step();

	void DrawMoveArrow() const;

	void RemoveArrows();

	POINT	m_GridPosition;
	Grid*	m_GridPtr;
	Boss*	m_Boss;
	int		m_MaxHealth;
	int		m_CurrentHealth;
	Color4f m_Color;

	bool	m_Stepping;
	float   m_StepTimer;
	Texture* m_InfoTexturePtr;

	std::vector<Arrow> m_ArrowVect;
};

