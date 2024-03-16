#pragma once
#include "Enemy.h"
class BossShot : public Enemy
{
private:

	float skillTimer = 0;
	bool onSkill = false;

public:
	BossShot(const std::string& name, sf::Image& mapImage);

	void Init() override;
	void Reset(int g);
	void Update(float dt) override;
	void LateUpdate(float dt) override;

};

