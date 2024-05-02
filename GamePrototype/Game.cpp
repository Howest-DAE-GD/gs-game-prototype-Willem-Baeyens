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

	m_BossGrid = Grid(5, 5, 60, Vector2f{ 500,120 });
	m_HeroGrid = Grid(5, 5, 60, Vector2f{ 100,120 });

	m_Mage = new Mage{ POINT{0,2},&m_BossGrid };
	m_Knight = new Knight{ POINT{4,2},&m_HeroGrid,&m_BossGrid};
	m_Boss = new Boss{ POINT{0,2},m_Knight,&m_BossGrid };

	m_BossGrid.AddCreature(m_Boss);

	m_HeroGrid.AddCreature(m_Mage);
	m_HeroGrid.AddCreature(m_Knight);
}

void Game::Cleanup( )
{
}

void Game::Update( float elapsedSec )
{
	// Check keyboard state
	m_Mage->Update(elapsedSec);
	m_Knight->Update(elapsedSec);
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
	m_BossGrid.Draw();
	m_HeroGrid.Draw();
	if (m_Stunning and m_Boss->GetStunCounter() == 2)
	{
		m_StunningText->Draw(Point2f{ 325.f,50.f });
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
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
	case SDLK_SPACE:
		m_Stunning = !m_Stunning;
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	switch (e.button)
	{
	case SDL_BUTTON_LEFT:
		Point2f mousePos = Point2f{ float(e.x), float(e.y) };
		break;
	}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
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
	if (m_Stunning)
	{
		m_Boss->Stun();
		m_Stunning = false;
	}
	m_BossMove = {};

	m_Between = true;
}