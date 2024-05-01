#include "pch.h"
#include "Fireball.h"
#include "utils.h"

Fireball::Fireball(POINT gridPos,Grid* grid):
	m_GridPosition(gridPos),
	m_GridPtr{grid},
	m_ExplosionDuration{ .5f },
	m_ExplostionTimer{0.f},
	m_Exploding{false}
{
}

void Fireball::Draw() const
{
	utils::SetColor(Color4f{ 1.f,0.165f, 0.01f,1 });
	int squareSize = m_GridPtr->GetSquareSize();
	Vector2f gridLocation = m_GridPtr->GetLocation();
	Rectf targetRect = Rectf{ gridLocation.x + m_GridPosition.x * squareSize
								,gridLocation.y + m_GridPosition.y * squareSize
								,float(squareSize),float(squareSize) };
	if (not m_Exploding)
	{
		utils::DrawRect(targetRect, 5.f);
	}


	if (m_Exploding)
	{
		Point2f center{ targetRect.left + targetRect.width / 2,targetRect.bottom + targetRect.height / 2 };
		Ellipsef explosionCircle{ center,squareSize / 1.5f,squareSize / 1.5f };
		utils::FillEllipse(explosionCircle);
	}
}

void Fireball::Update(float elapsedSec)
{
	if (m_Exploding)
	{
		m_ExplostionTimer += elapsedSec;
		if (m_ExplostionTimer > m_ExplosionDuration)
		{
			m_Exploding = false;
			m_ExplostionTimer = 0.f;
			m_GridPosition.x = -100;
		}
	}
}

void Fireball::Explode() 
{
	m_GridPtr->DoDamage(m_GridPosition, 2);
	m_Exploding = true;
}
