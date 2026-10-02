#include "Ui.h"

Ui::Ui() :
	m_playerCurrentWep(),
	m_playerHp(),
	m_playerAmmo(),
	m_playerAbilityCount()
{
	m_playerHp = 100;
	m_playerAmmo;
	m_playerAbilityCount = 2;
}

Ui::~Ui()
{

}

void Ui::drawUi()
{
	//UI Backdrops
	DrawRectangle(0, GetScreenHeight() - 80, 200, 80, ColorAlpha(GRAY, 0.6f));

	//Ability, player, and weapon status
	if (m_playerAbilityCount == 2)
	{
		DrawCircle(GetScreenWidth() / 2 - 15, GetScreenHeight() / 2 + 50, 10.0f, WHITE);	//ability charges
		DrawCircle(GetScreenWidth() / 2 + 15, GetScreenHeight() / 2 + 50, 10.0f, WHITE);
	}
	if (m_playerAbilityCount == 1)
	{
		DrawCircle(GetScreenWidth() / 2 - 15, GetScreenHeight() / 2 + 50, 10.0f, WHITE);	//ability charges
		DrawCircleLines(GetScreenWidth() / 2 + 15, GetScreenHeight() / 2 + 50, 10.0f, WHITE);
	}
	if (m_playerAbilityCount == 0)
	{
		DrawCircleLines(GetScreenWidth() / 2 - 15, GetScreenHeight() / 2 + 50, 10.0f, WHITE);	//ability charges
		DrawCircleLines(GetScreenWidth() / 2 + 15, GetScreenHeight() / 2 + 50, 10.0f, WHITE);
	}

	DrawText(TextFormat("Ammo: %i", m_playerAmmo), 10, GetScreenHeight() - 40, 25, WHITE);	//ammo count
	DrawText(TextFormat("Health: %i", m_playerHp), 10, GetScreenHeight() - 70, 25, WHITE);	//health 

	if (m_playerAmmo < 5 && m_playerAmmo > 0)
	{
		DrawText("LOW AMMO", GetMouseX() - 45.0f, GetMouseY() - 25.0f, 15, LIME);
	}

	else if (m_playerAmmo <= 0)
	{
		DrawText("OUT OF AMMO! \n(R) TO RELOAD", GetMouseX() - 45.0f, GetMouseY() - 25.0f, 15, LIME);
	}
}

void Ui::getAbilityCount(int abiltyCount)
{
	m_playerAbilityCount = abiltyCount;
}

void Ui::getAmmoCount(int ammoCount)
{
	m_playerAmmo = ammoCount;
}
