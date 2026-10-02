#include "raylib.h"
#include "raymath.h"
#include "Alien.h"
#include "Player.h"
#include "Constants.h"

Alien::Alien(int iD, float x, float y) :
	m_alien(),
	m_velocity(),
	m_knockbackVelocity(),
	m_oldPosition(),
	m_isActive(true),
	m_isColliding(false),
	m_isBulletHit(false),
	m_isIFrameActive(false),
	m_iD(iD),
	m_enemyHp(),
	m_alienSpeed(),
	m_maxAlienSpeed(),
	m_separtionRadius(),
	m_separationStrength(),
	m_colour(YELLOW)
{
	m_velocity = { 0.0f, 0.0f };
	m_alienSpeed = 1.0f;
	m_maxAlienSpeed = 1.5f;
	m_separtionRadius = 25.0f;
	m_separationStrength = 1.0f;
	m_enemyHp = 100;

	//----------------------------------------
	//RECTANGLES
	//----------------------------------------
	m_alien =
	{
		x,
		y,
		15.0f,
		15.0f
	};
}



//returns the seek calculation
Vector2 Alien::calculateSeekToPlayer(Vector2 target)
{
	if (m_isActive)
	{
		Vector2 position = { m_alien.x, m_alien.y };	//set position equal to aliens x and y pos
		Vector2 desired = Vector2Subtract(target, position);	//desired vector
		desired = Vector2Normalize(desired);	//normalize
		desired = Vector2Scale(desired, m_alienSpeed); //scale by aliens speed

		Vector2 steer = Vector2Subtract(desired, m_velocity);	//steering force

		return steer;
	}
}

Vector2 Alien::separationCalculation(const std::vector<std::unique_ptr<Alien>>& aliens)
{
	if (m_isActive)
	{
		Vector2 alienSeparationForce = { 0.0f, 0.0f };

		for (size_t i = 0; i < aliens.size(); i++)
		{
			//loop through aliens
			const Alien& other = *aliens[i];

			//check if Id is not equal 
			if (other.m_iD != m_iD)
			{
				//vector pointing AWAY from neighbour
				Vector2 offset = { m_alien.x - other.m_alien.x, m_alien.y - other.m_alien.y };

				//stores the distance between
				float distance = Vector2Length(offset);

				//checks to see if aliens are close enough to each other to begin separation
				if (distance < m_separtionRadius && distance > 0)
				{
					//normalize, get direction
					Vector2 pushDir = Vector2Normalize(offset);

					//aliens who are closer together will push away harder
					float strength = (m_separtionRadius - distance) / m_separtionRadius;

					//applying push force
					alienSeparationForce.x += pushDir.x * strength;
					alienSeparationForce.y += pushDir.y * strength;
				}

				//scale force
				alienSeparationForce = Vector2Scale(alienSeparationForce, m_separationStrength);
			}
		}
		return alienSeparationForce;
	}
}

void Alien::updateAlienPos(Vector2 playerPos, const std::vector<std::unique_ptr<Alien>>& aliens)
{
	if (!m_isActive)
		return;


	Vector2 seekToPlayerForce = calculateSeekToPlayer(playerPos);	//getting seek calculation value
	Vector2 separateAliensForce = separationCalculation(aliens);	//getting separation calculation value
	Vector2 combinedForces = Vector2Add(seekToPlayerForce, separateAliensForce);		//combining forces

	m_velocity = Vector2Add(m_velocity, combinedForces);	//apply forces

	if (Vector2Length(m_velocity) > m_maxAlienSpeed)	//limiting speed
	{
		m_velocity = Vector2Normalize(m_velocity);
		m_velocity = Vector2Scale(m_velocity, m_maxAlienSpeed);
	}


	m_knockbackVelocity = Vector2Scale(m_knockbackVelocity, KNOCKBACK_VELOCITY_DECAY);

	//Vector2 finalVelocity = Vector2Add(m_velocity, m_knockbackVelocity);

	if (!m_knockback.isTimerDone(&m_knockback))
	{
		//move aliens if knocked back
		m_alien.x += m_knockbackVelocity.x * GetFrameTime();
		m_alien.y += m_knockbackVelocity.y * GetFrameTime();
	}
	else if(m_knockback.isTimerDone(&m_knockback))
	{
		//move aliens 
		m_alien.x += m_velocity.x * GetFrameTime();
		m_alien.y += m_velocity.y * GetFrameTime();
	}
}

void Alien::startKnockbackTimer(float knockbackDuration)
{
	m_knockback.startTimer(&m_knockback, knockbackDuration);
}

void Alien::updateTimers()
{
	m_knockback.updateTimer(&m_knockback);
}

bool Alien::isActive()
{
	return m_isActive;
}

bool Alien::isIFramesActive()
{
	return m_isIFrameActive;
}

void Alien::activate(Vector2 position)
{
	m_isActive = true;
	m_alien.x = position.x;
	m_alien.y = position.y;
}

void Alien::activateIFrames()
{
	m_isIFrameActive = true;
	m_iFrameTimer.startTimer(&m_iFrameTimer, 0.05f);
}

void Alien::deactivateIFrames()
{
	m_isIFrameActive = false;
}

void Alien::updatePersonalIFrameTimer()
{
	if (m_isIFrameActive)
	{
		m_iFrameTimer.updateTimer(&m_iFrameTimer);
		if (m_iFrameTimer.isTimerDone(&m_iFrameTimer))
		{
			m_isIFrameActive = false;
		}
	}
}

void Alien::bulletHit()
{
	m_isBulletHit = true;
}

void Alien::bulletHitDeactivate()
{
	m_isBulletHit = false;
}

bool Alien::isBulletHit()
{
	return m_isBulletHit;
}

void Alien::deactivate()
{
	m_isActive = false;
}

Rectangle Alien::getAlien()
{
	return m_alien;
}

Vector2 Alien::getAlienPos()
{
	return { m_alien.x, m_alien.y };
}

void Alien::setKnockbackVelocity(Vector2 knockbackForce)
{
	m_knockbackVelocity = knockbackForce;

}

void Alien::setPosition(float posX, float posY)
{
	m_alien.x = posX;
	m_alien.y = posY;
}

Vector2 Alien::getVelocity()
{
	return m_velocity;
}

bool Alien::isColliding()
{
	return m_isColliding;
}

void Alien::colliding()
{
	m_isColliding = true;
}

void Alien::notColliding()
{
	m_isColliding = false;
}

void Alien::damageEnemy(int damage)
{
	if (m_isIFrameActive)
		return;
	m_enemyHp -= damage;
	activateIFrames();
	if (m_enemyHp <= 0)
	{
		m_enemyHp = 0;
		deactivate();
	}
}

void Alien::iDIncrease()
{
	m_iD += 1;
}

int Alien::getId()
{
	return m_iD;
}

void Alien::draw()
{
	Color colour = m_colour;

	if (m_isActive)
	{
		if (m_isIFrameActive)
		{
			colour = PURPLE;
		}
		//alien box
		DrawRectangleRec(m_alien, colour);
	}

}
