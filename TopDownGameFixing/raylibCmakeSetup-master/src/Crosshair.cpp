#include "Crosshair.h"

Crosshair::Crosshair() :
	m_crosshairPNG(),
	m_crosshair(),
	m_crosshairDirection(),
	m_playerDirection(),
	m_playerAngle()
{
	loadCrosshair();
}

Crosshair::~Crosshair()
{
	unloadCrosshair();
}

void Crosshair::loadCrosshair()
{
	m_crosshairPNG = LoadImage("resources/crosshair.png");
	m_crosshair = LoadTextureFromImage(m_crosshairPNG);
}

void Crosshair::unloadCrosshair()
{
	UnloadImage(m_crosshairPNG);
}

float Crosshair::getPlayerAngle()
{
	return m_playerAngle;
}

void Crosshair::updatePlayerAim(Vector2 mouseWorldPos, Vector2 playerPos)
{
	m_playerDirection = Vector2Subtract(mouseWorldPos, playerPos);
	m_playerAngle = (float)atan2(m_playerDirection.y, m_playerDirection.x) * RAD2DEG;
}

void Crosshair::drawCrosshair()
{
	DrawTexture(m_crosshair, GetMouseX() - 10.0f, GetMouseY() - 10.0f, WHITE);
}
