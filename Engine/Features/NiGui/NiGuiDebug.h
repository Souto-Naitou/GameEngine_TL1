#pragma once

#include <NiGui/Interface/NiGui_IDebug.h>

class NiGuiDebug : public INiGuiDebug
{
public:
    NiGuiDebug() = default;
    void DrawDebugUI();
};