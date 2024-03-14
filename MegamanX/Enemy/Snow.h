#pragma once
#include "Enemy.h"
class Snow : public Enemy
{
private:

	float skillTimer = 0;
	bool onSkill = false;

public:
	Snow(const std::string& name, sf::Image& mapImage);

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void LateUpdate(float dt) override;

};

