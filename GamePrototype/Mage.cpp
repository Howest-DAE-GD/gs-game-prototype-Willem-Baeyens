#include "pch.h"
#include "mage.h"
#include "utils.h"
#include "Fireball.h"
Mage::Mage(POINT gridPos, Grid* gridPtr) :
	m_GridPosition{ gridPos },
	m_Color{ 1,0,1,1 },
	m_GridPtr{ gridPtr },
	m_Charging{ false },
//	m_Fireball{ Fireball({-100,-1},enemyGrid) },
	m_LastBossPosition{ 0,1 },
	m_MaxHealth{3},
	m_CurrentHealth{m_MaxHealth}
{
	m_Rect = Rectf{ 200,200,50,50 };
}

void Mage::Draw(Rectf rect) const
{
	if ( not IsAlive()) return;
	Creature::Draw(rect);
	//m_Fireball.Draw();
	DrawHealth(rect, m_MaxHealth, m_CurrentHealth);
}

void Mage::Update(float elapsedSec)
{
	if (not IsAlive()) return;
	//m_Fireball.Update(elapsedSec);
}

void Mage::DoTurn()
{
	if (not IsAlive()) return;
	if (m_Charging)
	{
		//m_Fireball = Fireball(m_LastBossPosition, m_EnemyGridPtr);
	}
	else
	{
		//m_Fireball.Explode();
	}
	m_Charging = !m_Charging;
}

void Mage::TakeDamage(int damage)
{
	m_CurrentHealth -= damage;
}

bool Mage::IsAlive() const
{
	return (m_CurrentHealth > 0);
}

void Mage::SetLastBossPosition(POINT position)
{
	m_LastBossPosition = { position };
}


Color4f Mage::GetColor() const
{
	return m_Color;
}

POINT Mage::GetGridPosition() const
{
	return m_GridPosition;
}

