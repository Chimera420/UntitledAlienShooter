#include "BossAlien.h"

BossAlien::BossAlien(int iD, float x, float y) : Alien(iD, x, y)
{
	m_enemyHp = BOSS_ALIEN_HP;
	m_alienSpeed = 0.4f;
	m_maxAlienSpeed = 0.6f;

	m_colour = BROWN;

	m_alien =
	{
		x,
		y,
		50.0f,
		50.0f
	};
}

BossAlien::~BossAlien()
{

}
