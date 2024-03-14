#pragma once
#include "Enemy.h"
class Rabbit : public Enemy
{
private:

	float skillTimer = 0;
	bool onSkill = false;

public:
	Rabbit(const std::string& name, sf::Image& mapImage);

	void Init() override;
	void Reset() override;
	void Update(float dt) override;

	void Jump();
	void Fire();

};

