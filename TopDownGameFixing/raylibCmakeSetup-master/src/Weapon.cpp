#include "Weapon.h"
#include "Player.h"
#include "Constants.h"
#include <raylib.h>
#include <raymath.h>

Weapon::Weapon() :
	m_currentWeapon(),
	m_closestAlien(),
	m_meleeAttackActive(),
	m_comboAttackForgiveness(),
	m_laserPistolROF(),
	m_energyRifleROF(),
	m_isRayColliding(false),
	m_isAttacking(false),
	m_isMeleeActive(false),
	m_drawRay(false),
	m_bullet(),
	m_meleeHitbox(),
	m_position(),
	m_rayOrigin(),
	m_rayDirection(),
	m_meleeOrigin(),
	m_laserPistolAmmoCount(),
	m_energyRifleAmmoCount(),
	m_meleeChargeCount(),
	m_attackSequenceCount()

{
	m_bullet =
	{
		m_position.x,
		m_position.y,
		5.0f,
		5.0f
	};

	m_laserPistolAmmoCount = 12;
	m_energyRifleAmmoCount = 35;
	m_meleeChargeCount = 3;
	m_attackSequenceCount = 0;

	m_currentAttackState = AttackState::NOT_ATTACKING;
}

Weapon::~Weapon()
{

}

int Weapon::getAmmoCount()
{
	if (m_currentWeapon == WeaponType::WEAPONTYPE_LASERPISTOL)
	{
		return m_laserPistolAmmoCount;
	}

	if (m_currentWeapon == WeaponType::WEAPONTYPE_ENERGYRIFLE)
	{
		return m_energyRifleAmmoCount;
	}
	if (m_currentWeapon == WeaponType::WEAPONTYPE_MELEE)
	{
		return m_meleeChargeCount;
	}
}

int Weapon::getComboSequence()
{
	return m_attackSequenceCount;
}

AttackState Weapon::getCurrentAttackState()
{
	return m_currentAttackState;
}

void Weapon::updateMeleeHitboxPosition(Vector2 playerPos, float playerAngle)
{
	//converting the pplayer angle from degrees to radians
	float playerAngleRad = playerAngle * DEG2RAD;

	//converting player's direction angle into a point
	Vector2 playerDirection = { cosf(playerAngleRad), sinf(playerAngleRad) };

	Vector2 meleePosition = Vector2Add(playerPos, Vector2Scale(playerDirection, MELEE_HITBOX_OFFSET));


	if (m_currentAttackState == AttackState::MELEE_1)
	{
		m_meleeHitbox.x = meleePosition.x;
		m_meleeHitbox.y = meleePosition.y;
	}
	if (m_currentAttackState == AttackState::MELEE_2)
	{
		m_meleeHitbox.x = meleePosition.x;
		m_meleeHitbox.y = meleePosition.y;
	}
	if (m_currentAttackState == AttackState::MELEE_3)
	{
		m_meleeHitbox.x = meleePosition.x;
		m_meleeHitbox.y = meleePosition.y;
	}
}

void Weapon::updateTimers()
{
	m_laserPistolROF.updateTimer(&m_laserPistolROF);
	m_energyRifleROF.updateTimer(&m_energyRifleROF);
	m_meleeAttackActive.updateTimer(&m_meleeAttackActive);
	m_wait.updateTimer(&m_wait);
	m_comboAttackForgiveness.updateTimer(&m_comboAttackForgiveness);
	m_knockbackDuration.updateTimer(&m_knockbackDuration);
	if (m_comboAttackForgiveness.isTimerDone(&m_comboAttackForgiveness))
	{
		m_currentAttackState = AttackState::NOT_ATTACKING;
	}
	if (m_wait.isTimerDone(&m_wait))
	{
		m_isMeleeActive = false;
	}
}

Vector2 Weapon::getMeleeHitbox()
{
	return m_meleeHitbox;
}

Vector2 Weapon::getRayOrigin()
{
	return m_rayOrigin;
}

