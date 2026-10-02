#include "Player.h"
#include "Constants.h"
#include "raylib.h"
#include "raymath.h"

using namespace std::chrono_literals;

//----------------------------------------
//CONSTRUCTOR
//----------------------------------------
Player::Player() :
	m_currentState(PlayerState::CAN_MOVE),
	m_player{ 0.0f, 0.0f },
	m_bullet{ 0.0f, 0.0f },
	m_isActive(),
	m_isAlive(true),
	m_isIFrameActive(false),
	m_isRunning(false),
	m_isPunching(false),
	m_isFirstChargeActive(true),
	m_isSecondChargeActive(true),
	m_isDodging(false),
	m_isMovingUp(false),
	m_isMovingDown(false),
	m_isMovingLeft(false),
	m_isMovingRight(false),
	m_diagonal(false),
	m_elapsedTime(),
	m_playerAngle(),
	m_currentSpeed(),
	m_maxSpeed(),
	m_dodgeStrength(),
	m_dodgeDuration(),
	m_coolDownDuration(),
	m_acceleration(),
	m_friction(),
	m_momentum(),
	m_playerHp(),
	m_bulletId(),
	m_abilityCount(),
	m_bulletPosition{ 0.0f, 0.0f },
	m_bulletVelocity{ 0.0f, 0.0f },
	m_crosshairPosition{ 0.0f, 0.0f },
	m_crosshairDirection{ 0.0f, 0.0f },
	m_crosshairDirectionNormal{ 0.0f, 0.0f },
	m_playerPosition(),
	m_playerDirection(),
	m_playerVelocity(),
	m_colour(RED),
	m_hitboxColour(GREEN)
{
	//load images
	loadImages();

	//player
	m_playerHp = 100;
	m_abilityCount = 2;
	m_currentSpeed = 1.0f;
	m_maxSpeed = 10.0f;
	m_acceleration = 8.0f;
	m_friction = 3.0f;
	m_dodgeStrength = 3.0f;
	m_coolDownDuration = 6.0f;
	m_momentum = m_currentSpeed - m_maxSpeed;

	m_player =
	{
		m_playerPosition.x,
		m_playerPosition.y,
		30.0f,
		30.0f
	};

	m_playerPosition = { m_player.x, m_player.y };
	m_playerDirection = { 0.0f, 0.0f };
	m_playerVelocity = { 0.0f, 0.0f };
	m_playerAcceleration = { 0.0f, 0.0f };
	m_playerLastDirection = { 0.0f, 0.0f };

	//player hitbox
	m_playerHitbox =
	{
		m_playerPosition.x - 15.0f,
		m_playerPosition.y - 15.0f,
		27.0,
		27.0
	};

	//player's crosshair
	m_crosshairPosition = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };

	for (int i = 0; i < BULLET_POOL_SIZE; i++)
	{
		//bullet rectangle
		m_bullet[i].x = (m_player.x);
		m_bullet[i].y = (m_player.y);
		m_bullet[i].width = 3.0f;
		m_bullet[i].height = 5.0f;
	}

	//initializing the bullets. False means the bullet has not been fired yet
	for (int i = 0; i < BULLET_POOL_SIZE; i++)
	{
		m_isActive[i] = false;
	}
}


//----------------------------------------
//DESTRUCTOR
//----------------------------------------
Player::~Player()
{
	//frees up memory
	unloadImages();
}

void Player::loadImages()
{
	m_crosshairPNG = LoadImage("assets/png/crosshair.png");
	m_crosshair = LoadTextureFromImage(m_crosshairPNG);
}

void Player::unloadImages()
{
	UnloadImage(m_crosshairPNG);
}

void Player::movePlayerUp()
{
	//player is moving up
	m_playerDirection.y = -1.0f;
	m_playerDirection.x = 0.0f;

	//calculate acceleration force
	m_playerAcceleration.x = m_playerDirection.x * m_acceleration;
	m_playerAcceleration.y = m_playerDirection.y * m_acceleration;

	//calculate player's velocity
	m_playerVelocity.x += m_playerAcceleration.x * GetFrameTime();
	m_playerVelocity.y += m_playerAcceleration.y * GetFrameTime();

	//stores last direction pressed
	m_playerLastDirection.y = m_playerDirection.y;
}

