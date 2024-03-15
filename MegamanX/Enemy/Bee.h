#pragma once
#include "Enemy.h"
class Bee : public Enemy
{

public:
	enum Status
	{
		None,
		Idle,
	};
private:

	float skillTimer = 0;
	bool onSkill = false;
	sf::Vector2f playerPos;
	sf::Vector2f dir = { -1 ,0 };

public:
	Bee(const std::string& name, sf::Image& mapImage);

	void Init() override;
	void Reset();
	void Update(float dt) override;
	void LateUpdate(float dt) override;

	void Drop(int h);

};

