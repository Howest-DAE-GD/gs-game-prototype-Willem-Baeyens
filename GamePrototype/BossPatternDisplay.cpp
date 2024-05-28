#include "pch.h"
#include "BossPatternDisplay.h"
#include "utils.h"

BossPatternDisplay::BossPatternDisplay(Point2f pos, const std::vector<BossMove>& pattern) :
	m_Position{ pos },
	m_Pattern{ pattern },
	m_NextMoveIndex{ 0 }
{
	std::string fontPath = "DIN-Light.otf";
	int fontSize = 35;
	Color4f color{ 32 / 255.f, 77 / 255.f, 212 / 255.f,1.f };
	m_Letters.push_back(new Texture("\\",fontPath,fontSize,color));
	m_Letters.push_back(new Texture("B", fontPath, fontSize, color));
	m_Letters.push_back(new Texture("E", fontPath, fontSize, color));
}

void BossPatternDisplay::Draw() const
{
	for (int index{}; index < m_Pattern.size(); ++index)
	{
		utils::SetColor(Color4f{ 0,0,0,1.f });
		if (index == abs(m_NextMoveIndex - 3)) utils::SetColor(Color4f{ 135 / 255.f, 12 / 255.f, 20 / 255.f,1.f });

		utils::FillRect(m_Position.x, m_Position.y + 110 * index, 60, 60);

		
		Texture* letter;
		float xOffset = 17;
		switch (m_Pattern[abs(index-3)])
		{
		case BossMove::null:
			letter = m_Letters[0];
			xOffset += 6;
			break;
		case BossMove::beamAttack:
			letter = m_Letters[1];
			break;
		case BossMove::surroundingAttack:
			letter = m_Letters[2];
			break;
		default:
			letter = m_Letters[0];
			break;
		}
	
		letter->Draw(Point2f{ m_Position.x + xOffset, m_Position.y  +10 + 110 * index });

		if (index == abs(m_NextMoveIndex - 3))
		{
			utils::SetColor(Color4f{ 135 / 255.f, 12 / 255.f, 20 / 255.f,1.f });
			Point2f p1{ m_Position.x - 25.f,m_Position.y + 22 + 110 * index };
			Point2f p2{ m_Position.x - 10.f,m_Position.y + 32 + 110 * index };
			Point2f p3{ m_Position.x - 25.f,m_Position.y + 42 + 110 * index };

			utils::FillTriangle(p1, p2, p3);
			utils::SetColor(Color4f{ 0,0,0,1.f });
		}
	}
	utils::SetColor(Color4f{ 135 / 255.f, 12 / 255.f, 20 / 255.f,1.f });
	
	Point2f p1{ m_Position.x + 15.f, m_Position.y + -17.5f };
	Point2f p2{ m_Position.x + 45.f, m_Position.y + -17.5f };
	Point2f p3{ m_Position.x + 30.f, m_Position.y + -32.5f };
	for (int index{}; index < m_Pattern.size() - 1; ++index)
	{
		p1.y += 110;
		p2.y += 110;
		p3.y += 110;
		utils::FillTriangle(p1, p2, p3);
	}
}

void BossPatternDisplay::Turn()
{
	++m_NextMoveIndex %= m_Pattern.size();
}
