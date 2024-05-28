#include "pch.h"
#include "Knight.h"
#include "utils.h"

Knight::Knight(POINT gridPos,Grid* GridPtr):
	m_GridPosition{gridPos},
	m_Color{ 211,211,211,1},
	m_NextMove{0,1},
	m_Attacking{false},
	m_AttackDuration{0.4f},
	m_AttackTimer{0.f},
	m_GridPtr{GridPtr},
	m_Stunned{false},
	m_MaxRowIndex{ GridPtr->GetRowCount() - 1},
	m_MaxHealth{10},
	m_CurrentHealth{m_MaxHealth}
{

}

void Knight::Update(float elapsedSec)
{
	if (not IsAlive()) return;

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
	return m_GridPosition;
}

void Knight::Draw(Rectf rect) const
{
	if (not IsAlive()) return;

	Creature::Draw(rect);
	if (m_Attacking)
	{
		Rectf attackRect{ rect.left+5.f,rect.bottom + rect.height / 3,rect.width * 3.5f,rect.height / 3 };
		utils::FillRect(attackRect);
	}
	DrawNextMoveArrow();
	DrawHealth(rect, m_MaxHealth, m_CurrentHealth);
}

void Knight::Move()
{
	if (not IsAlive()) return;

	m_GridPosition = m_GridPosition + m_NextMove;
	if (m_GridPosition.y == 0)
	{
		m_NextMove.y = 1;
	}
	if (m_GridPosition.y == m_MaxRowIndex)
	{
		m_NextMove.y = -1;
	}
}

void Knight::Attack()
{
	if (not IsAlive()) return;
}

void Knight::TakeDamage(int damage)
{
	m_CurrentHealth -= damage;
}

bool Knight::IsAlive() const
{
	return m_CurrentHealth > 0;
}

void Knight::Stun()
{
	m_Stunned = true;
}

void Knight::DrawNextMoveArrow() const
{
	utils::SetColor(Color4f{ 0,0,0,1.f });

	Point2f currentCenter = utils::GetCenter(m_GridPtr->GetRectAtPosition(m_GridPosition));
	int length{ m_GridPtr->GetSquareSize() };
	Point2f corner3 = currentCenter + (m_NextMove * (length * .3f));
	Point2f corner1 = currentCenter + m_NextMove * (length * .1f);
	Point2f corner2 = corner1;

	if (m_NextMove.x == 0)
	{
		corner1.x -= 12;
		corner2.x += 12;
	}
	else
	{
		corner1.y -= 12;
		corner2.y += 12;
	}
	utils::FillPolygon(std::vector<Point2f>{corner1, corner3, corner2});
}