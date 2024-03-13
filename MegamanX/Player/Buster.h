#pragma once
#include "SpriteGo.h"

class SceneGame; // 충돌처리용 전방선언

class Buster : public SpriteGo
{
protected:
	Animator busterAnimation;

	sf::Vector2f direction;
	float speed = 300;
	int damage = 1;
	bool hit = false;
	SceneGame* sceneGame = nullptr;

	float timer = 0.f;
	float duration = 3.f;
	
public:
	Buster(const std::string& name = "");
	~Buster() override = default;

	void Fire(const sf::Vector2f dir, float s, int d, Sides side);
	void ChargeFire(const sf::Vector2f dir, float s, int d, Sides side);

	void Release() override;

	void Init() override;
	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Reset() override;

};

