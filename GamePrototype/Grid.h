#pragma once
class Creature;
class Boss;
class BossPatternDisplay;
#include <vector>
#include "vector2f.h"
#include "BossPatternDisplay.h"
#include <map>
class Grid
{
public:
	Grid(int columnCount, int rowCount, int squareSize,Vector2f location);
	Grid() = default;

	void AddCreature(Creature* creaturePtr);
	void AddBoss(Boss* cossPtr);
	void CreateHeroOrder();

	void SetPattern(BossPatternDisplay* patternPtr);

	void Draw() const;

	void Update(float elapsedSec);

	void DoDamage(POINT gridPosition, int damage);
	int GetSquareSize() const;
	Vector2f GetLocation() const;

	int GetRowCount() const;
	int GetColumnCount() const;

	Rectf GetRectAtPosition(POINT position) const;

	Creature* GetCreatureAtPosition(POINT position) const;

	bool IsInGrid(POINT position) const;

	std::vector<POINT> GetCreaturePositions() const;

	bool checkMoveHero(Creature* moving, POINT newPos);

	void Click(const SDL_MouseButtonEvent& e);

	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e);

	POINT GetBossPosition() const;
private:
	void BossMove();
	void HeroMove();
	void BossAttack();
	void HeroAttack();

	void CheckIfAllFinished();

	int m_TurnCounter;

	int m_ColumnCount;
	int m_RowCount;
	int m_SquareSize;
	Vector2f m_Location;
	std::vector<Creature*> m_CreaturePtrVect;
	Boss* m_Boss;

	std::multimap<int, Creature*> m_HeroAttackOrder;
	std::multimap<int, Creature*> m_HeroMoveOrder;

	Creature* m_CurrentInfoDisplay{ nullptr };

	float m_TimerBetweenMove{ 0.f };
	float m_TimerBetweenMoveAttack{ 0.f };
	float m_TimerBetweenAttack{ 0.f };

	bool m_TurnInProgress{ false };
	bool m_AllFinished{ true };

	BossPatternDisplay* m_BossPatternPtr;

	POINT m_BossMove{};
};

