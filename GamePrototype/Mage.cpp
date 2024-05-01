#include "pch.h"
#include "mage.h"
#include "utils.h"
#include "Fireball.h"
Mage::Mage(POINT gridPos, Grid* enemyGrid) :
	m_GridPosition{ gridPos },
	m_Color{ 1,0,1,1 },
	m_EnemyGridPtr{ enemyGrid },
	m_Charging{false},
	m_Fireball{Fireball({-100,-1},enemyGrid)},
	m_LastBossPosition{0,1}
{
	m_Rect = Rectf{ 200,200,50,50 };
}

void Mage::Draw(Rectf rect) const
{
	Creature::Draw(rect);


	m_Fireball.Draw();
	
}

void Mage::Update(float elapsedSec)
{
	m_Fireball.Update(elapsedSec);
}

void Mage::DoTurn()
{
	if (m_Charging)
	{
		m_Fireball = Fireball(m_LastBossPosition, m_EnemyGridPtr);
	}
	else
	{
		m_Fireball.Explode();
	}
	m_Charging = !m_Charging;
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

