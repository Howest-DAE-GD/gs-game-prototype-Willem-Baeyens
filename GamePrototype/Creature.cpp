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
