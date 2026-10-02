#include "Larvae.h"

Larvae::Larvae(int iD, float x, float y) : Alien(iD, x, y) 
{
	m_enemyHp = LARVAE_HP;
	m_alienSpeed = 65.0f;
	m_maxAlienSpeed = 65.0f;

	m_colour = YELLOW;
}

Larvae::~Larvae()
{

}

