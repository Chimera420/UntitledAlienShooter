#pragma once
#include "raylib.h"
#include "raymath.h"

class Crosshair
{
public:
	Crosshair();
	~Crosshair();

	void loadCrosshair();
	void unloadCrosshair();

float getPlayerAngle();

	void updatePlayerAim(Vector2 mouseWorldPos, Vector2 playerPos);

	void drawCrosshair();
private:
	//crosshair PNG
	Image m_crosshairPNG;
	Texture2D m_crosshair;

	Vector2 m_crosshairDirection;

	Vector2 m_playerDirection;
	float m_playerAngle;
};