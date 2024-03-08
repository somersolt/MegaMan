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
	sf::RectangleShape playerHitBox;  //충돌박스 표시
	sf::FloatRect playerBounds;
	sf::Color collisionColor = { 255 , 0 , 0 }; // 충돌 색깔 - 빨간색!
	sf::Color UnCollisionColor = { 0 , 0 , 255 }; // 충돌 색깔 - 빨간색!

	sf::Sprite effect;
	Animator playerEffectAnimation;

	float speed = 200.f;
	sf::Vector2f velocity;
	Status preStatus = Status::None;
	Status currentStatus = Status::None;
	Sides side = Sides::None;
	sf::Image MapImage;

	sf::Vector2f preposition;
	bool PreColliding = false; // 이전 충돌 처리
	bool isColliding = false; // 충돌 처리
	bool isSideColliding = false; // 충돌 처리

	bool isDash = false; // 대시

	bool isGrounded = false; // 서있는 상태
	bool PreGrounded = false; // 이전 프레임에 서있는 상태

	bool isCantFlip = false; // 방향변경 불가 변수
	
	bool isShooting = false; // 사격 상태 변수
	float shootTimer = 0.f;
	float shootInterval = 0.5f;

	bool isShootingMode = false; // 사격 모션으로 변경
	float ShootingModeTimer = 0.f;
	float ShootingModeInterval = 1.f;

	bool isCharge = false; // 차지 사격 변수
	bool chargeEffectMode = false;
	float chargeTimer = 0.f;


	float h = 0; // 방향키 입력변수
 	const float gravity = 1000.f;
		/*
		맵 만든뒤에 반드시 중력 추가
		*/

	void IdleAnimation();
	void ShotAnimation();


	sf::Vector2f boundingBoxWorldPos;
	sf::Vector2f boundingBoxLocalPos;

	int startX;
	int endX ;
	int startY;
	int endY;



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

	void LateUpdate(float dt) override;
	void FixedUpdate(float dt) override;

	void CheckBottomCollision();
	void CheckRightCollision();

	void Draw(sf::RenderWindow& window) override;
};

