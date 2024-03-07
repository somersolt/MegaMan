#pragma once
#include "SpriteGo.h"

class SceneGame; // 충돌처리용 전방선언

class Buster : public SpriteGo
{
protected:
	Animator busterAnimation;

	sf::Vector2f direction;
	float speed;
	int damage = 1;

	SceneGame* sceneGame;

	float timer = 0.f;
	float duration = 3.f;
	
public:
	Buster(const std::string& name = "");
	~Buster() override = default;

	void Fire(const sf::Vector2f dir, float s, int d, Sides side);
	void ChargeFire(const sf::Vector2f dir, float s, int d, Sides side);

	void Init() override;
	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Reset() override;

};

