#pragma once
#include "Creature.h"
#include <vector>
#include "vector2f.h"
class Grid
{
public:
	Grid(int columnCount, int rowCount, int squareSize,Vector2f location);
	Grid() = default;

	void AddCreature(Creature* creaturePtr);

	void Draw() const;

	void DoDamage(POINT gridPosition, int damage);
	int GetSquareSize() const;
	Vector2f GetLocation() const;

	int GetRowCount() const;
	int GetColumnCount() const;

	Rectf GetRectAtPosition(POINT position) const;

	Creature* GetCreatureAtPosition(POINT position) const;

	bool IsInGrid(POINT position) const;

	std::vector<POINT> GetCreaturePositions() const;
private:
	int m_ColumnCount;
	int m_RowCount;
	int m_SquareSize;
	Vector2f m_Location;
	std::vector<Creature*> m_CreaturePtrVect;
};

