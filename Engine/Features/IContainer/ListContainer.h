#pragma once

#include <list>

template <typename T>
class ListContainer
{
public:
    ListContainer() = default;
    ListContainer(const ListContainer& _container) = default;
    ListContainer(ListContainer&& _container) = default;


public:
    void push_back(const T& _val) { container_.push_back(_val); }
    void emplace_back(const T& _val) { container_.emplace_back(_val); }
    uint32_t size() const { return container_.size(); }
    constexpr void reserve(uint32_t _size) { container_.reserve(_size); }
    constexpr void resize(uint32_t _size) { container_.resize(_size); }
    void reserve(uint32_t _size) { container_.reserve(_size); }
    void resize(uint32_t _size) { container_.resize(_size); }
    void clear() { container_.clear(); }


public: /// access elements
    T& operator[](std::size_t _index);
    const T& operator[](std::size_t _index) const;

    auto begin() { return container_.begin(); }
    auto end() { return container_.end(); }
    const auto begin() const { return container_.begin(); }
    const auto end() const { return container_.end(); }

    const auto cbegin() const { return container_.cbegin(); }
    const auto cend() const { return container_.cend(); }

    T& front() { return container_.front(); }
    T& back() { return container_.back(); }


public:
    std::list<T> container_ = {};
};

template<typename T>
inline T& ListContainer<T>::operator[](std::size_t _index)
{
    auto itr = container_.begin();
    for (uint32_t index = 0; index < _index; ++index) ++itr;
    return *itr;
}

template<typename T>
inline const T& ListContainer<T>::operator[](std::size_t _index) const
{
    auto itr = container_.cbegin();
    for (uint32_t index = 0; index < _index; ++index) ++itr;
    return *itr;
}
