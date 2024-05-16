#include "pch.h"
#include "Grid.h"
#include "utils.h"

Grid::Grid(int columnCount, int rowCount, int squareSize, Vector2f location):
	m_ColumnCount{columnCount},
	m_RowCount{rowCount},
	m_SquareSize{squareSize},
	m_Location{location}
{
}

void Grid::AddCreature(Creature* creaturePtr)
{
	m_CreaturePtrVect.push_back(creaturePtr);
}

void Grid::Draw() const
{
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
		creaturePtr->Draw(rect);
	}
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



std::vector<POINT> Grid::GetCreaturePositions() const
{
	std::vector<POINT> positions{};
	for (Creature* creaturePtr : m_CreaturePtrVect)
	{
		;
		positions.push_back(creaturePtr->GetGridPosition());
	}
	return positions;
}
