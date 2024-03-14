#pragma once
#include "Enemy.h"

class SnowThrower : public Enemy
{
private:

	float skillTimer = 0;
	bool onSkill = false;
public:
	SnowThrower(const std::string& name, sf::Image& mapImage);

	void Init() override;
	void Reset() override;
	void Update(float dt) override;

	void Throw();
};

