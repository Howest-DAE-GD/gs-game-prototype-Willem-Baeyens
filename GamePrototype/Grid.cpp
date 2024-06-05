#include "pch.h"
#include "Grid.h"
#include "utils.h"
#include <iostream>
#include "Creature.h"


Grid::Grid(int columnCount, int rowCount, int squareSize, Vector2f location):
	m_ColumnCount{columnCount},
	m_RowCount{rowCount},
	m_SquareSize{squareSize},
	m_Location{location},
	m_TurnCounter{0},
	m_Boss{nullptr},
	m_HeroAttackOrder{},
	m_HeroMoveOrder{},
	m_BossPatternPtr{}
{
}

void Grid::AddCreature(Creature* creaturePtr)
{
	m_CreaturePtrVect.push_back(creaturePtr);
}

void Grid::AddBoss(Boss* bossPtr)
{
	m_Boss = bossPtr;
	AddCreature(bossPtr);
}

void Grid::SetPattern(BossPatternDisplay* patternPtr)
{
	m_BossPatternPtr = patternPtr;
}

void Grid::Draw() const
{
	m_Boss->Draw(GetRectAtPosition(m_Boss->GetGridPosition()));
	utils::SetColor(Color4f{ 0.1f,0.1f,0.1f,1.f });
	for (int rowIndex = 0; rowIndex < m_RowCount; ++rowIndex)
	{
		for (int colIndex = 0; colIndex < m_ColumnCount; ++colIndex)
		{
			Rectf rect = GetRectAtPosition(POINT{ colIndex,rowIndex });
			utils::DrawRect(rect);
		}
	}
	for (Creature* creaturePtr : m_CreaturePtrVect)
	{
		POINT gridPos = creaturePtr->GetGridPosition();
		Rectf rect = GetRectAtPosition(gridPos);
		if (creaturePtr->IsBoss())
		{
			continue;
		}
		creaturePtr->Draw(rect);
	}

	if (m_CurrentInfoDisplay)
	{
		m_CurrentInfoDisplay->DrawInfo();
	}

	m_BossPatternPtr->Draw();
}

void Grid::Update(float elapsedSec)
{
	if (m_CurrentInfoDisplay) return;

	if (m_TimerBetweenMove > 0.f)
	{
		m_TimerBetweenMove += elapsedSec;
		if (m_TimerBetweenMove > 0.5f) {
			HeroMove();
		}
	}
	if (m_TimerBetweenMoveAttack > 0.f)
	{
		m_TimerBetweenMoveAttack += elapsedSec;
		if (m_TimerBetweenMoveAttack > 0.5f) {
			BossAttack();
		}
	}
	if (m_TimerBetweenAttack > 0.f)
	{
		m_TimerBetweenAttack += elapsedSec;
		if (m_TimerBetweenAttack > 0.2f) {
			HeroAttack();
		}
	}

	for (int index{}; index < m_CreaturePtrVect.size(); ++index)
	{
		m_CreaturePtrVect[index]->Update(elapsedSec);
	}

	if (not m_TurnInProgress and not m_AllFinished)
	{
		CheckIfAllFinished();
		if (m_AllFinished) m_BossPatternPtr->Turn();
	}
}

bool Grid::IsLevelWon() const
{
	return m_Boss->GetHealth() <= 0;
}

bool Grid::IsLevelLost() const
{
	for (int index{}; index < m_CreaturePtrVect.size(); ++index)
	{
		if(m_CreaturePtrVect[index]->IsDamagingHero()) return false;
	}

	return true;
}

void Grid::DoDamage(POINT gridPosition, int damage)
{
	for (Creature* creaturePtr : m_CreaturePtrVect)
	{
		if (creaturePtr->GetGridPosition().x == gridPosition.x and creaturePtr->GetGridPosition().y == gridPosition.y)
		{
			creaturePtr->TakeDamage(damage);
		}
	}
}

int Grid::GetSquareSize() const
{
	return m_SquareSize;
}

Vector2f Grid::GetLocation() const
{
	return m_Location;
}

int Grid::GetRowCount() const
{
	return m_RowCount;
}

int Grid::GetColumnCount() const
{
	return m_ColumnCount;
}

Rectf Grid::GetRectAtPosition(POINT position) const
{
	Rectf rect{ (float)position.x * m_SquareSize,(float)position.y * m_SquareSize,(float)m_SquareSize,(float)m_SquareSize };
	rect += m_Location;

	return rect;
}

Creature* Grid::GetCreatureAtPosition(POINT position) const
{
	for (Creature* creaturePtr : m_CreaturePtrVect)
	{
		if (position == creaturePtr->GetGridPosition())
		{
			return creaturePtr;
		}
	}
	return nullptr;
}

