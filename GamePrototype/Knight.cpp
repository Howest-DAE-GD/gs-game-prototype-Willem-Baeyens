#include "pch.h"
#include "Knight.h"
#include "utils.h"

Knight::Knight(POINT gridPos,Grid* GridPtr,Grid* enemyGridPtr):
	m_Grid_Position{gridPos},
	m_Color{ 211,211,211,1},
	m_NextMove{1},
	m_Attacking{false},
	m_AttackDuration{0.4f},
	m_AttackTimer{0.f},
	m_GridPtr{GridPtr},
	m_EnemyGridPtr{enemyGridPtr},
	m_Stunned{false},
	m_MaxRowIndex{ GridPtr->GetRowCount() - 1}
{

}

void Knight::Update(float elapsedSec)
{
	if (m_Attacking)
	{
		m_AttackTimer += elapsedSec;
		if (m_AttackTimer > m_AttackDuration)
		{
			m_Attacking = false;
			m_AttackTimer = 0.f;
		}
	}
}

Color4f Knight::GetColor() const
{
	return m_Color;
}

POINT Knight::GetGridPosition() const
{
	return m_Grid_Position;
}

void Knight::Draw(Rectf rect) const
{
	Creature::Draw(rect);
	if (m_Attacking)
	{
		Rectf attackRect{ rect.left+5.f,rect.bottom + rect.height / 3,rect.width * 3.5f,rect.height / 3 };
		utils::FillRect(attackRect);
	}
}

void Knight::DoTurn()
{
	if (not m_Stunned)
	{
		m_Grid_Position.y += m_NextMove;
		if (m_Grid_Position.y == 0)
		{
			m_NextMove = 1;
		}
		if (m_Grid_Position.y == m_MaxRowIndex)
		{
			m_NextMove = -1;
		}
	}
	m_Attacking = true;
	m_EnemyGridPtr->DoDamage({ 0,m_Grid_Position.y }, 1);
	m_Stunned = false;
}

void Knight::Stun()
{
	m_Stunned = true;
}
