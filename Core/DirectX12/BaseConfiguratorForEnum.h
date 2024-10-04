#pragma once

#include <BaseConfigurator.h>

template<typename EnumType, typename DataType>
class BaseConfiguratorForEnum : public BaseConfigurator<DataType>
{
public:

    /// <summary>
    /// 変更を適用する
    /// </summary>
    /// <param name="_mode">モード</param>
    virtual void Apply(EnumType _mode) = 0;

protected:
    EnumType mode_ = {};
};