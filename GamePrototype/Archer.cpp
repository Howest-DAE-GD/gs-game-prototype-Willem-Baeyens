#include "pch.h"
#include "Archer.h"
#include <iostream>
#include "utils.h"

Archer::Archer(POINT gridPos, Grid* gridPtr, Boss* bossPtr, int health):
	m_GridPosition{gridPos},
	m_GridPtr{gridPtr},
	m_Boss{bossPtr},
	m_MaxHealth{health},
	m_CurrentHealth{m_MaxHealth},
	m_Color{ 74/255.f,103/255.f,65/255.f ,1.f},
	m_Stepping{false},
	m_StepTimer{0.f},
	m_ArrowVect{}
{
	m_InfoTexturePtr = new Texture("ArcherInfo.png");
}

void Archer::Update(float elapsedSec)
{
	if (not IsAlive()) return;

	if (m_Stepping)
	{
		m_StepTimer += elapsedSec;
		if (m_StepTimer >= 0.2f)
		{
			m_StepTimer = 0.f;
			m_Stepping = Step();
		}
	}

	for (int index{}; index < m_ArrowVect.size(); ++index)
	{
		m_ArrowVect[index].Update(elapsedSec);
		if(index == m_ArrowVect.size()-1) RemoveArrows();
	}
}

Color4f Archer::GetColor() const
{
	return m_Color;
}

POINT Archer::GetGridPosition() const
{
	return m_GridPosition;
}

Texture* Archer::GetTexturePtr() const
{
	return m_InfoTexturePtr;
}

void Archer::Draw(Rectf rect) const
{
	if (not IsAlive()) return;

	Creature::Draw(rect);
	DrawHealth(rect, m_MaxHealth, m_CurrentHealth);
	DrawMoveArrow();
	for (int index{}; index < m_ArrowVect.size(); ++index)
	{
		m_ArrowVect[index].Draw();
	}
}

void Archer::ArcherMove()
{
	if (not IsAlive()) return;

	m_Stepping = false;
	if (m_GridPosition.y != m_Boss->GetGridPosition().y)
	{
		m_Stepping = Step();
	}
}

void Archer::Attack()
{
	if (not IsAlive()) return;

	POINT bossPos = m_Boss->GetGridPosition();
	if (bossPos.y == m_GridPosition.y)
	{
		m_Boss->TakeDamage(1);
		Rectf archerRect{ m_GridPtr->GetRectAtPosition(m_GridPosition) };
		Rectf bossRect{ m_GridPtr->GetRectAtPosition(m_GridPtr->GetBossPosition()) };
		Point2f start = Point2f{ utils::GetCenter(archerRect)};
		start.x += archerRect.width / 2;
		Point2f end = Point2f{ utils::GetCenter(bossRect)};
		end.x -= bossRect.width / 2;
		m_ArrowVect.emplace_back(Arrow{ start,end });
	}



	ArcherMove();
}

void Archer::TakeDamage(int damage)
{
	m_CurrentHealth -= damage;
}

bool Archer::IsAlive() const
{
	return m_CurrentHealth > 0;
}

bool Archer::TurnDone() const
{
	return m_StepTimer == 0.f;
}

int Archer::GetAttackPriority() const
{
	return 2;
}

int Archer::GetMovePriority() const
{
	return -1;
}

bool Archer::IsDamagingHero() const
{
	return true;
}

int Archer::GetHealth() const
{
	return m_CurrentHealth;
}

void Archer::PrintInfo() const
{
	std::cout << "Stays in the backline. Each turn it will attack the boss if they're on the same horizontal line.\nMoves toward the boss after (attempted)attacking\n";
}

bool Archer::Step()
{
	POINT bossPos = m_Boss->GetGridPosition();
	POINT nextPos{ m_GridPosition };
	if (bossPos.y > m_GridPosition.y) nextPos.y += 1;
	else if (bossPos.y < m_GridPosition.y) nextPos.y -= 1;
	else return false;

	if (m_GridPtr->checkMoveHero(this, nextPos))
	{
		m_GridPosition = nextPos;
		return true;
	}
	return false;
}

void Archer::DrawMoveArrow() const
{
	if (m_GridPosition.y == m_GridPtr->GetBossPosition().y) return;

	Point2f currentCenter = utils::GetCenter(m_GridPtr->GetRectAtPosition(m_GridPosition));

	Point2f corner1{}, corner2{}, corner3{currentCenter};
	int length{ m_GridPtr->GetSquareSize() };

	if (m_GridPosition.y > m_GridPtr->GetBossPosition().y)
	{

		corner3.y -= length * .3f;
		corner1 = corner2 = corner3;
		corner1.x -= 12;
		corner2.x += 12;
		corner1.y += length * 0.2f;
		corner2.y += length * 0.2f;
	}
	else
	{
		corner3.y += length * .3f;
		corner1 = corner2 = corner3;
		corner1.x -= 12;
		corner2.x += 12;
		corner1.y -= length * 0.2f;
		corner2.y -= length * 0.2f;
	}
	utils::SetColor(Color4f{ 0,0,0,1.f });
	utils::FillPolygon(std::vector<Point2f>{corner1, corner3, corner2});
}

void Archer::RemoveArrows()
{
	for (auto it{ m_ArrowVect.begin() }; it != m_ArrowVect.end(); ++it)
	{
		if ((*it).IsDone())
		{
			m_ArrowVect.erase(it);
			RemoveArrows();
			return;
		}
	}
}
