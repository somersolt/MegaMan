#pragma once
#include "Enemy.h"

class Ostrich :
    public Enemy
{
private:

	float skillTimer = 0;
	bool onSkill = false;
	sf::Vector2f playerPos;

public:
	Ostrich(const std::string& name, sf::Image& mapImage);

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void LateUpdate(float dt) override;

	void Fire();
	void ToIdle();
};

