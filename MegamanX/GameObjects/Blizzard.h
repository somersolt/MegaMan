#pragma once
#include "SpriteGo.h"
class Blizzard :
    public SpriteGo
{
protected:
    Animator effectAnimation;
    float duration = 2.f;
    float timer = 0.f;
public:
    Blizzard(const std::string& name = "");
    ~Blizzard();

    void Init() override;
    void Reset() override;
    void Update(float dt) override;
};

