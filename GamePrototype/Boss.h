#pragma once
#include "Vector2f.h"
#include "Creature.h"
class Knight;
class Boss final : public Creature
{
public:
	Boss(POINT gridPos, Knight* knightPtr);

	virtual void Draw(Rectf rect) const override;

	void Move(const Vector2f& change);

	void Update(const float elapsedSec);

	virtual Color4f GetColor() const override;
	virtual POINT GetGridPosition() const override;
	virtual void TakeDamage(int damage) override;
	void Move(POINT change);

	void Stun();

	int GetStunCounter() const;

private:
	Rectf m_Rect;
	Rectf m_HealthBarRect;
	float m_Speed;
	Color4f m_Color;
	POINT	m_Grid_Position;
	int     m_Health;
	int		m_StunCounter;
	Knight* m_KnightPtr;
};

