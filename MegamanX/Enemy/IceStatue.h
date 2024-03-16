#pragma once
#include "Enemy.h"

class Boss;

class IceStatue : public Enemy
{

private:

	float skillTimer = 0;
	bool onSkill = false;
	Boss* boss = nullptr;


public:
	IceStatue(const std::string& name, sf::Image& mapImage);

	void Init() override;
	void Reset(int g);
	void Update(float dt) override;
	void LateUpdate(float dt) override;

};

