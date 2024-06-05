#pragma once
#include <vector>
#include "Creature.h"
#include "Fireball.h"
class Mage final : public Creature
{
public:
	Mage(POINT gridPosition, Grid* gridPtr, int health);

	virtual void Draw(Rectf rect) const override;

	virtual void Update(float elapsedSec) override;

	void Move();
	void Attack();
	virtual void TakeDamage(int damage);

	bool IsAlive() const;

	void SetLastBossPosition(POINT position);

	virtual Color4f GetColor() const override;
	virtual POINT GetGridPosition() const override;
	virtual Texture* GetTexturePtr() const override;


	virtual bool TurnDone() const override;

	virtual int GetAttackPriority() const override;
private:
	void CalculateNextMove();

	void DrawNextMoveArrow() const;
	Rectf m_Rect;
	Color4f m_Color;
	POINT m_GridPosition;
	Grid* m_GridPtr;
	POINT m_NextMove;
	POINT m_PreviousMove;
	std::vector<Fireball> m_FireballVect;
	bool m_Charging;
	POINT m_LastBossPosition;
	int m_MaxHealth;
	int m_CurrentHealth;

	int m_AttackStart{ 1 };

	Texture* m_InfoTexturePtr;
};