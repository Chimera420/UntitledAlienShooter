#pragma once
#include "raylib.h"
#include "Constants.h"
#include "Alien.h"
#include <vector>
#include <memory>

enum class WeaponType
{
	WEAPONTYPE_LASERPISTOL,
	WEAPONTYPE_ENERGYRIFLE,
	WEAPONTYPE_MELEE
};

enum class AttackState
{
	NOT_ATTACKING,
	MELEE_1,
	MELEE_2,
	MELEE_3
};

class Weapon
{
public:
	Weapon();
	~Weapon();

	//getters
	Alien* getClosestHitAlien(Vector2 rayOrigin, Vector2 rayDirection, std::vector<std::unique_ptr<Alien>>& aliens);
	
	Vector2 getMeleeHitbox();
	Vector2 getRayOrigin();
	Vector2 getRayDirection();

	int getAmmoCount();

	int getComboSequence();

	AttackState getCurrentAttackState();

	//setters



	//update
	void updateMeleeHitboxPosition(Vector2 playerPos, float playerAngle);

	//update timers
	void updateTimers();

	//player attacks
	void handleWeaponInput(Vector2 playerPos, Vector2 mouseWorldPos, std::vector<std::unique_ptr<Alien>>& vectorOfAliens);
	void melee();

	//knockback
	Vector2 calculateKnockbackForce(Vector2 source, Vector2 target);
	
	//weapon mechanics
	void reload();

	//draw
	void drawRay(Vector2 rayOrigin, Vector2 rayDirection);
	void drawMeleeHitbox();

	//Ammo depletion
	void ammoCountDown();

	//slab test raycast collision
	bool rayCollision(Vector2 rayOrigin, Vector2 rayDirection, Rectangle enemies, float& hitDist);
	
	//shooting bool for debug
	bool isDebugRay();

	bool isMeleeActive();
	bool isAttacking();

private:
	//weapon state
	WeaponType m_currentWeapon;

	//attack state
	AttackState m_currentAttackState;

	//aliens
	Alien* m_closestAlien;

	//timers
	Timer m_meleeAttackActive;

	//a timer that gives the player a bit of extra time between 
	//button presses to activate the next attack in the combo sequence
	Timer m_comboAttackForgiveness;

	Timer m_wait;

	//knockback
	Timer m_knockbackDuration;

	//rate of fire
	Timer m_laserPistolROF;
	Timer m_energyRifleROF;

	
	
	//bools
	bool m_isRayColliding;
	bool m_isAttacking;
	bool m_isMeleeActive;
	bool m_drawRay;

	//recs
	Rectangle m_bullet;	//bullet rectangle (will use for Projectile bullet collision)

	//vector2
	Vector2 m_position;	//bullet pos
	Vector2 m_rayOrigin;
	Vector2 m_rayDirection;
	Vector2 m_meleeHitbox; //hitbox for the punches 
	Vector2 m_meleeOrigin;

	//ints
	int m_laserPistolAmmoCount;
	int m_energyRifleAmmoCount;
	int m_meleeChargeCount;
	int m_attackSequenceCount;

};