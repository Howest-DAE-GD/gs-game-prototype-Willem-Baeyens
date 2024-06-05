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

	m_Grid1Ptr = new Grid(9, 5, 60, Vector2f{ 145,120 });

	m_Knight2 = new Knight{ POINT{4,1},m_Grid1Ptr,10 };
	m_Mage2 = new Mage{ POINT{1,1},m_Grid1Ptr,2 };
	m_Boss2 = new Boss{ POINT{7,2},m_Knight2,m_Grid1Ptr,4 };

	m_Grid1Ptr->AddBoss(m_Boss2);
	m_Grid1Ptr->AddCreature(m_Knight2);
	m_Grid1Ptr->AddCreature(m_Mage2);

	m_BossPatternPtr = new BossPatternDisplay{ Point2f{ 760,60 },m_Boss2->GetPattern() };
	m_Grid1Ptr->SetPattern(m_BossPatternPtr);
	m_Grid1Ptr->CreateHeroOrder();
}

void Game::Cleanup( )
{
}

void Game::Update(float elapsedSec)
{
	m_Grid1Ptr->Update(elapsedSec);
	if (m_Grid1Ptr->IsLevelDone())
	{
		++m_Level;
		switch (m_Level)
		{
		case 2:
			m_Grid1Ptr = new Grid(9, 5, 60, Vector2f{ 145, 120 });

			m_Mage1 = new Mage{ POINT{1,3},m_Grid1Ptr,2 };
			m_Knight1 = new Knight{ POINT{4,2},m_Grid1Ptr,10 };
			m_Boss1 = new Boss{ POINT{6,2},m_Knight1,m_Grid1Ptr,4 };
			m_Rogue1 = new Rogue{ POINT{3,0},m_Grid1Ptr,m_Boss1,7 };
			m_Archer1 = new Archer{ POINT{0,4},m_Grid1Ptr,m_Boss1,3 };

			m_Grid1Ptr->AddBoss(m_Boss1);
			m_Grid1Ptr->AddCreature(m_Knight1);
			m_Grid1Ptr->AddCreature(m_Rogue1);
			m_Grid1Ptr->AddCreature(m_Archer1);
			m_Grid1Ptr->AddCreature(m_Mage1);

			m_BossPatternPtr = new BossPatternDisplay{ Point2f{ 760,60 },m_Boss1->GetPattern() };

			m_Grid1Ptr->SetPattern(m_BossPatternPtr);
			m_Grid1Ptr->CreateHeroOrder();
			break;
		}
	}
}

void Game::Draw( ) const
{
	ClearBackground( );
	m_Grid1Ptr->Draw();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	m_Grid1Ptr->ProcessKeyUpEvent(e);
	switch (e.keysym.sym)
	{
	case SDLK_r:
		Initialize();
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	m_Grid1Ptr->Click(e);
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.3f, 0.3f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
