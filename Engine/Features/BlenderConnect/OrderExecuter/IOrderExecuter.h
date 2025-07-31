#pragma once
#include <string>

class IOrderExecuter
{
public:
    virtual ~IOrderExecuter() = default;
    virtual void Run() = 0;
    virtual std::string Serialize() const = 0;
    virtual bool IsNeedResponse() const = 0;
};