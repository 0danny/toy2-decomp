#pragma once

#include <windows.h>
#include <stdio.h>
#include <iostream>

// Typedefs
typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef __int64 int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned __int64 uint64_t;

typedef float float32_t;
typedef double float64_t;

// game types
struct Vector3I
{
	int32_t x;
	int32_t y;
	int32_t z;

	void print()
	{
		printf("[%d, %d, %d]\n", x, y, z);
	};
};

enum ActorFlags
{
	ACTOR_CUTSCENE_MOVE = 0x1,
	ACTOR_VISIBLE_ACTIVE = 0x2,
	ACTOR_LOCKED_MOVE = 0x4,
	ACTOR_GROUNDED = 0x8,
	ACTOR_FLOOR_SNAP = 0x10,
	ACTOR_STUNNED = 0x20,
	ACTOR_OOB_SPECIAL_MOVE = 0x40,
	ACTOR_DEATH_DAMAGE = 0x80,
	ACTOR_AIRBORNE_SPECIAL = 0x100,
	ACTOR_PLAYER_TRIGGER = 0x200,
	ACTOR_VEL_CLAMP_OVERRIDE = 0x400,
	ACTOR_NO_DESPAWN = 0x800,
	ACTOR_UNUSED_1000 = 0x1000,
	ACTOR_FORCE_REMOVE = 0x2000,
	ACTOR_CAMERA_HOVER = 0x4000,
	ACTOR_UNKNOWN_8000 = 0x8000,
};


struct CreatureListRam
{
	Vector3I pos;
	uint8_t creatureId;
	uint8_t movCtrl;
	uint8_t rotSpeed;
	uint8_t collectableId;
	int32_t entCtrl;
	int16_t boundHalfX;
	int16_t boundHalfZ;
	int16_t unk;
	int16_t defenseMode;
	uint8_t latSpeedNoTarget;
	uint8_t latSpeedTarget;
	uint8_t speedNoTarget;
	uint8_t speedTarget;
};

struct Toy2BaseActor
{
	Vector3I pos;
	int16_t pitchAngle;
	int16_t yawAngle;
	int16_t rollAngle;
	int16_t unkWord1_1;
	int16_t creatureId;
	int16_t unkWord18;
	int32_t unkVar7;
	int32_t unkVar8;
	int32_t unkVar9;
	int32_t unkVar10;
	int32_t unkVar11;
	int32_t unkVar12;
	int32_t unkVar13;
	int32_t unkVar14;
	int16_t unkWord1;
	int16_t unkWord2;
	int16_t unkWord3;
	int16_t unkWord4;
	int16_t actorFlags;
	int16_t visibilityDistance;
	Vector3I velocity;
	Vector3I boundary;
	Vector3I motionTargetPos;
	int32_t targetYaw;
	int32_t unkVar28;
	int32_t unkVar29;
	int32_t unkVar30;
	int32_t unkVar31;
	int16_t unkWord13;
	int16_t actorHealth;
	int32_t unkVar33;
	int32_t unkVar34;
	int32_t unkVar35;
	int16_t unkWord15;
	int16_t unkWord16;
	int32_t unkVar37;
};

struct Toy2Actor
{
	Toy2BaseActor base;
	int32_t unkVar38;
	CreatureListRam* creatureRam;
};


struct Toy2BuzzActor
{
	Toy2BaseActor base;
	int16_t buzzWord0;
	int16_t health;
	int16_t buzzWord1;
	int16_t lives;
	int16_t unkShort40;
	int16_t unkShort41;
};
