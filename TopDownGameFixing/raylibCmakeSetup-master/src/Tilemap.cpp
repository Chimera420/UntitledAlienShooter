#include "Tilemap.h"


Tilemap::Tilemap() :
	m_testMap(nullptr),
	m_collision()
{
	loadMap();
}

Tilemap::~Tilemap()
{
	unloadMap();
}

void Tilemap::loadMap()
{
	//m_testMap = LoadTMX("assets/tilemap/DemoMapPrototype.tmx");
	m_testMap = LoadTMX("resources/DemoMapPrototype.tmx");

	if (m_testMap == nullptr)
	{
		TraceLog(LOG_ERROR, "PROTOTYPE MAP FAILED TO LOAD!");
	}
	else
	{
		TraceLog(LOG_INFO, "PROTOTYPE MAP LOADED SUCCESSFULLY!");
	}
}

void Tilemap::unloadMap()
{
	if (m_testMap != nullptr)
	{
		UnloadTMX(m_testMap);
		m_testMap = nullptr;
	}
}

void Tilemap::checkForWallCollision(Rectangle playerHitbox)
{
	//stores the output data
	TmxObject objectOutput = {};

	//safety check. If collision layer in tiled gets moved, this will check where it is.
	for (uint32_t i = 0; i < m_testMap->layersLength; i++)
	{
		//layer for collision object
		TmxLayer& collisionLayer = m_testMap->layers[i];

		if (strcmp(collisionLayer.name, "Collisions") == 0 && collisionLayer.type == LAYER_TYPE_OBJECT_GROUP)
		{
			m_collision.checkMapWallCollision(collisionLayer.exact.objectGroup, playerHitbox, objectOutput);
			break;
		}
	}
}

TmxMap* Tilemap::getMap()
{
	return m_testMap;
}

void Tilemap::drawMap(Camera2D& playerCam)
{
	if (m_testMap != nullptr)
	{
		DrawTMX(m_testMap, &playerCam, nullptr, 0, 0, WHITE);
	}

}
