#include "pch.h"
#include "mage.h"
#include "utils.h"
#include "Fireball.h"
#include <iostream>
Mage::Mage(POINT gridPos, Grid* gridPtr,int health) :
	m_GridPosition{ gridPos },
	m_Color{ 1,0,1,1 },
	m_GridPtr{ gridPtr },
	m_NextMove{},
	m_PreviousMove{},
	m_Charging{ false },
	m_FireballVect{},
	m_LastBossPosition{ 0,1 },
	m_MaxHealth{health},
	m_CurrentHealth{m_MaxHealth}
{
	m_Rect = Rectf{ 200,200,50,50 };
	CalculateNextMove();
	m_InfoTexturePtr = new Texture("MageInfo.png");
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
}

void Mage::Attack()
{
	if (not IsAlive()) return;
	if (m_Charging)
	{
		POINT bossPos = m_GridPtr->GetBossPosition();
		int direction1{}, direction2{};
		while (direction1 == direction2)
		{
			direction1 = { rand() % 4 };
			direction2 = { rand() % 4 };
		}
		POINT location1{ bossPos }, location2{ bossPos };
		switch (direction1)
		{
		case 0:
			location1.y += 1;
			break;
		case 1:
			location1.x += 1;
			break;
		case 2:
			location1.y -= 1;
			break;
		case 3:
			location1.x -= 1;
			break;
		}
		switch (direction2)
		{
		case 0:
			location2.y += 1;
			break;
		case 1:
			location2.x += 1;
			break;
		case 2:
			location2.y -= 1;
			break;
		case 3:
			location2.x -= 1;
			break;
		}

		m_FireballVect.push_back(Fireball(location1, m_GridPtr));
		m_FireballVect.push_back(Fireball(location2, m_GridPtr));
	}
	else
	{
		for (int index{}; index < m_FireballVect.size(); ++index)
		{
			m_FireballVect[index].Explode();
		}
	}
	m_Charging = !m_Charging;
	CalculateNextMove();
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

Texture* Mage::GetTexturePtr() const
{
	return m_InfoTexturePtr;
}

bool Mage::TurnDone() const
{
	return true;
}

int Mage::GetAttackPriority() const
{
	return 3;
}

bool Mage::IsDamagingHero() const
{
	return true;
}

int Mage::GetHealth() const
{
	return m_CurrentHealth;
}

void Mage::PrintInfo() const
{
	std::cout << "Moves randomly, will shoot fireballs around the boss every 2 turns.\n";
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
	if (m_GridPtr->checkMoveHero(this, m_GridPosition + move))
	{
		m_NextMove = move;
	}
	else CalculateNextMove();
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

