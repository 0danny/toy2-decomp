#pragma once

#include "Common.h"
#include "Nu3D/Primitive.h"
#include <STDIO.H>

namespace NGNLoader
{
	namespace ObjectLoad
	{
		Nu3D::Primitive* ExtractShapeData(FILE* stream);
		Nu3D::Material* GetCurrentMatByIndex(uint32_t index);
	}
}