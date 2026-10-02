#include "SpeedAlien.h"

SpeedAlien::SpeedAlien(int iD, float x, float y) : Alien(iD, x, y)
{
	m_enemyHp = SPEED_ALIEN_HP;
	m_alienSpeed = 300.0f;
	m_maxAlienSpeed = 500.0f;

	m_colour = RED;

	m_alien =
	{
		x,
		y,
		30.0f,
		30.0f,
	};
}

SpeedAlien::~SpeedAlien()
{

}
