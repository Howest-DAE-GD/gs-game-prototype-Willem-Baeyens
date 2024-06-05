#pragma once
#include "Texture.h"
class Creature
{
public:
	Creature();

	virtual Color4f GetColor() const = 0;
	virtual POINT GetGridPosition() const = 0;
	virtual Texture* GetTexturePtr() const;
	virtual void Draw(Rectf rect) const;
	virtual void Update(float elapsedSec);
	virtual void TakeDamage(int damage);
	virtual bool IsBoss() const;
	void DrawHealth(const Rectf& rect,int maxHealth, int currentHealth) const;
	virtual bool TurnDone() const = 0;

	void DrawInfo() const;

	virtual int GetAttackPriority() const;
	virtual int GetMovePriority() const;

	virtual void Move();
	virtual void Attack();

	virtual int GetHealth() const = 0;

	virtual bool IsDamagingHero() const;
private:
};

