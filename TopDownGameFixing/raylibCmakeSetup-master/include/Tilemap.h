#pragma once
#include "raylib.h"
#include "raymath.h"
#include "raytmx.h"
#include "Constants.h"
#include "PlayerCamera.h"
#include "Collision.h"

#include <cstring>

class Tilemap
{
public:
	Tilemap();
	~Tilemap();

	void loadMap();
	void unloadMap();

	void checkForWallCollision(Rectangle playerHitbox);

	TmxMap* getMap();

	void drawMap(Camera2D& playerCam);
private:
	TmxMap* m_testMap;
	Collision m_collision;
};