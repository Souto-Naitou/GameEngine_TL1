#pragma once

// コンポーネントデータ
// コンポーネントのデータを保持する構造体です。
// この構造体はUIクラスの内部で使用されます。


#include <string> // string
#include "../math/NiVec2.h" // NiVec2
#include "NiUI_Enum.h" // enums


// フレックスコンテナのデータ
struct FlexContainerData
{
    NiVec2 leftTop;
    NiVec2 size;
    NiVec2 padding;
    NiVec2 margin;
    NiUI_Flex_Direction direction;
    NiUI_Flex_JustifyContent justifyContent;
    NiUI_Flex_AlignItems alignItems;
    NiUI_Flex_AlignContent alignContent;
};


// ボタンデータ
struct ButtonImageData
{
    std::string textureName;
    NiVec2 leftTop;
    NiVec2 size;

    /// ボタンの状態
    bool isHeld;
    bool isHover;
};