#include "Nu3D/Patch.h"

namespace Nu3D
{
    // GLOBAL: TOY2 0x00884480
    Patch *g_patchListHead;

    BOOL Patch::PatchVertices::CreateVertexBuffer(PatchVertices* patchVertices, int32_t flags) { return TRUE; }
}