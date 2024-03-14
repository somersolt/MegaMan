#pragma once
#include "SpriteGo.h"

class SceneGame; // 충돌처리용 전방선언
class Player;

class EnemyShot : public SpriteGo
{
protected:
	Animator EnemyShotAnimation;
	std::string animationId;
	sf::Vector2f direction;
	float speed = 300;
	int damage = 1;
	bool hit = false;
	SceneGame* sceneGame = nullptr;
	Player* player = nullptr;

	float timer = 0.f;
	float duration = 3.f;

public:
	EnemyShot(const std::string& name = "");
	~EnemyShot() override = default;

	void EnemyFire(const sf::Vector2f dir, float s, int d, int h);

	void Release() override;

	void Init(std::string aniId , std::string texId);
	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Reset() override;
};

