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
		Appear,
		Idle,
		Run,
		JumpingUp,
		FallingDown,
		Landing,
		Dash,
		Climbing,
		WallJump,
		Hit,
		Die,
		Victory,
	};

protected:
	SceneGame* sceneGame = nullptr;

	Animator playerAnimation; // sprite와는 별개
	sf::RectangleShape playerHitBox;  //충돌박스 표시
	sf::FloatRect playerBounds;
	sf::Color collisionColor = { 255 , 0 , 0 }; // 충돌 색깔 - 빨간색 (바닥)
	sf::Color sideCollisionColor = { 0 , 255 , 0 }; // 충돌 색깔 - 초록색 (벽)
	sf::Color slopeCollisionColor = { 0 , 0 , 255 }; // 충돌 색깔 - 파란색 (경사)
	sf::Color fallenCollisionColor = { 255 , 255 , 0 }; // 충돌 색깔 - 노란색 (추락)

	sf::Sprite effect;
	Animator playerEffectAnimation;

	int HP = 16; 
	float speed = 200.f;
	float damageTimer = 0.f; // 무적 타이머
	sf::Vector2f velocity;
	Status preStatus = Status::None;
	Status currentStatus = Status::None;
	Sides side = Sides::None;
	Sides oppsetSide = Sides::None;
	sf::Image& MapImage;


	bool isBottomColliding = false; // 바닥면 충돌 처리
	bool isTopColliding = false; // 윗면 충돌 처리
	bool isSlopeColliding = false; // 경사로 충돌 처리

	bool isBottomLeftColliding = false; // 좌측 하단 
	bool isBottomCenterColliding = false; // 중앙 하단 
	bool isBottomRightColliding = false; // 우측 하단

	bool isTopLeftColliding = false; // 좌측 상단
	bool isTopCenterColliding = false; // 중앙 상단
	bool isTopRightColliding = false; // 우측 상단

	bool isMiddleLeftColliding = false; // 좌측 충돌 처리
	bool isMiddleLeftHighColliding = false; // 좌측 충돌 처리
	bool isMiddleLeftLowColliding = false; // 좌측 충돌 처리

	bool isMiddleRightColliding = false; // 우측 충돌 처리
	bool isMiddleRightHighColliding = false; // 우측 충돌 처리
	bool isMiddleRightLowColliding = false; // 우측 충돌 처리


	bool isBottomLeftSlopeColliding = false;
	bool isBottomRightSlopeColliding = false;
	bool isBottomSlopeColliding = false;

	bool isDash = false; // 대시
	bool isJump = false; // 점프

	bool isGrounded = false; // 서있는 상태
	bool isSlopeGrounded = false; // 경사에 서있는 상태

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
	float maxChargeTimer = 0.f;

	float h = 0; // 방향키 입력변수
 	float gravity = 1000.f;

	void IdleAnimation();
	void ShotAnimation();


	sf::Vector2f boundingBoxWorldPos;
	sf::Vector2f boundingBoxLocalPos;

	int startX;
	int endX ;
	int startY;
	int endY;
	
	int rollBackMiddleHighLeft;
	int rollBackMiddleLowLeft;
	
	int rollBackMiddleHighRight;
	int rollBackMiddleLowRight;

	int rollBackBottomLeft;
	int rollBackBottomRight;
	int rollBackBottomCenter;

	int rollBackTopLeft;
	int rollBackTopCenter;
	int rollBackTopRight;

	int rollBackSlope;
	int rollBackSideSlope;

	bool isWait = false;
	bool isVictory = false;
	bool wind = false;
	Sides windSide = Sides::Left;

public:

	Player(const std::string& name, sf::Image& mapImage);
	~Player() override = default;

	const Status GetPreStatus() { return preStatus; }
	const Status GetCurrentStatus() {return currentStatus;}

	void SetPlayerStatus(Status status);
	const float GetDamageTimer() { return damageTimer; }
	void SetDamageTimer(float t) { damageTimer = t; }
	const sf::FloatRect GetPlayerBounds() { return playerBounds; }
	const int GetPlayerLife() { return HP; }
	void SetWait(bool s) { isWait = s; }
	void SetWind(bool s) { wind = s; }
	void SetWindSide(Sides s) { windSide = s; }

	void Shoot();
	void ChargeShoot();
	void MaxChargeShoot();

	void Init() override;
	void Reset() override;
	void Update(float dt) override;

	void UpdateAppear(float dt);
	void UpdateIdle(float dt);
	void UpdateRun(float dt);
	void UpdateJumpingUp(float dt);
	void UpdateFallingDown(float dt);
	void UpdateLanding(float dt);
	void UpdateDash(float dt);
	void UpdateClimbing(float dt);
	void UpdateWallJump(float dt);
	void UpdateHit(float dt);
	void UpdateDie(float dt);
	void UpdateVictory(float dt);

	void OnDamage(int damage);

	void LateUpdate(float dt) override;
	void FixedUpdate(float dt) override;

	void CheckBottomCollision();
	void CheckRightCollision();
	void CheckLeftCollision();
	void CheckTopCollision();
	void CheckSlopeCollision();
	void CheckFallenCollision();

	void Draw(sf::RenderWindow& window) override;
};

