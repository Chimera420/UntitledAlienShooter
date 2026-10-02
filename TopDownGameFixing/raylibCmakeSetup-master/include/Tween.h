#pragma once
#include "raylib.h"

class Tween
{
public:
	Tween(float& reference);
	~Tween();
private:
	bool m_isRunning;

	float m_duration;
	float m_elapsed;
	float m_start;
	float m_target;
	float& m_reference;
};