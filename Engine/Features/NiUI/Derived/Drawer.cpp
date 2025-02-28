#include "Drawer.h"

void Drawer::DrawSetting()
{
    IDrawer::DrawSetting();

    /// スプライトの数が足りない場合は追加
    if(buttonDrawDataQueue_.size() > sprites_.size())
    {
        sprites_.resize(buttonDrawDataQueue_.size());
        for(auto& sprite : sprites_)
        {
            sprite = std::make_unique<Sprite>();
        }
    }

    /// ボタンの描画データをスプライトに変換
    auto itr_sprites = sprites_.begin();
    for(auto& data : buttonDrawDataQueue_)
    {
        auto sprite = itr_sprites->get();
        sprite->Initialize(data->textureName);
        sprite->SetPosition(Vector2(data->leftTop.x, data->leftTop.y));
        sprite->SetSize(Vector2(data->size.x, data->size.y));

        auto& color = sprite->GetColor();
        Vector3 color3 = color.xyz();

        if(data->isHeld)
        {
            color3 -= Vector3(0.3f, 0.3f, 0.3f);
            sprite->SetColor({ color3 , 1.0f });
        }
        else if(data->isHover)
        {
            color3 -= Vector3(0.15f, 0.15f, 0.15f);
            sprite->SetColor({ color3 , 1.0f });
        }
        else
        {
            sprite->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
        }

        ++itr_sprites;
    }

    return;
}

void Drawer::Draw()
{
    for(auto& sprite : sprites_)
    {
        sprite->Update();
        sprite->Draw();
    }

    /// 描画データのクリア
    buttonDrawDataQueue_.clear();

    return;
}