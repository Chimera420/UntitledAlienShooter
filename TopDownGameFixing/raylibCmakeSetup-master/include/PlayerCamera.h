#pragma once
#include "raylib.h"
#include "raymath.h"
#include "Constants.h"

class PlayerCamera
{
public:
	PlayerCamera(Vector2 playerPosition);
	~PlayerCamera();

	void updateCameraPosition(Vector2 playerPos);

	void setCameraSpeed(float speed);

	//TODO: FINISH DEFINITION
	void clampToEdges();

	Camera2D getPlayerCamera();
private:
	Camera2D m_playerCamera;

	Vector2 m_cameraOffset;
	Vector2 m_cameraTarget;

	//camera movement
	Vector2 m_cameraVelocity;
	Vector2 m_cameraAcceleration;
	Vector2 m_cameraDirection;

	float m_acceleration;
	float m_friction;
	float m_cameraSpeed;
	float m_rotation;
	float m_zoom;
};