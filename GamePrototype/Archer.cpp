#include "pch.h"
#include "Archer.h"
#include <iostream>

Archer::Archer(POINT gridPos, Grid* gridPtr, Boss* bossPtr):
	m_GridPosition{gridPos},
	m_GridPtr{gridPtr},
	m_Boss{bossPtr},
	m_MaxHealth{4},
	m_CurrentHealth{m_MaxHealth},
	m_Color{ 74/255.f,103/255.f,65/255.f ,1.f},
	m_Stepping{false},
	m_StepTimer{0.f}
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
	if (bossPos.y == m_GridPosition.y) m_Boss->TakeDamage(1);

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
