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

	//m_BossGrid = Grid(5, 5, 60, Vector2f{ 480,120 });
	//m_HeroGrid = Grid(5, 5, 60, Vector2f{ 80,120 });

	m_Grid1Ptr = new Grid(9, 5, 60, Vector2f{ 145, 120 });

	m_Mage = new Mage{ POINT{1,3},m_Grid1Ptr };
	m_Knight = new Knight{ POINT{4,2},m_Grid1Ptr};
	m_Boss = new Boss{ POINT{5,2},m_Knight,m_Grid1Ptr};

	//m_BossGrid.AddCreature(m_Boss);
	//
	//m_HeroGrid.AddCreature(m_Mage);
	//m_HeroGrid.AddCreature(m_Knight);

	m_Grid1Ptr->AddCreature(m_Mage);
	m_Grid1Ptr->AddCreature(m_Knight);
	m_Grid1Ptr->AddCreature(m_Boss);

}

void Game::Cleanup( )
{
}

void Game::Update( float elapsedSec )
{
	m_Mage->Update(elapsedSec);
	m_Knight->Update(elapsedSec);
	m_BossDoneAttacking = m_Boss->Update(elapsedSec);
	if (m_Between)
	{
		m_Timer += elapsedSec;
	}
	if (m_Timer > m_TimeBetweenHeroBoss)
	{
		NextTurnHero();
	}
}

void Game::Draw( ) const
{
	ClearBackground( );
	//m_BossGrid.Draw();
	//m_HeroGrid.Draw();
	m_Grid1Ptr->Draw();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	if (m_Boss->GetHealth() <= 0) return;
	if (not m_BossDoneAttacking) return;
	if (m_Between) return;
	switch (e.keysym.sym)
	{
	case SDLK_RETURN:
		NextTurnBoss();
		break;
	case SDLK_LEFT:
		m_BossMove = { -1,0 };
		NextTurnBoss();
		break;
	case SDLK_RIGHT:
		m_BossMove = { 1,0 };
		NextTurnBoss();
		break;
	case SDLK_UP:
		m_BossMove = { 0,1 };
		NextTurnBoss();
		break;
	case SDLK_DOWN:
		m_BossMove = { 0,-1};
		NextTurnBoss();
		break;
	//case SDLK_SPACE:
	//	m_Stunning = !m_Stunning;
	//	break;
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

void Game::NextTurnHero()
{
	m_Knight->DoTurn();
	m_Mage->DoTurn();
	m_Between = false;
	m_Timer = 0;

	std::cout << m_TurnCounter << "\n";

}
void Game::NextTurnBoss()
{
	if (not m_Boss->CheckMove(m_BossMove)) return;

	++m_TurnCounter;

	m_Mage->SetLastBossPosition(m_Boss->GetGridPosition());

	m_Boss->Move(m_BossMove);
	//if (m_Stunning)
	//{
	//	m_Boss->Stun();
	//	m_Stunning = false;
	//}
	m_BossMove = {};

	m_Between = true;
}