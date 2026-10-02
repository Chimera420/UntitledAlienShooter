#pragma once
#include "raylib.h"
#include "Weapon.h"

class Ui
{
public:
	Ui();
	~Ui();

	void drawUi();

	void getAbilityCount(int abilityCount);
	void getAmmoCount(int ammoCount);
	void getCurrentWeapon(WeaponType currentWep);
private:
	WeaponType m_playerCurrentWep;

	int m_playerHp;
	int m_playerAmmo;
	int m_playerAbilityCount;
};