void Player::movePlayerDown()
{
	//player is moving down
	m_playerDirection.y = 1.0f;
	m_playerDirection.x = 0.0f;

	//calculate acceleration force
	m_playerAcceleration.x = m_playerDirection.x * m_acceleration;
	m_playerAcceleration.y = m_playerDirection.y * m_acceleration;

	//calculate player's velocity
	m_playerVelocity.x += m_playerAcceleration.x * GetFrameTime();
	m_playerVelocity.y += m_playerAcceleration.y * GetFrameTime();

	//stores last direction pressed
	m_playerLastDirection.y = m_playerDirection.y;
}

void Player::movePlayerLeft()
{
	//player is moving left
	m_playerDirection.x = -1.0f;
	m_playerDirection.y = 0.0f;

	//calculate acceleration force
	m_playerAcceleration.x = m_playerDirection.x * m_acceleration;
	m_playerAcceleration.y = m_playerDirection.y * m_acceleration;

	//calculate player's velocity
	m_playerVelocity.x += m_playerAcceleration.x * GetFrameTime();
	m_playerVelocity.y += m_playerAcceleration.y * GetFrameTime();

	//stores last direction pressed
	m_playerLastDirection.x = m_playerDirection.x;
}

void Player::movePlayerRight()
{
	//player is moving right
	m_playerDirection.x = 1.0f;
	m_playerDirection.y = 0.0f;

	//calculate acceleration force
	m_playerAcceleration.x = m_playerDirection.x * m_acceleration;
	m_playerAcceleration.y = m_playerDirection.y * m_acceleration;

	//calculate player's velocity
	m_playerVelocity.x += m_playerAcceleration.x * GetFrameTime();
	m_playerVelocity.y += m_playerAcceleration.y * GetFrameTime();

	//stores last direction pressed
	m_playerLastDirection.x = m_playerDirection.x;
}

void Player::dodgeAbility()
{
	Vector2 dodgeForce = Vector2Add(m_playerVelocity, (Vector2Scale(m_playerVelocity, m_dodgeStrength)));
	m_playerVelocity = Vector2Add(m_playerVelocity, dodgeForce);
	if (Vector2Length(m_playerVelocity) > 1)
	{
		Vector2Normalize(m_playerVelocity);
		Vector2Scale(m_playerVelocity, m_maxSpeed);
	}
}

void Player::handlePlayerInput()
{
	switch (m_currentState)
	{
	case PlayerState::CAN_MOVE:
		if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
		{
			m_isMovingUp = true;
			m_isMovingLeft = false;
			m_isMovingRight = false;
			m_isMovingDown = false;
			movePlayerUp();
		}
		if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
		{
			m_isMovingDown = true;
			m_isMovingUp = false;
			m_isMovingLeft = false;
			m_isMovingRight = false;
			movePlayerDown();
		}
		if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
		{
			m_isMovingLeft = true;
			m_isMovingRight = false;
			m_isMovingUp = false;
			m_isMovingDown = false;
			movePlayerLeft();
		}
		if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
		{
			m_isMovingRight = true;
			m_isMovingLeft = false;
			m_isMovingUp = false;
			m_isMovingDown = false;
			movePlayerRight();
		}
		if (m_currentState != PlayerState::DODGING && m_abilityCount > 0 && IsKeyPressed(KEY_SPACE))
		{
			m_currentState = PlayerState::DODGING;
			dodgeAbility();
			if (m_abilityCount > 0)
			{
				m_abilityCount--;

				if (m_abilityCount == 2)
				{
					m_isFirstChargeActive = true;
					m_isSecondChargeActive = true;
				}

				if (m_abilityCount == 1)
				{
					m_isFirstChargeActive = false;
					m_FirstChargeCoolDownTimer.startTimer(&m_FirstChargeCoolDownTimer, m_coolDownDuration);
				}
				if (m_abilityCount == 0)
				{
					m_isSecondChargeActive = false;
					m_SecondChargeCoolDownTimer.startTimer(&m_SecondChargeCoolDownTimer, m_coolDownDuration);
				}

			}
		}
	}
}

