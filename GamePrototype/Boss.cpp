#include "pch.h"
#include "Boss.h"
#include "utils.h"
#include "Knight.h"

Boss::Boss(POINT gridPos,Knight* knightPtr, Grid* gridPtr):
	m_Speed{200},
	m_GridPosition{gridPos},
	m_Health{10},
	m_HealthBarRect{250.f,475.f,350.f,20.f},
	//m_StunCounter{0},
	m_KnightPtr{knightPtr},
	m_GridPtr{gridPtr},
	m_AttackDuration{1.f},
	m_AttackTimer{0.f},
	m_Attacking{false},
	m_DidDamage{false},
	m_AttackLocations{},
	m_AttackPatternVect{BossMove::beamAttack,BossMove::beamAttack,BossMove::null,BossMove::columnAttack},
	m_AttackIndex{}
{	
	m_Rect = Rectf{ 600,200,100,100 };
	m_Color = Color4f{ 135 / 255.f, 12 / 255.f, 20 / 255.f,1.f };
}

void Boss::Draw(Rectf rect) const
{
	Creature::Draw(rect);

	utils::SetColor(Color4f{ 0,0,0,0.8f });
	Point2f p1{ rect.left + rect.width * 0.175f,rect.bottom + rect.height * 0.65f};
	Point2f p2{ rect.left + rect.width * 0.175f,rect.bottom + rect.height * 0.80f};
	Point2f p3{ rect.left + rect.width * 0.425f,rect.bottom + rect.height * 0.65f};

	Point2f p4{ rect.left + rect.width * 0.575f,rect.bottom + rect.height * 0.65f};
	Point2f p5{ rect.left + rect.width * 0.825f,rect.bottom + rect.height * 0.65f};
	Point2f p6{ rect.left + rect.width * 0.825f,rect.bottom + rect.height * 0.80f};
	utils::FillTriangle(p1,p2,p3 );
	utils::FillTriangle(p4, p5, p6);

	utils::SetColor(Color4f{ 1,0,0,1 });
	utils::FillRect(m_HealthBarRect);

	utils::SetColor(Color4f{ 0,1,0,1 });
	Rectf currentHealthRect = m_HealthBarRect;
	currentHealthRect.width = m_HealthBarRect.width * m_Health / 10.f;
	utils::FillRect(currentHealthRect);

	if (m_AttackTimer >= 0.7f)
	{
		DrawAttack();
	}
}

void Boss::DrawAttack() const
{
	utils::SetColor(Color4f{10/255.f,0,0,1});

	for (int index = 0; index < m_AttackLocations.size(); ++index)
	{
		Rectf rect = m_GridPtr->GetRectAtPosition(m_AttackLocations[index]);
		rect.left += rect.width * 0.2f;
		rect.bottom += rect.height * 0.2f;
		rect.width = rect.width * 0.6f;
		rect.height = rect.height * 0.6f;
		utils::FillRect(rect);
	}
}

void Boss::Move(const Vector2f& change)
{
	m_Rect += change;
}

bool Boss::Update(const float elapsedSec)
{
	if (m_Attacking)
	{
		if (not m_DidDamage and m_AttackTimer >= 0.7f)
		{
			HitEnemies();
			m_DidDamage = true;
		}
		m_AttackTimer += elapsedSec;
		if (m_AttackTimer > m_AttackDuration)
		{
			m_AttackTimer = 0.f;
			m_Attacking = false;
			m_AttackLocations.clear();
			return true;
			//test comment
		}
		return false;
	}
	return true;
}

Color4f Boss::GetColor() const
{
	return m_Color;
}

POINT Boss::GetGridPosition() const
{
	return m_GridPosition;
}

void Boss::TakeDamage(int damage)
{
	m_Health -= damage;
}

void Boss::Move(POINT change)
{
	m_GridPosition.x += change.x;
	m_GridPosition.y += change.y;
	Attack();
}

void Boss::RowAttack()
{
	for (int index = 0; index < m_GridPosition.x; ++index)
	{
		POINT location{ index,m_GridPosition.y };
		m_AttackLocations.push_back(location);
	}
	m_DidDamage = false;
}

void Boss::ColumAttack()
{
	for (int index = 0; index < m_GridPtr->GetRowCount(); ++index)
	{
		POINT location{ m_GridPosition.x - 1,index };
		m_AttackLocations.push_back(location);
	}
	m_DidDamage = false;
}

void Boss::CometAttack()
{
	for (int rowIndex{ 1 }; rowIndex < 4; ++rowIndex)
	{
		for (int columnIndex{ 1 }; columnIndex < 4; ++columnIndex)
		{
			POINT location{ columnIndex,rowIndex };
			m_AttackLocations.push_back(location);
		}
	}
	m_DidDamage = false;
}

void Boss::SurroundingAttack()
{
	for (int rowIndex{ m_GridPosition.y - 1 }; rowIndex <= m_GridPosition.y + 1; ++rowIndex)
	{
		for (int columnIndex{ m_GridPosition.x - 1 }; columnIndex <= m_GridPosition.x + 1; ++columnIndex)
		{
			POINT location{ columnIndex,rowIndex };
			if (location == m_GridPosition) continue;
			m_AttackLocations.push_back(location);
		}
	}
	m_DidDamage = false;
}

void Boss::HitEnemies()
{
	for (int index = 0; index < m_AttackLocations.size(); ++index)
	{
		m_GridPtr->DoDamage(m_AttackLocations[index], 2);
	}
}

bool Boss::CheckMove(POINT change) const
{
	int newX{ m_GridPosition.x + change.x };
	int newY{ m_GridPosition.y + change.y };
	if (newY >= m_GridPtr->GetRowCount()) return false;
	if (newY < 0) return false;
	if (newX >= m_GridPtr->GetColumnCount()) return false;
	if (newX < 0) return false;

	if (m_GridPtr->GetCreatureAtPosition(POINT{newX,newY})) return false;

	return true;
}

//void Boss::Stun()
//{
//	if (m_StunCounter != 2) return;
//	m_KnightPtr->Stun();
//	m_Color = Color4f{ 219 / 255.f, 99 / 255.f, 107 / 255.f,1.f };
//	m_StunCounter = 0;
//}
//
//int Boss::GetStunCounter() const
//{
//	return m_StunCounter;
//}

int Boss::GetHealth() const
{
	return m_Health;
}

void Boss::Attack()
{
	switch (m_AttackPatternVect[m_AttackIndex])
	{
	case BossMove::null:
		break;
	case BossMove::columnAttack:
		ColumAttack();
		m_Attacking = true;
		break;
	case BossMove::beamAttack:
		RowAttack();
		m_Attacking = true;
		break;
	case BossMove::cometAttack:
		CometAttack();
		m_Attacking = true;
		break;
	case BossMove::surroundingAttack:
		SurroundingAttack();
		m_Attacking = true;
		break;
	}
	IncrementAttackIndex();
}

void Boss::IncrementAttackIndex()
{
	if (++m_AttackIndex >= m_AttackPatternVect.size())
	{
		m_AttackIndex = 0;
	}
}


