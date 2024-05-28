#include "pch.h"
#include "Game.h"
#include "utils.h"
#include <iostream>

Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_StunningText = new Texture("Stunning next turn", "DIN-Light.otf", 30, Color4f{ 1,1,1,1 });


	m_Grid1Ptr = new Grid(9, 5, 60, Vector2f{ 145, 120 });

	m_Mage = new Mage{ POINT{1,3},m_Grid1Ptr };
	m_Knight = new Knight{ POINT{4,2},m_Grid1Ptr};
	m_Boss = new Boss{ POINT{6,2},m_Knight,m_Grid1Ptr};
	m_Rogue = new Rogue{ POINT{2,1},m_Grid1Ptr,m_Boss };

	m_Grid1Ptr->AddCreature(m_Mage);
	m_Grid1Ptr->AddCreature(m_Knight);
	m_Grid1Ptr->AddCreature(m_Rogue);
	m_Grid1Ptr->AddCreature(m_Boss);

}

void Game::Cleanup( )
{
}

void Game::Update( float elapsedSec )
{
	m_Mage->Update(elapsedSec);
	m_Knight->Update(elapsedSec);
	m_Rogue->Update(elapsedSec);
	m_Boss->Update(elapsedSec);

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
}

void Game::Draw( ) const
{
	ClearBackground( );
	m_Boss->Draw(m_Grid1Ptr->GetRectAtPosition(m_Boss->GetGridPosition()));
	m_Grid1Ptr->Draw();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	if (m_Boss->GetHealth() <= 0) return;
	if (m_TurnInProgress) return;
	switch (e.keysym.sym)
	{
	case SDLK_RETURN:
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
		m_BossMove = { 0,-1};
		BossMove();
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	switch (e.button)
	{
	case SDL_BUTTON_LEFT:
		Point2f mousePos = Point2f{ float(e.x), float(e.y) };
		break;
	}
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.3f, 0.3f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::BossMove()
{
	if (not m_Boss->CheckMove(m_BossMove)) return;

	++m_TurnCounter;
	m_TurnInProgress = true;

	m_Boss->Move(m_BossMove);
	m_BossMove = {};

	m_TimerBetweenMove = { 0.0001f };
}

void Game::HeroMove()
{
	m_TimerBetweenMove = 0.f;
	m_Knight->Move();
	m_Mage->Move();
	m_Rogue->Move();
	m_TimerBetweenMoveAttack = { 0.0001f };
}

void Game::BossAttack()
{
	m_TimerBetweenMoveAttack = 0.f;
	m_Boss->Attack();
	m_TimerBetweenAttack = { 0.0001f };
}

void Game::HeroAttack()
{
	m_Boss->HitEnemies();
	m_TimerBetweenAttack = 0.f;
	m_Knight->Attack();
	m_Mage->Attack();
	m_Rogue->Attack();
	m_TimerBetweenAttack = { 0.f };
	m_TurnInProgress = false;
	std::cout << "Turn " << m_TurnCounter << "\n";
}
