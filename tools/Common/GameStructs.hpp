#pragma once

#include <cstdint>
#include <iostream>

#include <directx7/d3d.h>
#include <directx7/d3dtypes.h>

struct InputMapping
{
	const char* name;
	int32_t id;
};

struct GameControl
{
	int32_t id;
	const char* name;
};

// $STATIC_VAR 00500E58
inline GameControl g_gameControls[14] = {
	{ 16, "up" },
	{ 64, "down" },
	{ 128, "left" },
	{ 32, "right" },
	{ 16384, "jump" },
	{ 32768, "fire" },
	{ 8192, "spin" },
	{ 256, "camera left" },
	{ 512, "camera right" },
	{ 1024, "visor toggle" },
	{ 2048, "target lock" },
	{ 8, "menu" },
	{ 4096, "cancel" },

	// ADDED, secret menu button, not actually listed in the
	// games real control structs
	{ 1, "secret menu" },
};

constexpr size_t g_gameControlsCount = sizeof(g_gameControls) / sizeof(GameControl);

// $STATIC_VAR 004ED398
inline InputMapping g_inputMapping[] = {
	{ "esc", 1 },
	{ "1", 2 },
	{ "2", 3 },
	{ "3", 4 },
	{ "4", 5 },
	{ "5", 6 },
	{ "6", 7 },
	{ "7", 8 },
	{ "8", 9 },
	{ "9", 10 },
	{ "0", 11 },
	{ "minus", 12 },
	{ "equals", 13 },
	{ "back", 14 },
	{ "tab", 15 },
	{ "q", 16 },
	{ "w", 17 },
	{ "e", 18 },
	{ "r", 19 },
	{ "t", 20 },
	{ "y", 21 },
	{ "u", 22 },
	{ "i", 23 },
	{ "o", 24 },
	{ "p", 25 },
	{ "(", 26 },
	{ ")", 27 },
	{ "return", 28 },
	{ "lcontrol", 29 },
	{ "a", 30 },
	{ "s", 31 },
	{ "d", 32 },
	{ "f", 33 },
	{ "g", 34 },
	{ "h", 35 },
	{ "j", 36 },
	{ "k", 37 },
	{ "l", 38 },
	{ ";", 39 },
	{ "apostrophe", 40 },
	{ "grave", 41 },
	{ "lshift", 42 },
	{ "\\", 43 },
	{ "z", 44 },
	{ "x", 45 },
	{ "c", 46 },
	{ "v", 47 },
	{ "b", 48 },
	{ "n", 49 },
	{ "m", 50 },
	{ ",", 51 },
	{ ".", 52 },
	{ "/", 53 },
	{ "rshift", 54 },
	{ "multiply", 55 },
	{ "lmenu", 56 },
	{ "space", 57 },
	{ "capital", 58 },
	{ "f1", 59 },
	{ "f2", 60 },
	{ "f3", 61 },
	{ "f4", 62 },
	{ "f5", 63 },
	{ "f6", 64 },
	{ "f7", 65 },
	{ "f8", 66 },
	{ "f9", 67 },
	{ "f10", 68 },
	{ "numlock", 69 },
	{ "scroll", 70 },
	{ "pad 7", 71 },
	{ "pad 8", 72 },
	{ "pad 9", 73 },
	{ "subtract", 74 },
	{ "pad 4", 75 },
	{ "pad 5", 76 },
	{ "pad 6", 77 },
	{ "add", 78 },
	{ "pad 1", 79 },
	{ "pad 2", 80 },
	{ "pad 3", 81 },
	{ "pad 0", 82 },
	{ "pad .", 83 },
	{ "f11", 87 },
	{ "f12", 88 },
	{ "f13", 100 },
	{ "f14", 101 },
	{ "f15", 102 },
	{ "kana", 112 },
	{ "convert", 121 },
	{ "noconvert", 123 },
	{ "yen", 125 },
	{ "numpadequals", 141 },
	{ "circumflex", 144 },
	{ "at", 145 },
	{ "colon", 146 },
	{ "underline", 147 },
	{ "kanji", 148 },
	{ "stop", 149 },
	{ "ax", 150 },
	{ "unlabeled", 151 },
	{ "numpadenter", 156 },
	{ "rcontrol", 157 },
	{ "pad,", 179 },
	{ "divide", 181 },
	{ "sysrq", 183 },
	{ "rmenu", 184 },
	{ "home", 199 },
	{ "A", 200 },
	{ "prior", 201 },
	{ "C", 203 },
	{ "D", 205 },
	{ "end", 207 },
	{ "B", 208 },
	{ "next", 209 },
	{ "ins", 210 },
	{ "del", 211 },
	{ "lwin", 219 },
	{ "rwin", 220 },
	{ "apps", 221 },
	{ "joy 1", 1024 },
	{ "joy 2", 1025 },
	{ "joy 3", 1026 },
	{ "joy 4", 1027 },
	{ "joy 5", 1028 },
	{ "joy 6", 1029 },
	{ "joy 7", 1030 },
	{ "joy 8", 1031 },
	{ "joy 9", 1032 },
	{ "joy 10", 1033 },
	{ "joy 11", 1034 },
	{ "joy 12", 1035 },
	{ "joy 13", 1036 },
	{ "joy 14", 1037 },
	{ "joy 15", 1038 },
	{ "joy 16", 1039 },
	{ "joy 17", 1040 },
	{ "joy 18", 1041 },
	{ "joy 19", 1042 },
	{ "joy 20", 1043 },
	{ "joy 22", 1044 },
	{ "joy 23", 1045 },
	{ "joy 24", 1046 },
	{ "joy 25", 1047 },
	{ "joy 26", 1048 },
	{ "joy 27", 1049 },
	{ "joy 28", 1050 },
	{ "joy 29", 1051 },
	{ "pov up", 1052 },
	{ "pov right", 1053 },
	{ "pov down", 1054 },
	{ "pov left", 1055 },
	{ "direction pad", 2048 },
	{ nullptr, -1 },
};