Vector2 Weapon::getRayDirection()
{
	return m_rayDirection;
}

void Weapon::handleWeaponInput(Vector2 playerPos, Vector2 mouseWorldPos, std::vector<std::unique_ptr<Alien>>& vectorOfAliens)
{
	//##################//
	// WEAPON FUNCTIONS //	
	//##################// 

	//laser pistol
	if (IsKeyPressed(KEY_ONE))
	{
		m_currentWeapon = WeaponType::WEAPONTYPE_LASERPISTOL;
	}

	//energy rifle
	if (IsKeyPressed(KEY_TWO))
	{
		m_currentWeapon = WeaponType::WEAPONTYPE_ENERGYRIFLE;
	}

	//melee
	if (IsKeyPressed(KEY_X))
	{
		m_currentWeapon = WeaponType::WEAPONTYPE_MELEE;
	}

	//reload
	if (IsKeyPressed(KEY_R))
	{
		reload();
	}

	//##################//
	//      WEAPONS     //	
	//##################// 
	switch (m_currentWeapon)
	{
		//----------------------------------------
		//LASER PISTOL
		//----------------------------------------
	case WeaponType::WEAPONTYPE_LASERPISTOL:

		if (IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT) && m_laserPistolROF.isTimerDone(&m_laserPistolROF))
		{
			m_laserPistolROF.startTimer(&m_laserPistolROF, LASER_PISTOL_ROF);
			//shoot if bullets don't = 0.
			if (m_laserPistolAmmoCount > 0)
			{

				///***this shit barely makes any sense to me so there is gonna be some overkill documentation
				///here because I am a little slow like when you push "2, 0, 0" on a microwave but the buttons 
				/// 1-9 correspond to minutes so that mf goes in for 20:00 minutes and you're just there like 
				/// ( ⊙ _ ⊙ ) ***

				m_laserPistolAmmoCount--;

				//stores players current pos for raycast calc in rayorigin. This is the initial point where the ray will be casted from.
				m_rayOrigin = playerPos;

				//calculate direction of raycast.
				m_rayDirection = Vector2Normalize((Vector2Subtract(mouseWorldPos, m_rayOrigin)));	//normalize to convert to unit vector, I only need direction

				//turn on debug ray
				m_drawRay = true;

				//loops through alienGrunt vector. Range based loop.

					//variable to store the distance of the closest alien hit by the raycast.
				Alien* hitAlien = getClosestHitAlien(m_rayOrigin, m_rayDirection, vectorOfAliens);

				//check if null
				if (hitAlien != nullptr)
				{
					//checks for invincibility
					if (!hitAlien->isIFramesActive())
					{
						//hit the closest alien
						hitAlien->damageEnemy(LASER_PISTOL_DAMAGE);
					}
				}
			}
		}
		else
		{
			//turn off debug ray
			m_drawRay = false;
		}
		break;
		//---------------------------------------- 
		//MELEE ATTACK
		//----------------------------------------
	case WeaponType::WEAPONTYPE_MELEE:
		if (IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT) && m_currentAttackState == AttackState::NOT_ATTACKING)
		{

			m_wait.startTimer(&m_wait, WAIT_TIME);
			m_meleeAttackActive.startTimer(&m_meleeAttackActive, MELEE_ATTACK_ROF);
			m_comboAttackForgiveness.startTimer(&m_comboAttackForgiveness, NEXT_COMBO_ATTACK_FORGIVENESS);
			m_isMeleeActive = true;
			m_currentAttackState = AttackState::MELEE_1;
			melee();


		}
		if (IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT) && (m_currentAttackState == AttackState::MELEE_1))
		{
			if (m_wait.isTimerDone(&m_wait))
			{
				m_isMeleeActive = true;
				m_wait.startTimer(&m_wait, WAIT_TIME);
				m_comboAttackForgiveness.startTimer(&m_comboAttackForgiveness, NEXT_COMBO_ATTACK_FORGIVENESS);
				m_currentAttackState = AttackState::MELEE_2;
			}
		}
		if (IsMouseButtonPressed(MouseButton::MOUSE_BUTTON_LEFT) && (m_currentAttackState == AttackState::MELEE_2))
		{
			if (m_wait.isTimerDone(&m_wait))
			{
				m_isMeleeActive = true;
				m_wait.startTimer(&m_wait, WAIT_TIME);
				m_currentAttackState = AttackState::MELEE_3;
			}
		}
		break;
	case WeaponType::WEAPONTYPE_ENERGYRIFLE:
		if (IsMouseButtonDown(MouseButton::MOUSE_BUTTON_LEFT) && m_energyRifleROF.isTimerDone(&m_energyRifleROF))
		{
			m_energyRifleROF.startTimer(&m_energyRifleROF, ENERGY_RIFLE_ROF);
			//shoot if bullets don't = 0.
			if (m_energyRifleAmmoCount > 0)
			{
				m_energyRifleAmmoCount--;

				//stores players current pos for raycast calc in rayorigin. This is the initial point where the ray will be casted from.
				m_rayOrigin = playerPos;

				//calculate direction of raycast.
				m_rayDirection = Vector2Normalize((Vector2Subtract(mouseWorldPos, m_rayOrigin)));	//normalize to convert to unit vector, I only need direction

				//turn on debug ray
				m_drawRay = true;

				//loops through alienGrunt vector. Range based loop.

				//variable to store the distance of the closest alien hit by the raycast.
				Alien* hitAlien = getClosestHitAlien(m_rayOrigin, m_rayDirection, vectorOfAliens);

				//check if null
				if (hitAlien != nullptr)
				{
					//checks for invincibility
					if (!hitAlien->isIFramesActive())
					{
						//hit the closest alien
						hitAlien->damageEnemy(ENRGY_RIFLE_DAMAGE);
					}
				}
			}
		}
		else
		{
			//turn off debug ray
			m_drawRay = false;
		}
		break;
	}
}

