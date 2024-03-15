#pragma once
#include "Enemy.h"
class Bat : public Enemy
{

public:
	enum Status
	{
		None,
		Idle,
		Follow,
		GoBack,
	};
private:

	float skillTimer = 0;
	bool onSkill = false;
	bool goHome = false;
	Status status = Status::None;
	sf::Vector2f spawn;
	sf::Vector2f playerPos;
	sf::Vector2f dir;

public:
	Bat(const std::string& name, sf::Image& mapImage);

	void Init() override;
	void Reset(sf::Vector2f s);
	void Update(float dt) override;
	void LateUpdate(float dt) override;

};
