#pragma once

#include "Common/GameStructs.hpp"

inline auto g_levelActors = reinterpret_cast<Toy2Actor*>(0x0052C840);
inline auto g_buzzActor = reinterpret_cast<Toy2BuzzActor*>(0x0052F300);
inline auto g_activeCameraTransform = reinterpret_cast<ActiveCameraTransform*>(0x0052ADC0);
