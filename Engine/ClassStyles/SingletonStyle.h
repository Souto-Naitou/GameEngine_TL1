#pragma once

template <typename T>
class SingletonStyle
{
public:
    SingletonStyle(const SingletonStyle&) = delete;
    SingletonStyle& operator=(const SingletonStyle&) = delete;
    SingletonStyle(SingletonStyle&&) = delete;
    SingletonStyle& operator=(SingletonStyle&&) = delete;

    static T* GetInstance()
    {
        static T instance;
        return &instance;
    }

protected:
    SingletonStyle() = default;
    ~SingletonStyle() = default;
};