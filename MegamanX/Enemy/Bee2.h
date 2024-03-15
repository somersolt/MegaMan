#pragma once
#include "Enemy.h"
class Bee2 :
    public Enemy
{
public:
	enum Status
	{
		None,
		Far,
		Medium,
		Close,
	};
private:

	float skillTimer = 0;
	bool onSkill = false;
	Status status = Status::None;
	sf::Vector2f playerPos;
	sf::Vector2f dir;

public:
	Bee2(const std::string& name, sf::Image& mapImage);

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void LateUpdate(float dt) override;

};

