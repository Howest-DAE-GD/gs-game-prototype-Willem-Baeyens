#include "pch.h"
#include "Boss.h"
#include "utils.h"
#include "Knight.h"

Boss::Boss(POINT gridPos,Knight* knightPtr, Grid* gridPtr):
	m_Speed{200},
	m_Grid_Position{gridPos},
	m_Health{10},
	m_HealthBarRect{250.f,475.f,350.f,20.f},
	m_StunCounter{0},
	m_KnightPtr{knightPtr},
	m_GridPtr{gridPtr}
{
	m_Rect = Rectf{ 600,200,100,100 };
	m_Color = Color4f{ 219/255.f, 99/255.f, 107/255.f,1.f };
}

void Boss::Draw(Rectf rect) const
{
	Creature::Draw(rect);

	utils::SetColor( Color4f{1,0,0,1});
	utils::FillRect(m_HealthBarRect);


	utils::SetColor(Color4f{ 0,0,0,0.8f });
	Point2f p1{ rect.left + rect.width * 0.175f,rect.bottom + rect.height * 0.65f};
	Point2f p2{ rect.left + rect.width * 0.175f,rect.bottom + rect.height * 0.80f};
	Point2f p3{ rect.left + rect.width * 0.425f,rect.bottom + rect.height * 0.65f};

	Point2f p4{ rect.left + rect.width * 0.575f,rect.bottom + rect.height * 0.65f};
	Point2f p5{ rect.left + rect.width * 0.825f,rect.bottom + rect.height * 0.65f};
	Point2f p6{ rect.left + rect.width * 0.825f,rect.bottom + rect.height * 0.80f};
	utils::FillTriangle(p1,p2,p3 );
	utils::FillTriangle(p4, p5, p6);


	utils::SetColor(Color4f{ 0,1,0,1 });
	Rectf currentHealthRect = m_HealthBarRect;
	currentHealthRect.width = m_HealthBarRect.width * m_Health / 10.f;
	utils::FillRect(currentHealthRect);
}

void Boss::Move(const Vector2f& change)
{
	m_Rect += change;
}

void Boss::Update(const float elapsedSec)
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	Vector2f change{};
	if (pStates[SDL_SCANCODE_DOWN])
	{
		change.y -= elapsedSec * m_Speed;
	}
	if (pStates[SDL_SCANCODE_UP])
	{
		change.y += elapsedSec * m_Speed;
	}
	Move(change);
}

Color4f Boss::GetColor() const
{
	return m_Color;
}

POINT Boss::GetGridPosition() const
{
	return m_Grid_Position;
}

void Boss::TakeDamage(int damage)
{
	m_Health -= damage;
}

void Boss::Move(POINT change)
{
	m_Grid_Position.x += change.x;
	m_Grid_Position.y += change.y;

	if (m_StunCounter < 2)
	{
		m_StunCounter++;
		if (m_StunCounter == 2)
		{
			m_Color = Color4f{ 135 / 255.f, 12 / 255.f, 20 / 255.f,1.f };
		}
	}
}

bool Boss::CheckMove(POINT change) const
{
	if (m_Grid_Position.y + change.y >= m_GridPtr->GetRowCount()) return false;
	if (m_Grid_Position.y + change.y < 0) return false;
	if (m_Grid_Position.x + change.x >= m_GridPtr->GetColumnCount()) return false;
	if (m_Grid_Position.x + change.x < 0) return false;

	return true;
}

void Boss::Stun()
{
	if (m_StunCounter != 2) return;
	m_KnightPtr->Stun();
	m_Color = Color4f{ 219 / 255.f, 99 / 255.f, 107 / 255.f,1.f };
	m_StunCounter = 0;
}

int Boss::GetStunCounter() const
{
	return m_StunCounter;
}

int Boss::GetHealth() const
{
	return m_Health;
}


