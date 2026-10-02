#pragma once
#include "raylib.h"
#include "Constants.h"
#include "Timer.h"
#include <vector>
#include <memory>

class Alien
{
public:
	//TODO: MAKE ALIEN CONSTRUCTOR TAKE ALIEN STATS AS WELL
	Alien(int iD, float x, float y);
	virtual ~Alien() = default;

	//enemy movement
	Vector2 calculateSeekToPlayer(Vector2 target);
	Vector2 separationCalculation(const std::vector<std::unique_ptr<Alien>>& aliens);
	virtual void updateAlienPos(Vector2 playerPos, const std::vector<std::unique_ptr<Alien>>& aliens);
	
	//timers
	void updateTimers();
	void startKnockbackTimer(float knockbackDuration);

	//iFrames
	bool isIFramesActive();
	void activateIFrames();
	void deactivateIFrames();
	void updatePersonalIFrameTimer();

	//active state
	bool isActive();
	void activate(Vector2 position);
	void deactivate();
	void bulletHit();
	void bulletHitDeactivate();
	bool isBulletHit();

	//alien pos
	void setPosition(float posX, float posY);
	void setFuturePos(float posX, float posY);

	//getters
	Vector2 getVelocity();
	Rectangle getAlien();
	Vector2 getAlienPos();

	//settters
	void setKnockbackVelocity(Vector2 knockbackForce);

	//collision
	bool isColliding();
	void colliding();
	void notColliding();
	void damageEnemy(int damage);

	//identification
	void iDIncrease();
	int getId();

	//draw
	void draw();
protected:
	//timer for each alien
	Timer m_iFrameTimer;
	Timer m_knockback;



	//recs
	Rectangle m_alien;

	//vectors
	Vector2 m_velocity;
	Vector2 m_knockbackVelocity;
	Vector2 m_oldPosition;

	//bools
	bool m_isActive;
	bool m_isColliding;
	bool m_isBulletHit;
	bool m_isIFrameActive;

	//ints
	int m_iD;
	int m_enemyHp;

	//floats
	float m_alienSpeed;
	float m_maxAlienSpeed;
	float m_separtionRadius;
	float m_separationStrength;

	Color m_colour;
};