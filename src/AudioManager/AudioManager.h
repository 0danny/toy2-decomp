#pragma once

#include "Common.h"

namespace AudioManager
{
	void StopAndFlush();
	void Init();
	void SetVolumesProcessed(int32_t musicVolume, int32_t sfxVolume);
    void FlushSoundVoices();
    int32_t StopAndWait();
}