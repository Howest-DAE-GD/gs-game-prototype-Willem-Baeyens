#pragma once
#include <vector>
#include "Creature.h"
#include "Fireball.h"
class Mage final : public Creature
{
public:
	Mage(POINT gridPosition, Grid* gridPtr);

	virtual void Draw(Rectf rect) const override;

	void Update(float elapsedSec);

	void DoTurn();
	virtual void TakeDamage(int damage);

	bool IsAlive() const;

	void SetLastBossPosition(POINT position);

	virtual Color4f GetColor() const override;
	virtual POINT GetGridPosition() const override;
private:
	Rectf m_Rect;
	Color4f m_Color;
	POINT m_GridPosition;
	Grid* m_GridPtr;
	//Fireball m_Fireball;
	bool m_Charging;
	POINT m_LastBossPosition;
	int m_MaxHealth;
	int m_CurrentHealth;
};