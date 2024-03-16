#include "pch.h"
#include "Blizzard.h"

Blizzard::Blizzard(const std::string& name) : SpriteGo(name)
{
}

Blizzard::~Blizzard()
{
}

void Blizzard::Init()
{
	SetTexture("graphics/boss/blizzard");
	effectAnimation.SetTarget(&sprite);

	SpriteGo::Init();
}

void Blizzard::Reset()
{
	timer = 0.f;
	effectAnimation.Play("animations/boss/Blizzard.csv");
}

void Blizzard::Update(float dt)
{
	effectAnimation.Update(dt);
	timer += dt;
	if (timer > duration)
	{
		SetActive(false);
		SCENE_MGR.GetCurrentScene()->RemoveGo(this);
	}
}
