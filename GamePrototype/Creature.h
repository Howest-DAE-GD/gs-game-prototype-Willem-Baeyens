#pragma once
class Creature
{
public:
	Creature();

	virtual Color4f GetColor() const = 0;
	virtual POINT GetGridPosition() const = 0;
	virtual void Draw(Rectf rect) const;
	virtual void TakeDamage(int damage);
	virtual bool IsBoss() const;
	void DrawHealth(const Rectf& rect,int maxHealth, int currentHealth) const;
	virtual bool TurnDone() const = 0;
private:
};

