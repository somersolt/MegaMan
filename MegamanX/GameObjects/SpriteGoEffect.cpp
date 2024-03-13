#include "pch.h"
#include "SpriteGoEffect.h"

SpriteGoEffect::SpriteGoEffect(const std::string& name) : SpriteGo(name)
{
}

SpriteGoEffect::~SpriteGoEffect()
{
}

void SpriteGoEffect::Init()
{
	effectAnimation.SetTarget(&sprite);

	SpriteGo::Init();
}

void SpriteGoEffect::Reset()
{
	timer = 0.f;
	effectAnimation.Play("animations/BoomEffect.csv");
}

void SpriteGoEffect::Update(float dt)
{
	effectAnimation.Update(dt);
	timer += dt;
	if (timer > duration)
	{
		SetActive(false);
		SCENE_MGR.GetCurrentScene()->RemoveGo(this);
	}
}
