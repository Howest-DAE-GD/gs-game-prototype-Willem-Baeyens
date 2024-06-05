#pragma once
#include "utils.h"
class Arrow
{
public:
	Arrow(Point2f start, Point2f end);

	void Update(float elapsedSec);

	void Draw() const;

	bool IsDone() const;
private:
	Point2f m_Position;
	float   m_Speed;
	float   m_AngularSpeed;
	float   m_BaseHeight;
	float   m_BaseX;
	float   m_Timer;
};

