#pragma once
#include "SpriteGo.h"
#include "Animator.h"

class SceneGame;

class Player : public SpriteGo
{
public:
	enum Status
	{
		None,
		Idle,
		Run,
		JumpingUp,
		FallingDown,
		Landing,
		Dash,
		Hit,
		Die,
	};

protected:
	SceneGame* sceneGame = nullptr;

	Animator playerAnimation; // sprite와는 별개
	sf::RectangleShape playerCollision; //충돌박스 (아직 안씀)

	sf::Sprite effect;
	Animator playerEffectAnimation;

	float speed = 200.f;
	sf::Vector2f velocity;
	Status preStatus = Status::None;
	Status currentStatus = Status::None;
	Sides side = Sides::None;


	bool isCharge = false;
	bool chargeEffectMode = false;
	float chargeTimer = 0.f;


	bool isDash = false; // 대시

	bool isCantFlip = false; // 입력방지 변수
	bool isShooting = false; // 사격 모션으로 변경


	float shootTimer = 0.f;
	float shootInterval = 0.5f;

	bool isShootingMode = false; // 대시
	float ShootingModeTimer = 0.f;
	float ShootingModeInterval = 1.f;


	float h; // 방향키 입력변수
 	const float gravity = 1500.f;
		/*
		맵 만든뒤에 반드시 중력 추가
		*/

	void IdleAnimation();
	void ShotAnimation();

public:

	Player(const std::string& name = "");
	~Player() override = default;

	const Status GetPreStatus() { return preStatus; }
	const Status GetCurrentStatus() {return currentStatus;}
	void SetPlayerStatus(Status status);

	void Shoot();
	void ChargeShoot();


	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void UpdateIdle(float dt);
	void UpdateRun(float dt);
	void UpdateJumpingUp(float dt);
	void UpdateFallingDown(float dt);
	void UpdateLanding(float dt);
	void UpdateDash(float dt);
	void UpdateHit(float dt);
	void UpdateDie(float dt);

	void Draw(sf::RenderWindow& window) override;
};

