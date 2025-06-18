#include "./ResourceStateTracker.h"

void ResourceStateTracker::Reset()
{
    resource.Reset();
    state = D3D12_RESOURCE_STATE_COMMON;
}