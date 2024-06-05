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
	m_Grid1Ptr	= new Grid(9, 5, 60, Vector2f{ 145, 120 });

	m_Mage		= new Mage{ POINT{1,3},m_Grid1Ptr };
	m_Knight	= new Knight{ POINT{4,2},m_Grid1Ptr};
	m_Boss		= new Boss{ POINT{6,2},m_Knight,m_Grid1Ptr};
	m_Rogue		= new Rogue{ POINT{3,0},m_Grid1Ptr,m_Boss };
	m_Archer	= new Archer{ POINT{0,4},m_Grid1Ptr,m_Boss };

	m_Grid1Ptr->AddBoss(m_Boss);
	m_Grid1Ptr->AddCreature(m_Knight);
	m_Grid1Ptr->AddCreature(m_Rogue);
	m_Grid1Ptr->AddCreature(m_Archer);
	m_Grid1Ptr->AddCreature(m_Mage);

	m_BossPatternPtr = new BossPatternDisplay{ Point2f{ 760,60 },m_Boss->GetPattern() };
	m_Grid1Ptr->SetPattern(m_BossPatternPtr);
	m_Grid1Ptr->CreateHeroOrder();
}

void Game::Cleanup( )
{
}

void Game::Update( float elapsedSec )
{
	m_Grid1Ptr->Update(elapsedSec);
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

void Game::BossMove()
{
	if (not m_Boss->CheckMove(m_BossMove)) return;

	++m_TurnCounter;
	m_TurnInProgress = true;
	m_AllFinished = false;

	m_Boss->Move(m_BossMove);
	m_BossMove = {};

	m_TimerBetweenMove = { 0.0001f };
}

void Game::HeroMove()
{
	m_TimerBetweenMove = 0.f;
	m_Mage->Move();
	m_Knight->Move();
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
	m_Archer->Attack();
	m_Rogue->Attack();
	m_TimerBetweenAttack = { 0.f };
	m_TurnInProgress = false;
	std::cout << "Turn " << m_TurnCounter << "\n";
}

void Game::CheckIfAllFinished()
{
	bool allFinished = true;
	if (not m_Knight->TurnDone()) allFinished = false;
	if (not m_Rogue->TurnDone()) allFinished = false;
	if (not m_Boss->TurnDone()) allFinished = false;
	if (not m_Mage->TurnDone()) allFinished = false;
	if (not m_Archer->TurnDone()) allFinished = false;
	m_AllFinished = allFinished;
}
