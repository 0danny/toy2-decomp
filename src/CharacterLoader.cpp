#include "CharacterLoader.h"

namespace CharacterLoader
{
	// GLOBAL: TOY2 0x0053EEE0
	BoneTransform g_boneTransforms[300];

	// GLOBAL: TOY2 0x0054717C
	int16_t g_unk54717C;

	// GLOBAL: TOY2 0x00546D78
	int16_t g_unk546D78;

	// GLOBAL: TOY2 0x00547CD0
	int16_t g_unk547CD0;

	// GLOBAL: TOY2 0x0053E4C8
	int32_t g_alternateAllParse[128];

	// GLOBAL: TOY2 0x0053E8C8
	uint8_t* g_charFileDataCache[128];

	// GLOBAL: TOY2 0x0043B9B0
	void Start(int32_t* value, uint8_t** buffer, uint8_t* creatureList) {}
}