#include "Tween.h"
#include "Constants.h"
#include "raylib.h"
#include "raymath.h"

Tween::Tween(float& reference) :
	m_isRunning(false),
	m_duration(),
	m_elapsed(),
	m_start(),
	m_target(),
	m_reference(reference)
{
}

Tween::~Tween()
{

}
