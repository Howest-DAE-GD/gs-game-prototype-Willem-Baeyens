#pragma once
#include "Vector2f.h"
#include "Creature.h"
#include "Grid.h"
class Knight final : public Creature
{
public:
	Knight(POINT gridPos,Grid* gridPtr);
	
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
	void DrawNextMoveArrow() const;

	Color4f m_Color;
	POINT	m_GridPosition;
	POINT	m_NextMove;
	const int m_MaxRowIndex;
	Grid* m_GridPtr;
	int  m_MaxHealth;
	int  m_CurrentHealth;

	Texture* m_InfoTexturePtr;
};
