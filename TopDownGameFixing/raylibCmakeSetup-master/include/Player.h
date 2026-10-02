#pragma once
#include "raylib.h"
#include "Constants.h"
#include "Timer.h"
#include <vector>
#include <chrono>

//TODO: ORAGANIZE CLASS IT IS A FUCKING MESS MEMBER VARIABLES WITHW WRONG TITLES

enum class PlayerState
{
	CAN_MOVE,
	CANT_MOVE,
	IDLE,
	WALKING,
	RUNNING,
	DODGING
};

class Player
{
public:
	Player();
	~Player();

	//textures
	Rectangle m_player;
	Rectangle m_playerHitbox;
	Rectangle m_bullet[BULLET_POOL_SIZE];

	//images
	void loadImages();
	void unloadImages();

	//movement
	void movePlayerUp();
	void movePlayerDown();
	void movePlayerLeft();
	void movePlayerRight();
	void dodgeAbility();

	void handlePlayerInput();
	void updatePlayerPosition();

	void bulletCalculation();
	void movebullet();

	//getters
	PlayerState getPlayerState();

	Vector2 getBulletPosition(int index);
	Vector2 getBulletVelocity();

	Vector2 getCrosshairPosition();
	Vector2 getCrosshairDirection();

	Vector2 getPlayerPosition();
	Vector2 getPlayerDirection();
	Vector2 getPlayerLastDirection();
	Vector2 getPlayerVelocity();
	Vector2 getPlayerAccelerationForce();

	Rectangle getPlayerHitbox();

	float getPlayerAcceleration();
	float getDodgeDuration();

	float getPlayerAngle();
	int getPlayerHp();
	int getAbilityCount();

	//setters
	void setPunchPosition(Vector2 newPos);
	void setPlayerAngle(float angle);
	void setPlayerSpeed(float speed);
	void setPlayerDirection(Vector2 direction);
	void setColour(Color colour);

	void damagePlayer();
	void updateDodgeIFrameTimer();
	void updateDodgeCoolDownTimer();
	void updateMagnetizeTimer();
	void updatePlayerTimers();

	void setBulletPosition(Vector2 bulletPos);
	Vector2 getBullet();

	bool isActive(int index);
	bool isPunchingActive();
	bool isIFramesActive();
	bool isAlive();
	bool isFiring();
	bool isRunning();
	bool isMovingUp();
	bool isMovingDown();
	bool isMovingLeft();
	bool isMovingRight();

	void deactivate(int index);

	void activateIFrames();
	void deactivateIFrames();

	void activatePunch();
	void deactivatePunch();

	void activateSprint();
	void deactivateSprint();

	void activateMagnetizeTimer();


	void draw(float playerAngle);
	void drawPlayerHitbox();
	void displayPlayerHp();

private:
	//player state
	PlayerState m_currentState;

	//player timers
	Timer m_playerIFrameTimer;
	Timer m_FirstChargeCoolDownTimer;
	Timer m_SecondChargeCoolDownTimer;
	Timer m_dodgeDuration;
	Timer m_magnetizeDuration;

	//images
	Image m_crosshairPNG; //crosshair image

	//textures
	Texture2D m_crosshair; //useable crosshair textures

	//bools
	bool m_isActive[BULLET_POOL_SIZE];
	bool m_isAlive;
	bool m_isIFrameActive;
	bool m_isRunning;
	bool m_isPunching;

	//ability bools
	bool m_isFirstChargeActive;
	bool m_isSecondChargeActive;

	//movement bools
	bool m_isDodging;

	bool m_isMovingUp;
	bool m_isMovingDown;
	bool m_isMovingLeft;
	bool m_isMovingRight;
	bool m_diagonal;

	//timer
	float m_elapsedTime;

	//floats
	float m_playerAngle;
	float m_currentSpeed;
	float m_maxSpeed;
	float m_playerVelocityValue;
	float m_dodgeStrength;
	float m_coolDownDuration;
	float m_acceleration;
	float m_friction;
	float m_momentum;

	//ints
	int m_playerHp;
	int m_bulletId;
	int m_abilityCount;

	//vector arrays
	std::vector<Player> m_bullets;

	//vector variables
	//bullets
	Vector2 m_bulletPosition[BULLET_POOL_SIZE];
	Vector2 m_bulletVelocity[BULLET_POOL_SIZE];

	//crosshair
	Vector2 m_crosshairPosition;
	Vector2 m_crosshairDirection;
	Vector2 m_crosshairDirectionNormal;

	//player
	Vector2 m_playerAcceleration;
	Vector2 m_playerDirection;
	Vector2 m_playerLastDirection;
	Vector2 m_playerPosition;
	Vector2 m_playerVelocity;
	Vector2 m_punchDistance;

	//Colours
	Color m_colour;
	Color m_hitboxColour;

};