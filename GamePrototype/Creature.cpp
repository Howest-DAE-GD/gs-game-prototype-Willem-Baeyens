#include "pch.h"
#include "Creature.h"
#include "utils.h"

Creature::Creature()
{
}

void Creature::Draw(Rectf rect) const
{
	utils::SetColor(GetColor());
	utils::FillRect(rect);
}

void Creature::TakeDamage(int damage)
{
}

void Creature::DrawHealth(const Rectf& rect, int maxHealth, int currentHealth) const
{
	Rectf currentHealthRect = rect;
	currentHealthRect.bottom += rect.height * 1.1f;
	currentHealthRect.height = rect.height / 5;
	currentHealthRect.left += rect.width * 0.05f;
	currentHealthRect.width = (currentHealthRect.width * 0.9f);

	utils::SetColor(Color4f{ 1,0,0,1 });
	utils::FillRect(currentHealthRect);
	currentHealthRect.width *= float(currentHealth) / maxHealth;
	utils::SetColor(Color4f{ 0,1,0,1 });
	utils::FillRect(currentHealthRect);
}