void Player::updatePlayerPosition()
{
	//subtracts friction force from velocity and apply the force
	m_playerVelocity = Vector2Subtract(m_playerVelocity, Vector2Scale(m_playerVelocity, m_friction * GetFrameTime()));

	m_player.x += m_playerVelocity.x;
	m_player.y += m_playerVelocity.y;

	m_playerHitbox.x += m_playerVelocity.x;
	m_playerHitbox.y += m_playerVelocity.y;
}


//----------------------------------------
//BULLET MATH
//----------------------------------------
void Player::bulletCalculation()
{
	for (int i = 0; i < BULLET_POOL_SIZE; i++)
	{
		if (!m_isActive[i])
		{
			m_isActive[i] = true;

			m_bulletPosition[i] = m_playerPosition;
			m_crosshairDirection = Vector2Subtract(m_crosshairPosition, m_playerPosition);

			//setting vector magnitude to 1. This keeps the bullets velocity consistent
			m_crosshairDirectionNormal = Vector2Normalize(m_crosshairDirection);

			m_bulletVelocity[i] = Vector2Scale(m_crosshairDirectionNormal, BULLET_SPEED);

			break;
		}
	}
}

void Player::movebullet()
{
	for (int i = 0; i < BULLET_POOL_SIZE; i++)
	{
		if (m_isActive[i])
		{
			m_bulletPosition[i].x += m_bulletVelocity[i].x * GetFrameTime();
			m_bulletPosition[i].y += m_bulletVelocity[i].y * GetFrameTime();
			if (m_bulletPosition[i].x > GetScreenWidth() || m_bulletPosition[i].x < 0 || m_bulletPosition[i].y > GetScreenHeight() || m_bulletPosition[i].y < 0)
			{
				deactivate(i);
			}
		}
	}
}


bool Player::isActive(int index)
{
	return m_isActive[index];
}

bool Player::isPunchingActive()
{
	return m_isPunching;
}

void Player::deactivate(int index)
{
	m_isActive[index] = false;
}

PlayerState Player::getPlayerState()
{
	return m_currentState;
}

Vector2 Player::getBulletPosition(int index)
{
	return m_bulletPosition[index];
}

Vector2 Player::getBulletVelocity()
{
	for (int i = 0; i < BULLET_POOL_SIZE; i++)
	{
		return m_bulletVelocity[i];
	}
}


Vector2 Player::getCrosshairPosition()
{
	return m_crosshairPosition;
}

Vector2 Player::getCrosshairDirection()
{
	return m_crosshairDirection;
}

Vector2 Player::getPlayerPosition()
{
	m_playerPosition.x = m_player.x;
	m_playerPosition.y = m_player.y;
	return m_playerPosition;
}

Vector2 Player::getPlayerDirection()
{
	return m_playerDirection;
}

Vector2 Player::getPlayerLastDirection()
{
	return m_playerLastDirection;
}

Vector2 Player::getPlayerVelocity()
{
	return m_playerVelocity;
}

Vector2 Player::getPlayerAccelerationForce()
{
	return m_playerAcceleration;
}

float Player::getPlayerAcceleration()
{
	return m_acceleration;
}

Rectangle Player::getPlayerHitbox()
{
	return m_player;
}

float Player::getPlayerAngle()
{
	return m_playerAngle;
}

int Player::getPlayerHp()
{
	return m_playerHp;
}

int Player::getAbilityCount()
{
	return m_abilityCount;
}

void Player::setPunchPosition(Vector2 newPos)
{
	m_punchDistance = newPos;
}

void Player::setPlayerAngle(float angle)
{
	m_playerAngle = angle;
}

void Player::setPlayerSpeed(float speed)
{
	m_acceleration = speed;
}

void Player::setPlayerDirection(Vector2 direction)
{
	m_playerDirection = direction;
}

