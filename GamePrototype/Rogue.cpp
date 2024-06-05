#include "pch.h"
#include "Rogue.h"
#include "utils.h"

Rogue::Rogue(POINT gridPos, Grid* gridPtr, Boss* bossPtr,int health):
	m_GridPosition{gridPos},
	m_Boss{bossPtr},
	m_GridPtr{gridPtr},
	m_MaxHealth{health},
	m_CurrentHealth{m_MaxHealth},
	m_Color{ Color4f{ 2 / 255.f, 6 / 255.f, 23 / 255.f,1.f } },
	m_ExtraStep{false},
	m_ExtraStepTimer{0.f},
	m_NextStep{}
{
	m_InfoTexturePtr = new Texture("RogueInfo.png");
}

void Rogue::Update(float elapsedSec)
{
	if (not IsAlive()) return;
	if (m_ExtraStep)
	{
		m_ExtraStepTimer += elapsedSec;
		if (m_ExtraStepTimer >= 0.2f)
		{
			Step();
			m_ExtraStep = false;
			m_ExtraStepTimer = 0.f;
		}
	}

	if (m_Rotating)
	{
		m_ExtraStepTimer += elapsedSec;
		if (m_ExtraStepTimer >= 0.2f)
		{
			m_GridPosition = m_GridPosition + m_NextStep;
			m_Rotating = false;
			m_ExtraStepTimer = 0.f;
		}
	}
}

Color4f Rogue::GetColor() const
{
	return m_Color;
}

POINT Rogue::GetGridPosition() const
{
	return m_GridPosition;
}

Texture* Rogue::GetTexturePtr() const
{
	return m_InfoTexturePtr;
}

void Rogue::Draw(Rectf rect) const
{
	if (not IsAlive()) return;
	Creature::Draw(rect);
	DrawHealth(rect, m_MaxHealth, m_CurrentHealth);
}

void Rogue::Move()
{
	if (not IsAlive()) return;
	m_NextStep = {};
	m_ExtraStep = false;
	int distance{ OrthogonalDistance(m_GridPosition,m_Boss->GetGridPosition()) };
	if (distance == 1)
	{
		Rotate();
		return;
	}
	Step();

	distance = OrthogonalDistance(m_GridPosition, m_Boss->GetGridPosition());
	if (distance <= 1) return;

	m_ExtraStep = true;
}

void Rogue::Attack()
{
	if (not IsAlive()) return;

	ChangeColorIfAllyInRange();
	if (OrthogonalDistance(m_GridPosition, m_Boss->GetGridPosition()) == 1)
	{
		int damage = 1;
		if (AllyInBossRange()) damage = 2;
		m_Boss->TakeDamage(damage);
	}
}

void Rogue::TakeDamage(int damage)
{
	m_CurrentHealth -= damage;
}

bool Rogue::IsAlive() const
{
	return m_CurrentHealth > 0;
}

bool Rogue::TurnDone() const
{
	return m_ExtraStepTimer == 0.f;
}

int Rogue::GetAttackPriority() const
{
	return 1;
}

int Rogue::GetMovePriority() const
{
	return 1;
}

bool Rogue::AllyInBossRange()
{
	std::vector<POINT> creaturePositions{ m_GridPtr->GetCreaturePositions() };
	for (int index{};index < creaturePositions.size();++index)
	{
		POINT pos = creaturePositions[index];
		if (pos == m_GridPosition) continue;
		if (pos == m_Boss->GetGridPosition()) continue;
		POINT difference = Difference(pos, m_Boss->GetGridPosition());
		float distanceSquared = difference.x * difference.x + difference.y * difference.y;
		if (distanceSquared <= 2) return true;
	}

	return false;
}

void Rogue::Step()
{
	POINT diffBoss{ Difference(m_Boss->GetGridPosition(),m_GridPosition) };
	if (diffBoss.x > 1) {
		m_NextStep.x = +1;
	}
	else if (diffBoss.x < -1) {
		m_NextStep.x  = -1;
	}
	else if (diffBoss.y > 1) {
		m_NextStep.y = +1;
	}
	else if (diffBoss.y < -1) {
		m_NextStep.y = -1;
	}
	else if (diffBoss.x > 0) {
		m_NextStep.x = +1;
	}
	else if (diffBoss.x < 0) {
		m_NextStep.x = -1;
	}

	m_GridPosition = m_GridPosition + m_NextStep;
	ChangeColorIfAllyInRange();
	m_NextStep = {};
}

void Rogue::ChangeColorIfAllyInRange()
{
	if (AllyInBossRange() and OrthogonalDistance(m_Boss->GetGridPosition(), m_GridPosition) == 1) m_Color = Color4f{ 31 / 255.f, 69 / 255.f, 130 / 255.f ,1.f };
	else m_Color = Color4f{ 2 / 255.f, 6 / 255.f, 23 / 255.f,1.f };
}

void Rogue::Rotate(bool clockwise)
{
	POINT diffBoss{ Difference(m_Boss->GetGridPosition(),m_GridPosition) };

	POINT firstMove{};
	POINT secondMove{};
	if (diffBoss.y == -1)//up
	{
		if (clockwise)
		{
			firstMove = { 1,0 };
			secondMove = { 0,-1 };
		}
		else
		{
			firstMove = { -1,0 };
			secondMove = { 0,-1 };
		}
	}
	if (diffBoss.y == 1)//down
	{
		if (clockwise)
		{
			firstMove = { -1,0 };
			secondMove = { 0,1 };
		}
		else
		{
			firstMove = { 1,0 };
			secondMove = { 0,1 };
		}
	}
	if (diffBoss.x == -1)//right
	{
		if (clockwise)
		{
			firstMove = { 0,-1 };
			secondMove = { -1,0 };
		}
		else
		{
			firstMove = { 0,1 };
			secondMove = { -1,0 };
		}
	}
	if (diffBoss.x == 1)//left
	{
		if (clockwise)
		{
			firstMove = { 0,1 };
			secondMove = { 1,0 };
		}
		else
		{
			firstMove = { 0,1 };
			secondMove = { 1,0 };
		}
	}

	if (not clockwise)
	{
		if (m_GridPtr->checkMoveHero(this, m_GridPosition + firstMove) and m_GridPtr->checkMoveHero(this, m_GridPosition + firstMove + secondMove))
		{
			m_GridPosition = m_GridPosition + firstMove;
			m_NextStep = secondMove;
			m_Rotating = true;
		}
		else
		{
			Rotate(true);
		}
	}
	if (clockwise)
	{
		if (m_GridPtr->checkMoveHero(this, m_GridPosition + firstMove) and m_GridPtr->checkMoveHero(this, m_GridPosition + firstMove + secondMove))
		{
			m_GridPosition = m_GridPosition + firstMove;
			m_NextStep = secondMove;
			m_Rotating = true;
		}
		else return;
	}

}

POINT Difference(POINT lhs, POINT rhs)
{
	return POINT{ lhs.x - rhs.x,lhs.y - rhs.y };
}

int OrthogonalDistance(POINT lhs, POINT rhs)
{
	POINT difference = Difference(lhs, rhs);
	return  (abs(difference.x) + abs(difference.y)) ;
}
