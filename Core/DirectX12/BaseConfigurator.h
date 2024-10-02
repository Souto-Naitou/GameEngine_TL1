#pragma once

template<typename T>
class BaseConfigurator
{
public:

    /// <summary>
    /// 変更を予約する
    /// </summary>
    /// <param name="_setting">モード</param>
    inline void ReserveMode(T _mode)
    {
        reservedMode_ = _mode;
    }

    /// <summary>
    /// 変更を適用する
    /// </summary>
    /// <param name="_mode">モード</param>
    virtual void ApplyMode(T _mode) = 0;


protected:
    T currentMode_  = {};
    T reservedMode_ = {};
};