void Weapon::melee()
{
	drawMeleeHitbox();
}

Vector2 Weapon::calculateKnockbackForce(Vector2 source, Vector2 target)
{

	//knockback vector
	Vector2 knockbackForce = Vector2Subtract(target, source);

	//get the direction vector
	Vector2 knockbackDirection = Vector2Normalize(knockbackForce);

	//apply the knockback
	if (m_currentAttackState == AttackState::MELEE_1)
	{
		m_knockbackDuration.startTimer(&m_knockbackDuration, KNOCKBACK_DURATION_MELEE_1);
		return Vector2Scale(knockbackDirection, MELEE_1_KNOCKBACK_FORCE);
	}
	if (m_currentAttackState == AttackState::MELEE_2)
	{
		m_knockbackDuration.startTimer(&m_knockbackDuration, KNOCKBACK_DURATION_MELEE_2);
		return Vector2Scale(knockbackDirection, MELEE_2_KNOCKBACK_FORCE);
	}
	if (m_currentAttackState == AttackState::MELEE_3)
	{
		m_knockbackDuration.startTimer(&m_knockbackDuration, KNOCKBACK_DURATION_MELEE_3);
		return Vector2Scale(knockbackDirection, MELEE_3_KNOCKBACK_FORCE);
	}
}

void Weapon::reload()
{
	if (m_currentWeapon == WeaponType::WEAPONTYPE_LASERPISTOL)
	{
		m_laserPistolAmmoCount = 12;
	}
	if (m_currentWeapon == WeaponType::WEAPONTYPE_ENERGYRIFLE && m_energyRifleAmmoCount < 35)
	{
		m_energyRifleAmmoCount = 35;
	}
}

void Weapon::drawRay(Vector2 rayOrigin, Vector2 rayDirection)
{
	Vector2 rayEnd = { rayOrigin.x + rayDirection.x * 5000.0f, rayOrigin.y + rayDirection.y * 5000.0f };
	DrawLineEx(rayOrigin, rayEnd, 5.0f, WHITE);
}