void Player::setColour(Color colour)
{
	m_colour = colour;
}

void Player::damagePlayer()
{
	if (m_isIFrameActive)
		return;
	m_playerHp -= 25;
	if (m_playerHp <= 0)
	{
		m_playerHp = 0;
		m_isAlive = false;
	}
}

void Player::updateDodgeIFrameTimer()
{
	m_playerIFrameTimer.updateTimer(&m_playerIFrameTimer);
	if (m_playerIFrameTimer.isTimerDone(&m_playerIFrameTimer))
	{
		m_currentState = PlayerState::CAN_MOVE;
		m_isIFrameActive = false;
	}
}

void Player::updateDodgeCoolDownTimer()
{
	m_FirstChargeCoolDownTimer.updateTimer(&m_FirstChargeCoolDownTimer);
	if (m_FirstChargeCoolDownTimer.isTimerDone(&m_FirstChargeCoolDownTimer))
	{
		if (!m_isFirstChargeActive)
		{
			m_isFirstChargeActive = true;
			m_abilityCount++;
		}
	}
	m_SecondChargeCoolDownTimer.updateTimer(&m_SecondChargeCoolDownTimer);
	if (m_SecondChargeCoolDownTimer.isTimerDone(&m_SecondChargeCoolDownTimer))
	{
		if (!m_isSecondChargeActive)
		{
			m_isSecondChargeActive = true;
			m_abilityCount++;
		}
	}

}

void Player::updateMagnetizeTimer()
{
	m_magnetizeDuration.updateTimer(&m_magnetizeDuration);
	if (m_magnetizeDuration.isTimerDone(&m_magnetizeDuration))
	{
		m_isPunching = false;
	}
}

void Player::updatePlayerTimers()
{
	updateDodgeIFrameTimer();
	updateDodgeCoolDownTimer();
	updateMagnetizeTimer();
}

void Player::activateIFrames()
{
	m_isIFrameActive = true;
}

void Player::deactivateIFrames()
{
	m_isIFrameActive = false;
}

void Player::activateSprint()
{
	m_isRunning = true;
}

void Player::activatePunch()
{
	m_isPunching = true;
}

void Player::deactivatePunch()
{
	m_isPunching = false;
}

void Player::deactivateSprint()
{
	m_isRunning = false;
}

void Player::activateMagnetizeTimer()
{
	m_magnetizeDuration.startTimer(&m_magnetizeDuration, MAGNETIZE_DURATION);
}

void Player::setBulletPosition(Vector2 bulletPos)
{
	for (int i = 0; i < BULLET_POOL_SIZE; i++)
	{
		m_bulletPosition[i] = bulletPos;
	}
}

Vector2 Player::getBullet()
{
	return Vector2();
}

bool Player::isIFramesActive()
{
	return m_isIFrameActive;
}

bool Player::isAlive()
{
	return m_isAlive;
}

bool Player::isFiring()
{
	return m_isActive;
}

bool Player::isRunning()
{
	return m_isRunning;
}

bool Player::isMovingUp()
{
	return m_isMovingUp;
}

bool Player::isMovingDown()
{
	return m_isMovingDown;
}

bool Player::isMovingLeft()
{
	return m_isMovingLeft;
}

bool Player::isMovingRight()
{
	return m_isMovingRight;
}

void Player::draw(float playerAngle)
{
	//setting origin to middle of rec
	Vector2 bodyOrigin{ m_player.width / 2.0f, m_player.height / 2.0f };
	////draw player
	DrawRectanglePro(m_player, bodyOrigin, playerAngle, BLUE);
}

void Player::drawPlayerHitbox()
{
	DrawRectangleLines(m_playerHitbox.x + 4.0f, m_playerHitbox.y + 4.0f, m_player.width - 8.0f, m_player.height - 8.0f, m_hitboxColour);
}

void Player::displayPlayerHp()
{
	DrawText(TextFormat("Health: %i", m_playerHp), m_playerPosition.x - 40.0, m_playerPosition.y - 50.0, 20, WHITE);
}



