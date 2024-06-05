#pragma once
#include "BaseGame.h"
#include "Mage.h"
#include "Knight.h"
#include "Rogue.h"
#include "Archer.h"
#include "Boss.h"
#include "Grid.h"
#include "Texture.h"
#include "BossPatternDisplay.h"
class Game : public BaseGame
{
public:
	explicit Game(const Window& window);
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game(Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update(float elapsedSec) override;
	void Draw() const override;

	// Event handling
	void ProcessKeyDownEvent(const SDL_KeyboardEvent& e) override;
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e) override;
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e) override;
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e) override;
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e) override;

private:

	// FUNCTIONS
	void Initialize();
	void Cleanup();
	void ClearBackground() const;

	Mage*	m_Mage1;
	Knight* m_Knight1;
	Rogue*	m_Rogue1;
	Archer* m_Archer1;
	Boss*	m_Boss1;
	Grid*	m_Grid1Ptr;
	
	Mage*	m_Mage2;
	Knight* m_Knight2;
	Boss*	m_Boss2;

	Creature* m_CurrentInfoDisplay{ nullptr };

	POINT m_BossMove{};
	int m_TurnCounter{ 0 };

	BossPatternDisplay* m_BossPatternPtr;

	float m_TimerBetweenMove{ 0.f };
	float m_TimerBetweenMoveAttack{ 0.f };
	float m_TimerBetweenAttack{ 0.f };

	bool m_TurnInProgress{ false };
	bool m_AllFinished{ true };

	int m_Level{1};
};