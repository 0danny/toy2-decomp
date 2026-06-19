#include "AudioManager/AudioManager.h"

namespace AudioManager
{
	// GLOBAL: TOY2 005282CC
	int32_t g_curTrackIndex;

	// STUB: TOY2 0x0047D840
	void StopAndFlush() {}

	// STUB: TOY2 0x0047EDE0
	void Init() {}

	// STUB: TOY2 0x0049E8D0
	void SetVolumesProcessed(int32_t musicVolume, int32_t sfxVolume) {}

	// STUB: TOY2 0x004A3EF0
	void FlushSoundVoices() {}

	// STUB: TOY2 0x00436D40
	int32_t StopAndWait() { return 1; }

	// STUB: TOY2 0x0047EC20
	void LoadSfxPackForLevel(int32_t levelId) {}

	// STUB: TOY2 0x004A37E0
	int32_t PlayOneShotSoundGlobal(int32_t soundIndex, int32_t volume, int32_t leftVolume, int32_t rightVolume) { return 0; }

	// STUB: TOY2 0x0047D7F0
	void PlayMusicOneShot(int32_t trackIndex) {}
}