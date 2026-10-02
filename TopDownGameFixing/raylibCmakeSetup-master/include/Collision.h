#pragma once
#include "raylib.h"
#include "raymath.h"
#include "raytmx.h"
#include "Player.h"



class Collision
{
public:
	Collision();
	~Collision();

	void updateCollisions(TmxObjectGroup collisionGroup, Rectangle playerHitbox, TmxObject& outputObject);
	void checkMapWallCollision(TmxObjectGroup collisionGroup, Rectangle playerHitbox, TmxObject& outputObject);

	bool isCollidingWithWall();

private:
	bool m_isCollidingWithWall;
};