void Weapon::drawMeleeHitbox()
{
	if (isMeleeActive() && m_currentAttackState == AttackState::MELEE_1)
	{
		DrawCircle(m_meleeHitbox.x, m_meleeHitbox.y, PUNCH_RADIUS, GREEN);
	}
	if (isMeleeActive() && m_currentAttackState == AttackState::MELEE_2)
	{
		DrawCircle(m_meleeHitbox.x, m_meleeHitbox.y, PUNCH_RADIUS, ORANGE);
	}
	if (isMeleeActive() && m_currentAttackState == AttackState::MELEE_3)
	{
		DrawCircle(m_meleeHitbox.x, m_meleeHitbox.y, PUNCH_RADIUS, RED);
	}
}

void Weapon::ammoCountDown()
{
	m_laserPistolAmmoCount--;
}

//2D raycast. Uses the slab method. Got from here https://en.wikipedia.org/wiki/Slab_method
bool Weapon::rayCollision(Vector2 rayOrigin, Vector2 rayDirection, Rectangle enemies, float& hitDist)
{
	//edge case check if direction is perfectly horizontal. Prevents division by zero error.
	float tX1, tX2;
	if (fabs(rayDirection.x) < 0.0001f)
	{
		if (rayOrigin.x < enemies.x || rayOrigin.x > enemies.x + enemies.width)
		{
			return false;
		}
		tX1 = -FLT_MAX;
		tX2 = FLT_MAX;
	}
	else
	{
		//Distance to the left side
		tX1 = (enemies.x - rayOrigin.x) / rayDirection.x;
		//Distance to the right side
		tX2 = (enemies.x + enemies.width - rayOrigin.x) / rayDirection.x;
	}

	//edge case check if direction is perfectly vertical. Prevents division by zero error.
	float tY1, tY2;
	if (fabs(rayDirection.y) < 0.0001f)
	{
		if (rayOrigin.y < enemies.y || rayOrigin.y > enemies.y + enemies.height)
		{
			return false;
		}
		tY1 = -FLT_MAX;
		tY2 = FLT_MAX;
	}
	else
	{
		//Distance to the bottom
		tY1 = (enemies.y - rayOrigin.y) / rayDirection.y;
		//Distacne to the top
		tY2 = (enemies.y + enemies.height - rayOrigin.y) / rayDirection.y;
	}

	//find the entry time (for x slab and y slab)
	float tMin = std::max(std::min(tX1, tX2), std::min(tY1, tY2));

	//find the exit times (for slab x and y slab)
	float tMax = std::min(std::max(tX1, tX2), std::max(tY1, tY2));

	if (tMax < 0)
	{
		return false;
	}

	if (tMin > tMax)
	{
		return false;
	}

	hitDist = std::max(tMin, 0.0f);
	return true;
}

bool Weapon::isDebugRay()
{
	return m_drawRay;
}

bool Weapon::isMeleeActive()
{
	return m_isMeleeActive;
}

bool Weapon::isAttacking()
{
	return m_isAttacking;
}

Alien* Weapon::getClosestHitAlien(Vector2 rayOrigin, Vector2 rayDirection, std::vector<std::unique_ptr<Alien>>& aliens)
{
	//store the closest hit enemy. set to nullptr because it has not hit anything at the start yet.
	m_closestAlien = nullptr;
	//stores distnace to the closest hit. Initially, this is infinite because there is no target yet.
	float closestDistance = FLT_MAX;	//FLT_MAX is max float value (3.4e38f <---- (big MF number yo))

	for (auto& alien : aliens)
	{
		if (!alien->isActive())
			continue;

		float hitDistance = 0.0f;
		if (rayCollision(rayOrigin, rayDirection, alien->getAlien(), hitDistance))
		{
			if (hitDistance < closestDistance)
			{
				closestDistance = hitDistance;
				m_closestAlien = alien.get();
			}
		}
	}
	return m_closestAlien;
}



