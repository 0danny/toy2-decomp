#pragma once

#include "Common.h"

namespace AudioManager
{
	extern int32_t g_curTrackIndex;

	void StopAndFlush();
	void Init();
	void SetVolumesProcessed(int32_t musicVolume, int32_t sfxVolume);
    void FlushSoundVoices();
    int32_t StopAndWait();
	void LoadSfxPackForLevel(int32_t levelId);
}