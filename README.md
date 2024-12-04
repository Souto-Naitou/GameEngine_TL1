## [branch:main](https://github.com/Souto-Naitou/GameEngine_DX12/tree/customized03)
|Debug|Release|
|:---:|:---:|
[![DebugBuild](https://github.com/Souto-Naitou/GameEngine_DX12/actions/workflows/DebugBuildTest.yml/badge.svg)](https://github.com/Souto-Naitou/GameEngine_DX12/actions/workflows/DebugBuildTest.yml)|[![ReleaseBuild](https://github.com/Souto-Naitou/GameEngine_DX12/actions/workflows/ReleaseBuildTest.yml/badge.svg)](https://github.com/Souto-Naitou/GameEngine_DX12/actions/workflows/ReleaseBuildTest.yml)|

```mermaid

classDiagram
direction BT

Particle --* ParticleManager
Particle --o ParticleEmitter
Particle --|> VectorContainer
ParticleEmitter --* ParticleManager

class ParticleManager

class ParticleEmitter

class Particle{
    +Initialize(ParticleSystem* std::string) void
    +Draw() void
    +Update() void
    +reserve(uint32_t) void
    -Transform[] transforms
}

class VectorContainer{
    +VectorContainer()
    +~VectorContainer()
    +reserve(uint32_t) void
    #std::vector~T~ container_
}

```