// game types
struct Vector3I
{
	int32_t x;
	int32_t y;
	int32_t z;

	void print()
	{
		printf("[%d, %d, %d]\n", x, y, z);
	}
};

struct Vector3I16
{
	int16_t x;
	int16_t y;
	int16_t z;

	void print()
	{
		printf("[%d, %d, %d]\n", x, y, z);
	}
};

struct Toy2BuzzActor
{
	Vector3I pos;
	int16_t pitchAngle;
	int16_t yawAngle;
	int16_t rollAngle;
	int16_t primaryAnimIdx;
	int16_t creatureId;
	int16_t secondaryAnimIdx;
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
	uint16_t actorFlags;
	int16_t visibilityDistance;
	int32_t unkVar22;
	int16_t facingAngle;
	int16_t unkVar23;
	int32_t unkVar24;
	int32_t unkVar25;
	int32_t unkVar26;
	int32_t unkVar27;
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
	int16_t unkVar37;
	int16_t unkVar38;
	int16_t buzzWord0;
	int16_t health;
	int16_t buzzWord1;
	int16_t lives;
	int16_t unkShort40;
	int16_t unkShort41;
};

struct EntityControl
{
	uint8_t actorPhase;
	uint8_t unkByte;
	uint16_t actorFlags;
};

enum ActorUpdateFlags : int16_t
{
	ACTOR_UPDATE_MOVING = 0x1,
	ACTOR_UPDATE_GROUNDED = 0x2,
	ACTOR_UPDATE_ANIM_TICK = 0x4,
};

enum BehaviourTargetFlags : int16_t
{
	BEHAVIOUR_TARGET_PLAYER_IN_BOUNDS = 0x1,
};

struct CreatureRam
{
	Vector3I pos;
	uint8_t creatureId;
	uint8_t movCtrl;
	uint8_t rotSpeed;
	uint8_t initialFacingAngle;
	EntityControl entCtrl;
	int16_t boundHalfX;
	int16_t boundHalfZ;
	int16_t boundAngle;
	int16_t defenseMode;
	uint8_t latSpeedNoTarget;
	uint8_t latSpeedTarget;
	uint8_t speedNoTarget;
	uint8_t speedTarget;
};

class ActorBehaviourContext;
typedef void(__cdecl* ActorBehaviourFn)(ActorBehaviourContext* ctx);

struct Toy2Actor
{
	Vector3I pos;
	int16_t pitchAngle;
	int16_t yawAngle;
	int16_t rollAngle;
	int16_t primaryAnimIdx;
	int16_t creatureId;
	int16_t secondaryAnimIdx;
	int32_t unkVar7;
	int32_t unkVar8;
	int32_t unkVar9;
	Vector3I16 unkVector;
	int16_t unkVar10;
	int32_t unkVar12;
	int32_t unkVar13;
	int32_t unkVar14;
	int16_t unkWord1;
	int16_t unkWord2;
	int16_t unkWord3;
	int16_t unkWord4;
	uint16_t actorFlags;
	int16_t visibilityDistance;
	int32_t velX;
	int32_t gravityVel;
	int32_t velForward;
	Vector3I boundary;
	Vector3I motionTargetPos;
	int32_t targetYaw;
	int32_t unkVar28;
	int32_t unkVar29;
	int32_t unkVar30;
	int16_t unkShort1;
	int16_t unkShort2;
	int16_t damageCooldownTimer;
	int16_t actorPhase;
	uint16_t* movementData;
	int32_t unkVar34;
	int16_t unkShort3;
	int16_t unkShort4;
	int16_t unkWord15;
	int16_t unkWord16;
	int32_t lastValidYPosition;
	ActorBehaviourFn actorBehaviour;
	CreatureRam* creatureRam;
};

struct ActorBehaviourContext
{
	Toy2Actor* actor;
	ActorUpdateFlags updateFlags;
	BehaviourTargetFlags targetFlags;
	int16_t localStrafeSpeed;
	int16_t localForwardSpeed;
};

constexpr int32_t kMaxActors = 64;

struct Angles
{
	uint16_t pitch;
	uint16_t yaw;
};

struct ActiveCameraTransform
{
	Vector3I pos;
	Angles angles;
	int16_t roll;
	int16_t unkShort;
};

enum ActorFlags : uint16_t
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

struct CD3DFramework
{
	int32_t hWnd;
	int32_t bIsFullscreen;
	int32_t dwRenderWidth;
	int32_t dwRenderHeight;
	RECT rcScreenRect;
	RECT rcViewportRect;
	LPDIRECTDRAWSURFACE4 pddsFrontBuffer;
	LPDIRECTDRAWSURFACE4 pddsBackBuffer;
	LPDIRECTDRAWSURFACE4 pddsRenderTarget;
	LPDIRECTDRAWSURFACE4 pddsZBuffer;
	LPDIRECT3DDEVICE3 pd3dDevice;
	LPDIRECT3DVIEWPORT3 pvViewport;
	LPDIRECTDRAW4 pDD;
	LPDIRECT3D3 pD3D;
	D3DDEVICEDESC ddDeviceDesc;
	int32_t dwDeviceMemType;
	DDPIXELFORMAT ddpfZBuffer;
	int32_t initialized;
};


constexpr size_t g_inputMappingCount = sizeof(g_inputMapping) / sizeof(InputMapping);
