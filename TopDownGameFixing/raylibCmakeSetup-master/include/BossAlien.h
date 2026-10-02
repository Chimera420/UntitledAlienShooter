#pragma once
#include "raylib.h"
#include "Alien.h"

class BossAlien : public Alien
{
public:
	BossAlien(int iD, float x, float y);
	~BossAlien();
private:
};