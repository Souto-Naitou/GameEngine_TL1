#pragma once

#include <wrl.h>

template<typename T>
class BaseConfiguratorForComPtr
{
public:
    virtual void Initialize() = 0;
    T* Get() { return current_.Get(); }

protected:
    Microsoft::WRL::ComPtr<T> current_;
};