#pragma once

#include "IOrderExecuter.h"

class OESendModelList : public IOrderExecuter
{
public:
    OESendModelList() = default;
    ~OESendModelList() override = default;
    void Run() override;
    std::string Serialize() const override;
};