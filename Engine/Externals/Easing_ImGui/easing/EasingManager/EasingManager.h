// Copyright © 2024 Souto-Naitou. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for license information.


#ifndef EASING_MANAGER_H_
#define EASING_MANAGER_H_

#include <list>
#include <string>

class Easing;

class EasingManager
{
public:
    EasingManager() = default;
    ~EasingManager() = default;

    EasingManager(const EasingManager&) = delete;
    EasingManager& operator=(const EasingManager&) = delete;
    EasingManager(EasingManager&&) = delete;
    EasingManager& operator=(const EasingManager&&) = delete;

    static EasingManager* GetInstance() { static EasingManager instance; return &instance; }

    void SetEasing(Easing* _easing);
    void DeleteEasing(Easing* _target);
    Easing* GetEasing(std::string _ID);

    void DrawUI();

private:
    std::list<Easing*> pEasings = {};
    bool isFirstFrame_ = true;
};


#endif // EASING_MANAGER_H_