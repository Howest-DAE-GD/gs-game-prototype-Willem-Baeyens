#include "pch.h"
#include "mage.h"
#include "utils.h"
#include "Fireball.h"
#include <iostream>
Mage::Mage(POINT gridPos, Grid* gridPtr) :
	m_GridPosition{ gridPos },
	m_Color{ 1,0,1,1 },
	m_GridPtr{ gridPtr },
	m_NextMove{},
	m_PreviousMove{},
	m_Charging{ false },
	m_FireballVect{},
	m_LastBossPosition{ 0,1 },
	m_MaxHealth{3},
	m_CurrentHealth{m_MaxHealth}
{
	m_Rect = Rectf{ 200,200,50,50 };
	CalculateNextMove();
}

void Mage::Draw(Rectf rect) const
{
	if ( not IsAlive()) return;
	Creature::Draw(rect);
	for (int index{}; index < m_FireballVect.size(); ++index)
	{
		m_FireballVect[index].Draw();
	}
	DrawNextMoveArrow();
	DrawHealth(rect, m_MaxHealth, m_CurrentHealth);
}

void Mage::Update(float elapsedSec)
{
	if (not IsAlive()) return;
	bool fireballsDone{ true };
	for (int index{}; index < m_FireballVect.size(); ++index)
	{
		if(not m_FireballVect[index].Update(elapsedSec)) fireballsDone = false;
	}
	if (fireballsDone)
	{
		m_FireballVect.clear();
	}
}

void Mage::Move()
{
	if (not IsAlive()) return;

	m_GridPosition = m_GridPosition + m_NextMove;
	m_PreviousMove = m_NextMove;
	CalculateNextMove();
}

void Mage::Attack()
{
	if (not IsAlive()) return;
	if (m_Charging)
	{
		int columnCount{ m_GridPtr->GetColumnCount() };
		for (int index{ m_AttackStart }; index < m_GridPtr->GetColumnCount() * m_GridPtr->GetRowCount(); index += 2)
		{
			POINT location{ index % columnCount,index / columnCount };
			m_FireballVect.push_back(Fireball(location, m_GridPtr));
		}
		++m_AttackStart %= 2;
	}
	else
	{
		for (int index{}; index < m_FireballVect.size(); ++index)
		{
			m_FireballVect[index].Explode();
		}
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

bool Mage::TurnDone() const
{
	return true;
}

void Mage::CalculateNextMove()
{
	bool possibleMove{ false };
	POINT move{};
	while (not possibleMove)
	{
		int moveNumber(rand() % 4);
		move = {};
		if (moveNumber > 3) std::cout << "Mage move error \n";
		switch (moveNumber)
		{
		case 0:   //left
			move.x = -1;
			break;
		case 1:	  //up
			move.y = 1;
			break;
		case 2:	  //right
			move.x = 1;
			break;
		case 3:	  //down
			move.y = -1;
			break;
		}
		possibleMove = m_GridPtr->IsInGrid(m_GridPosition + move) and not (move == m_PreviousMove);
	}

	m_NextMove = move;
}

void Mage::DrawNextMoveArrow() const
{
	utils::SetColor(Color4f{ 0,0,0,1.f });

	Point2f currentCenter	= utils::GetCenter(m_GridPtr->GetRectAtPosition(m_GridPosition));
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

