#pragma once
#include "Enemy.h"
class Boss :
    public Enemy
{
public:
	enum BossStatus
	{
		Appear,
		Idle,
		JumpingUp,
		FallingDown,
		Sliding,
		Breath,
		Hanging,
		Hit,
		Die,
	};
private:
	Sides side = Sides::Left;
	float skillTimer = 3;
	float boomTimer = 0.5;
	float dieTimer = 0;
	float hitTimer = 1;
	bool onSkill = false;
	bool isappear = false;
	bool isSuperArmor = false;

	sf::Vector2f playerPos;
	BossStatus bossCurrentStatus = BossStatus::Appear;

public:
	Boss(const std::string& name, sf::Image& mapImage);

	void Init() override;
	void Reset() override;
	void Update(float dt) override;

	void UpdateAppear(float dt);
	void UpdateIdle(float dt);
	void UpdateJumpingUp(float dt);
	void UpdateFallingDown(float dt);
	void UpdateSliding(float dt);
	void UpdateBreath(float dt);
	void UpdateHanging(float dt);
	void UpdateHit(float dt);
	void UpdateDie(float dt);

	void OnDamage(int damage) override;

	void LateUpdate(float dt) override;
	void FixedUpdate(float dt) override;
	void SetBossStatus(BossStatus b) { bossCurrentStatus = b; }
	const int GetBossLife() { return Hp; }

	void ToIdle();
	void Shot();
	void Jump();

};