bool Grid::IsInGrid(POINT position) const
{
	if (position.x < 0 or position.y < 0) return false;
	if (position.x >= m_ColumnCount or position.y >= m_RowCount) return false;
	return true;
}



std::vector<POINT> Grid::GetCreaturePositions() const
{
	std::vector<POINT> positions{};
	for (Creature* creaturePtr : m_CreaturePtrVect)
	{
		positions.push_back(creaturePtr->GetGridPosition());
	}
	return positions;
}

bool Grid::checkMoveHero(Creature* moving, POINT newPos)
{
	for (Creature* creaturePtr : m_CreaturePtrVect)
	{
		if (creaturePtr == moving) continue;
		if (newPos == creaturePtr->GetGridPosition()) return false;
	}

	return true;
}

void Grid::Click(const SDL_MouseButtonEvent& e)
{
	std::vector<Rectf> creatureGrids;
	std::vector<POINT> positions{ GetCreaturePositions()};
	for (int index{};index<positions.size();++index)
	{
		creatureGrids.push_back(GetRectAtPosition(positions[index]));
	}
	Point2f mousePos{ float(e.x),float(e.y) };
	for (int index{}; index < positions.size(); ++index)
	{
		if (utils::IsPointInRect(mousePos, creatureGrids[index]))
		{
			m_CurrentInfoDisplay = GetCreatureAtPosition(positions[index]);
			return;
		}
	}
	m_CurrentInfoDisplay = nullptr;
}

void Grid::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	if (m_CurrentInfoDisplay) return;

	if (m_Boss->GetHealth() <= 0) return;

	if (not m_AllFinished) return;
	switch (e.keysym.sym)
	{
	case SDLK_RETURN:
		m_BossMove = {};
		BossMove();
		break;
	case SDLK_LEFT:
		m_BossMove = { -1,0 };
		BossMove();
		break;
	case SDLK_RIGHT:
		m_BossMove = { 1,0 };
		BossMove();
		break;
	case SDLK_UP:
		m_BossMove = { 0,1 };
		BossMove();
		break;
	case SDLK_DOWN:
		m_BossMove = { 0,-1 };
		BossMove();
		break;
	}
}

POINT Grid::GetBossPosition() const
{
	return m_Boss->GetGridPosition();
}

void Grid::RemoveDeadHeroes()
{
	for (auto it{m_CreaturePtrVect.begin()}; it != m_CreaturePtrVect.end(); ++it)
	{
		if ((*it)->GetHealth() == 0)
		{
			m_CreaturePtrVect.erase(it);
			RemoveDeadHeroes();
			return;
		}
	}
}

void Grid::BossMove()
{
	if (not m_Boss->CheckMove(m_BossMove)) return;

	++m_TurnCounter;
	m_TurnInProgress = true;
	m_AllFinished = false;

	m_Boss->Move(m_BossMove);
	m_BossMove = {};

	m_TimerBetweenMove = { 0.0001f };
}

void Grid::HeroMove()
{
	m_TimerBetweenMove = 0.f;
	for (auto it{ m_HeroMoveOrder.begin() }; it != m_HeroMoveOrder.end(); ++it)
	{
		(*it).second->Move();
	}
	m_TimerBetweenMoveAttack = { 0.0001f };
}

void Grid::BossAttack()
{
	m_TimerBetweenMoveAttack = 0.f;
	m_Boss->Attack();
	m_TimerBetweenAttack = { 0.0001f };

}

void Grid::HeroAttack()
{
	m_Boss->HitEnemies();
	m_TimerBetweenAttack = 0.f;
	for (auto it{ m_HeroAttackOrder.begin() }; it != m_HeroAttackOrder.end(); ++it)
	{
		(*it).second->Attack();
	}
	m_TimerBetweenAttack = { 0.f };
	m_TurnInProgress = false;
	std::cout << "Turn " << m_TurnCounter << "\n";
	RemoveDeadHeroes();
}

void Grid::CheckIfAllFinished()
{
	bool allFinished = true;
	for (int index{}; index < m_CreaturePtrVect.size(); ++index)
	{
		if (not m_CreaturePtrVect[index]->TurnDone()) allFinished = false;
	}
	m_AllFinished = allFinished;
}

void Grid::CreateHeroOrder()
{
	for (int index{}; index < m_CreaturePtrVect.size(); ++index)
	{
		Creature* creaturePtr{ m_CreaturePtrVect[index] };
		int attackPrio{ creaturePtr->GetAttackPriority() };
		int movePrio{ creaturePtr->GetMovePriority() };
		if (attackPrio != -1)
		{
			m_HeroAttackOrder.insert({ attackPrio,creaturePtr });
		}
		if (movePrio != -1)
		{
			m_HeroMoveOrder.insert({ attackPrio,creaturePtr });
		}
	}
}
