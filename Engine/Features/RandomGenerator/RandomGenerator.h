#pragma once

#include <random>

class RandomGenerator
{
public:
    RandomGenerator(const RandomGenerator&) = delete;
    RandomGenerator& operator=(const RandomGenerator&) = delete;
    RandomGenerator(RandomGenerator&&) = delete;
    RandomGenerator& operator=(RandomGenerator&&) = delete;

    static RandomGenerator* GetInstance()
    {
        static RandomGenerator instance;
        return &instance;
    }

    void Initialize();
    void Finalize();

    template <class T>
    T Generate(T _min, T _max)
    {
        std::uniform_real_distribution<T> dist(_min, _max);
        return dist(engine_);
    }

private:
    RandomGenerator() = default;
    ~RandomGenerator() = default;

private:
    std::random_device seed_;
    std::mt19937 engine_;
};