#pragma once

//-----------------------------------------
//CONSTANTS
//-----------------------------------------

//decay rate
const float KNOCKBACK_VELOCITY_DECAY = 0.8f;

//speed
const float PLAYER_WALK_SPEED = 8.0f;
const float PLAYER_ATTACK_WALK_SPEED = 1.0f;
const float PLAYER_SPRINT_SPEED = 180.0f;
const float PLAYER_DODGE_SPEED = 1.1f;
const float BULLET_SPEED = 5000.0f;
const float ALIEN_SPEED = 1.0f;

//fire rate
const float LASER_PISTOL_ROF = 0.16f;
const float ENERGY_RIFLE_ROF = 0.06f;
const float MELEE_ATTACK_ROF = 0.35f;

//weapon damage
const int LASER_PISTOL_DAMAGE = 50;
const int ENRGY_RIFLE_DAMAGE = 32;
const int MELEE_DAMAGE = 150;

//combo
const float NEXT_COMBO_ATTACK_FORGIVENESS = 1.25f;
const float WAIT_TIME = 0.3f;
const float	MAGNET_OFFSET = 10.0f;
const float	MAGNETIZE_DURATION = 2.0f;

//knockback
const float  MELEE_1_KNOCKBACK_FORCE = 50.0f;
const float  MELEE_2_KNOCKBACK_FORCE = 100.0f;
const float  MELEE_3_KNOCKBACK_FORCE = 5000.0f;

const float KNOCKBACK_DURATION_MELEE_1 = 0.5;
const float KNOCKBACK_DURATION_MELEE_2 = 0.8;
const float KNOCKBACK_DURATION_MELEE_3 = 1.5;

//hitbox
const float PUNCH_RADIUS = 12;

//health
const int LARVAE_HP = 100;
const int SPEED_ALIEN_HP = 450;
const int BOSS_ALIEN_HP = 6000;

//vectors
const int BULLET_POOL_SIZE = 12; 
const int ALIEN_POOL_SIZE = 50;
const int MAX_LARVAE = 15;
const int MAX_SPEED_ALIEN = 3;
const int MAX_BOSS_ALIEN = 2;
const int MELEE_HITBOX_OFFSET = 22;

//camera
const float CAMERA_DAMPING_X = 1.0f;
const float CAMERA_DAMPING_Y = 1.0f;
const float CAMERA_DAMPING = 2.0f;