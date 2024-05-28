#pragma once
#include "BaseGame.h"
#include "Mage.h"
#include "Boss.h"
#include "Grid.h"
#include "Knight.h"
#include "Texture.h"
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


	void BossMove();
	void HeroMove();
	void BossAttack();
	void HeroAttack();

	Mage* m_Mage;
	Boss* m_Boss;
	Knight* m_Knight;
	Grid* m_Grid1Ptr;

	POINT m_BossMove{};
	int m_TurnCounter{ 0 };


	Texture* m_StunningText;

	float m_TimerBetweenMove{ 0.f };
	float m_TimerBetweenMoveAttack{ 0.f };
	float m_TimerBetweenAttack{ 0.f };

	bool m_TurnInProgress{ false };
};