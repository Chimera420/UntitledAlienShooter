#include "PlayerCamera.h"

PlayerCamera::PlayerCamera(Vector2 playerPosition) :
	m_playerCamera(),
	m_cameraOffset(),
	m_cameraTarget(playerPosition),
	m_cameraVelocity(),
	m_cameraAcceleration(),
	m_cameraDirection(),
	m_acceleration(),
	m_friction(),
	m_cameraSpeed(),
	m_rotation(),
	m_zoom()
{
	m_cameraOffset = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
	m_cameraSpeed = PLAYER_WALK_SPEED;
	m_acceleration = 10.0f;
	m_friction = 1.0f;
	m_rotation = 0.0f;
	m_zoom = 3.0f;

	m_playerCamera =
	{
		m_cameraOffset,
		m_cameraTarget,
		m_rotation,
		m_zoom
	};
}

PlayerCamera::~PlayerCamera()
{

}

void PlayerCamera::updateCameraPosition(Vector2 playerPos)
{
	//make sure to get mouse world position
	Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), m_playerCamera);

	//midpoint between player and mouse
	Vector2 midpoint = { (playerPos.x + mouseWorldPos.x) / 2, (playerPos.y + mouseWorldPos.y) / 2 };

	//sets the camera's origin to the middle of screen
	m_playerCamera.offset = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };

	//set the camera's target to the midpoint of the player and the crosshair
	m_playerCamera.target = midpoint;
}

void PlayerCamera::setCameraSpeed(float speed)
{
	m_cameraSpeed = speed;
}

//TODO: WRITE A CLAMPING FUNCTION WHEN I HAVE A PLAYFIELD DRAWN
void PlayerCamera::clampToEdges()
{
	//i.e
	//take camera.target.x and clamp the x-value to the extent of the playfields walls
	//do the same for camera.target.y
}

Camera2D PlayerCamera::getPlayerCamera()
{
	return m_playerCamera;
}
