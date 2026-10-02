#define RAYTMX_IMPLEMENTATION
#include "raytmx.h"

#define HOXML_IMPLEMENTATION
#include "hoxml.h"

#include "Timer.h"
#include "Player.h"
#include "PlayerCamera.h"
#include "Ui.h"
#include "Crosshair.h"
#include "Weapon.h"

#include "Alien.h"
#include "Larvae.h"
#include "SpeedAlien.h"
#include "BossAlien.h"

#include "Tilemap.h"
#include "Constants.h"
#include "Collision.h"

#include "raylib.h"
#include "raymath.h"

#include <vector>
#include <memory>

#define MAX_FRAME_SPEED = 15
#define MIN_FRAME_SPEED = 1

void update()
{
	// check keyboard
	// update positions
}

void drawScene()
{

}

int main(void)
{
	//window size
	const int screenWidth = 1600;
	const int screenHeight = 900;
	InitWindow(screenWidth, screenHeight, "Top Down Game");
	//ToggleBorderlessWindowed();
	SetTargetFPS(144);

	//timer for players invincibility frames
	Timer playerTimer;
	float iFrameDuration = 1.5f; //(seconds)

	//timer for enemies invincibility frames
	Timer enemyIFrame;
	float enemyIFrameDuration = 2.0; //(seconds)

	//objects
	Player myPlayer;
	PlayerCamera playerCamera(myPlayer.getPlayerPosition());
	Tilemap prototype;
	Collision wallCollision;
	Ui playerUI;
	Crosshair crosshair;
	Weapon weapon;
	std::vector<std::unique_ptr<Alien>> alienVector;	//unique_ptr for storing a pointer of each aliens class
	Vector2 getMouseWorldPos;
	Vector2 getPlayerWorldPos;
	Vector2 rayOrigin = {};
	Vector2 rayDirection = {};
	bool drawRay = false;
	Image backgroundImage = LoadImage("assets/png/Grid.png");
	Texture2D background = LoadTextureFromImage(backgroundImage);

	//fill the vector of aliens
	for (int i = 0; i < MAX_LARVAE; i++)
	{
		alienVector.push_back(std::make_unique<Larvae>(i, 0.0f, 0.0f));
	}

	for (int i = 0; i < MAX_SPEED_ALIEN; i++)
	{
		alienVector.push_back(std::make_unique<SpeedAlien>(i, 0.0f, 0.0f));
	}
	for (int i = 0; i < MAX_BOSS_ALIEN; i++)
	{
		alienVector.push_back(std::make_unique<BossAlien>(i, 0.0f, 0.0f));
	}

	//----------------------------------------
	//ENEMY SPAWN POSITION
	//----------------------------------------
	for (size_t i = 0; i < alienVector.size(); i++)
	{
		int spawnLocation = GetRandomValue(0, 3);

		switch (spawnLocation)
		{
			//spawn aliens at top
		case 0:
			alienVector[i]->setPosition(GetRandomValue(-50, GetScreenWidth()), 0);
			break;
			//spawn aliens at bottom
		case 1:
			alienVector[i]->setPosition(GetRandomValue(0, GetScreenWidth()), GetScreenHeight() + 50);
			break;
			//spawn aliens on the left
		case 2:
			alienVector[i]->setPosition(0, GetRandomValue(0, GetScreenHeight() + 50));
			break;
			//spawn aliens on the right 
		case 3:
			alienVector[i]->setPosition(GetScreenWidth() + 50, GetRandomValue(0, GetScreenHeight()));
			break;
		}
	}

	//------------------------------------------
	//PLAYER VARIABLES
	//------------------------------------------
	float playerAngle;

	Vector2 playerPosition;
	Vector2 crosshairPosition;

	//setting origin to middle of rec
	Vector2 bodyOrigin{ 30.0f,30.0f };

	//frame counters for sprite animation
	int currentFrame = 0;
	int framesCounter = 0;
	int framesSpeed = 8;
	float time = 0;

	while (!WindowShouldClose())
	{
		HideCursor();

		//------------------------------------------
		//GAME LOOP (Update)
		//------------------------------------------

		//-----------------------------------------------
		//STEERING BEHAVIOUR MOVEMENT (SEEK AND SEPARATE)
		//-----------------------------------------------

		//TODO: FIX ENEMIES SEPARATING WHEN INACTIVE
		for (int sourceIndex = 0; sourceIndex < alienVector.size(); sourceIndex++)
		{
			//first loop checks all the aliens in the vector
			auto& sourceAlien = alienVector[sourceIndex];
			sourceAlien->updateAlienPos(myPlayer.getPlayerPosition(), alienVector);
		}

		//----------------------------------------
		//PLAYER MOVEMENT
		//----------------------------------------

		//update players position every frame. This is so player will continue to move a bit after input minus the friction coefficient
		myPlayer.handlePlayerInput();
		playerUI.getAbilityCount(myPlayer.getAbilityCount());
		playerUI.getAmmoCount(weapon.getAmmoCount());
		weapon.updateTimers();
		weapon.updateMeleeHitboxPosition(myPlayer.getPlayerPosition(), crosshair.getPlayerAngle());
		myPlayer.updatePlayerTimers();
		myPlayer.updatePlayerPosition();

		if (weapon.isMeleeActive())
		{
			myPlayer.setPlayerSpeed(PLAYER_ATTACK_WALK_SPEED);
		}
		else if (!weapon.isMeleeActive())
		{
			myPlayer.setPlayerSpeed(PLAYER_WALK_SPEED);
		}

		//----------------------------------------
		//PLAYER FUNCTIONS
		//----------------------------------------
		if (IsKeyDown(KEY_I))
		{
			for (int i = 0; i < ALIEN_POOL_SIZE; i++)
			{
				alienVector[i]->deactivate();
			}
		}

		//----------------------------------------
		//PLAYER CAMERA
		//----------------------------------------

		//update camera's pos every frame.
		playerCamera.updateCameraPosition(myPlayer.getPlayerPosition());

		//get screen coordiantes for mouse to update player's aim every frame
		getMouseWorldPos = GetScreenToWorld2D(GetMousePosition(), playerCamera.getPlayerCamera());
		crosshair.updatePlayerAim(getMouseWorldPos, myPlayer.getPlayerPosition());

		////----------------------------------------
		////PLAYER SHOOTING
		////----------------------------------------

		weapon.handleWeaponInput(myPlayer.getPlayerPosition(), getMouseWorldPos, alienVector);

		//tick through the timer
		for (auto& alien : alienVector)
		{
			alien->updatePersonalIFrameTimer();
			alien->updateTimers();
		}

		//----------------------------------------
		//COLLISION
		//----------------------------------------
		for (size_t i = 0; i < alienVector.size(); i++)
		{
			if (CheckCollisionRecs(myPlayer.m_playerHitbox, alienVector[i]->getAlien()))
			{
				if (myPlayer.isIFramesActive())
					continue;
				//check if Alien is alive, if not, do not damage the player anymore
				if (alienVector[i]->isActive())
				{
					//if the player gets hit, activate invincibilty frames so player does not instantly die.
					//myPlayer.damagePlayer();
					//playerUI.decreaseHp(25);
					myPlayer.activateIFrames();
					playerTimer.startTimer(&playerTimer, iFrameDuration);
				}
			}
			if (CheckCollisionCircleRec(weapon.getMeleeHitbox(), PUNCH_RADIUS, alienVector[i]->getAlien()) && weapon.isMeleeActive() && !myPlayer.isPunchingActive())
			{
				alienVector[i]->startKnockbackTimer(KNOCKBACK_DURATION_MELEE_1);

				Vector2 knockbackForce = weapon.calculateKnockbackForce(myPlayer.getPlayerPosition(), alienVector[i]->getAlienPos());

				alienVector[i]->setKnockbackVelocity(knockbackForce);
				myPlayer.activatePunch();
			} 
		}

		prototype.checkForWallCollision(myPlayer.getPlayerHitbox());

		//tick through timer 
		playerTimer.updateTimer(&playerTimer);

		//deactivate when the timer is up
		if (playerTimer.isTimerDone(&playerTimer))
			myPlayer.deactivateIFrames();


		//----------------------------------------
		//BEGIN RENDERING
		//----------------------------------------
		BeginDrawing();
		//HideCursor();

		//clears the background and sets it to a colour
		ClearBackground(DARKBLUE);

		//start 2D camera mode
		//BeginMode2D(playerCamera.getPlayerCamera());
		BeginMode2D(playerCamera.getPlayerCamera());

		//draw test map
		prototype.drawMap(playerCamera.getPlayerCamera());

		//test grid (png)
		//DrawTextureEx(background, { -1000.0f, -1000.0f }, 0.0f, 5.0f, WHITE);

		if (weapon.isDebugRay())
		{
			//debugging visual ray
			weapon.drawRay(weapon.getRayOrigin(), weapon.getRayDirection());
		}

		//player direction debug
		//DrawCircle(myPlayer.getPlayerDirection().x, myPlayer.getPlayerDirection().y, 5.0f, LIME);

		//if playerr is alive, draw scene 
		if (myPlayer.isAlive())
		{
			for (int i = 0; i < alienVector.size(); i++)
			{
				alienVector[i]->draw();
			}
			if (myPlayer.isIFramesActive())
			{
				myPlayer.setColour(GRAY);
			}
			else
			{
				myPlayer.setColour(RED);
			}
			myPlayer.draw(crosshair.getPlayerAngle());
			myPlayer.drawPlayerHitbox();
			myPlayer.movebullet();
			weapon.drawMeleeHitbox();
		}
		//end Camera2D
		EndMode2D();


		if (!myPlayer.isAlive())
		{
			//game over screen. Turns your mouse back on and displays death screen.
			ShowCursor();
			DrawText("YOU DIED!\n\nGAME OVER!", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2, 30, WHITE);
		}

		//----------------------------------------
		//DEBUG STUFF
		//----------------------------------------
		//debug info

		//draws an FPS counter in the top left corner of screen
		DrawFPS(10.0f, 10.0f);

		//player's debug (pos, velo, acc, accF, lastDirection, )
		DrawText(TextFormat("Position: (%.0f, %.0f)", myPlayer.getPlayerPosition().x, myPlayer.getPlayerPosition().y), 10, 20, 20, WHITE);
		DrawText(TextFormat("Velocity: (%.0f, %.0f)", myPlayer.getPlayerVelocity().x, myPlayer.getPlayerVelocity().y), 10, 40, 20, WHITE);
		DrawText(TextFormat("Acceleration Vector: (%.0f, %.0f)", myPlayer.getPlayerAccelerationForce().x, myPlayer.getPlayerAccelerationForce().y), 10, 60, 20, WHITE);
		DrawText(TextFormat("Current Direction: (%.0f, %.0f)", myPlayer.getPlayerDirection().x, myPlayer.getPlayerDirection().y), 10, 80, 20, WHITE);
		DrawText(TextFormat("Last Direction: (%.0f, %.0f)", myPlayer.getPlayerLastDirection().x, myPlayer.getPlayerLastDirection().y), 10, 100, 20, WHITE);
		DrawText(TextFormat("Acceleration: %.0f", myPlayer.getPlayerAcceleration()), 10, 120, 20, WHITE);

		if (weapon.getCurrentAttackState() == AttackState::NOT_ATTACKING)
		{
			DrawText("Attack State: IDLE", 10, 260, 20, WHITE);
		}
		if (weapon.getCurrentAttackState() == AttackState::MELEE_1)
		{
			DrawText("Attack State: RIGHT JAB!", 10, 260, 20, WHITE);
		}
		if (weapon.getCurrentAttackState() == AttackState::MELEE_2)
		{
			DrawText("Attack State: LEFT HOOK BABY!", 10, 260, 20, WHITE);
		}
		if (weapon.getCurrentAttackState() == AttackState::MELEE_3)
		{
			DrawText("Attack State: FALCON KICK!!!", 10, 260, 20, WHITE);
		}


		if (weapon.isMeleeActive())
		{
			DrawText("ATTACKING", 10, 280, 20, WHITE);
		}
		else
		{
			DrawText("NOT ATTACKING", 10, 280, 20, WHITE);
		}
		if (weapon.isMeleeActive())
		{
			DrawText("HITBOX ACTIVE", 10, 300, 20, WHITE);
		}
		else
		{
			DrawText("HITBOX IN-ACTIVE", 10, 300, 20, WHITE);
		}

		DrawText(TextFormat("Camera Position: (%.0f, %.0f)", playerCamera.getPlayerCamera().target.x, playerCamera.getPlayerCamera().target.y), 10, 320, 20, WHITE);

		//if (wallCollision.isCollidingWithWall())
		//{
		//	DrawText("WALL COLLISION!", 10, 340, 20, WHITE);
		//}
		//else
		//{
		//	DrawText("WALL COLLISION!", 10, 340, 20, WHITE);
		//}

		//debug for invincibility on player
		if (myPlayer.isIFramesActive())
		{
			DrawText("iFrame Active", 10, 160, 20, WHITE);
		}

		if (myPlayer.isMovingUp())
		{
			DrawText("Moving up", 10, 180, 20, WHITE);
		}
		if (myPlayer.isMovingDown())
		{
			DrawText("Moving Down", 10, 200, 20, WHITE);
		}
		if (myPlayer.isMovingLeft())
		{
			DrawText("Moving Left", 10, 220, 20, WHITE);
		}
		if (myPlayer.isMovingRight())
		{
			DrawText("Moving Right", 10, 240, 20, WHITE);
		}

		//----------------------------------------
		//PLAYER UI (MUST BE AFTER EndMode2D)
		//----------------------------------------
		//player UI elements
		crosshair.drawCrosshair();
		playerUI.drawUi();


		//stop drawing window
		EndDrawing();
	}
	UnloadTexture(background);
	CloseWindow();
	return 0;
}


