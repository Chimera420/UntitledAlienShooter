#include "Collision.h"

Collision::Collision() :
	m_isCollidingWithWall(false)
{

}

Collision::~Collision()
{

}

void Collision::updateCollisions(TmxObjectGroup collisionGroup, Rectangle playerHitbox, TmxObject& outputObject)
{
	checkMapWallCollision(collisionGroup, playerHitbox, outputObject);
}

void Collision::checkMapWallCollision(TmxObjectGroup collisionGroup, Rectangle playerHitbox, TmxObject& outputObject)
{
		//m_isCollidingWithWall = CheckCollisionTMXObjectGroupRec(collisionGroup, playerHitbox, &outputObject);
	if (CheckCollisionTMXObjectGroupRec(collisionGroup, playerHitbox, &outputObject))
	{
		DrawText("WALL COLLISION!", 10, 340, 20, WHITE);
	}
	else
	{
		DrawText("NO WALL COLLISION!", 10, 340, 20, WHITE);
	}
}

bool Collision::isCollidingWithWall()
{
	return m_isCollidingWithWall;
}
