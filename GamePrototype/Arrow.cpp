#include "pch.h"
#include "Arrow.h"
#include <cmath>

Arrow::Arrow(Point2f start, Point2f end):
	m_BaseHeight{end.y},
	m_BaseX{start.x},
	m_AngularSpeed{(44/7.f)/(end.x - start.x)/2},
	m_Speed{700.f},
	m_Position{start},
	m_Timer{0.f}
{
}

void Arrow::Update(float elapsedSec)
{
	m_Timer += elapsedSec;
	m_Position.x += elapsedSec * m_Speed;
	m_Position.y = 100*sin(m_AngularSpeed * (m_Position.x - m_BaseX)) + m_BaseHeight;
}

void Arrow::Draw() const
{
	utils::SetColor(Color4f{ 147 / 255.f, 190 / 255.f, 219 / 255.f,1.f });
	utils::FillRect(m_Position, 40, 8);
}

bool Arrow::IsDone() const
{
	return m_Position.y < m_BaseHeight;
}
