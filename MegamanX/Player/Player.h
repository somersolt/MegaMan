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

	Animator playerAnimation; // sprite�ʹ� ����
	sf::RectangleShape playerHitBox;  //�浹�ڽ� ǥ��
	sf::FloatRect playerBounds;
	sf::Color collisionColor = { 255 , 0 , 0 }; // �浹 ���� - ������ (�ٴ�)
	sf::Color sideCollisionColor = { 0 , 255 , 0 }; // �浹 ���� - �ʷϻ� (��)
	sf::Color slopeCollisionColor = { 0 , 0 , 255 }; // �浹 ���� - �Ķ��� (���)
	sf::Color fallenCollisionColor = { 255 , 255 , 0 }; // �浹 ���� - ����� (�߶�)

	sf::Sprite effect;
	Animator playerEffectAnimation;

	int HP = 16; 
	float speed = 200.f;
	float damageTimer = 0.f; // ���� Ÿ�̸�
	sf::Vector2f velocity;
	Status preStatus = Status::None;
	Status currentStatus = Status::None;
	Sides side = Sides::None;
	Sides oppsetSide = Sides::None;
	sf::Image& MapImage;


	bool isBottomColliding = false; // �ٴڸ� �浹 ó��
	bool isTopColliding = false; // ���� �浹 ó��
	bool isSlopeColliding = false; // ���� �浹 ó��

	bool isBottomLeftColliding = false; // ���� �ϴ� 
	bool isBottomCenterColliding = false; // �߾� �ϴ� 
	bool isBottomRightColliding = false; // ���� �ϴ�

	bool isTopLeftColliding = false; // ���� ���
	bool isTopCenterColliding = false; // �߾� ���
	bool isTopRightColliding = false; // ���� ���

	bool isMiddleLeftColliding = false; // ���� �浹 ó��
	bool isMiddleLeftHighColliding = false; // ���� �浹 ó��
	bool isMiddleLeftLowColliding = false; // ���� �浹 ó��

	bool isMiddleRightColliding = false; // ���� �浹 ó��
	bool isMiddleRightHighColliding = false; // ���� �浹 ó��
	bool isMiddleRightLowColliding = false; // ���� �浹 ó��


	bool isBottomLeftSlopeColliding = false;
	bool isBottomRightSlopeColliding = false;
	bool isBottomSlopeColliding = false;

	bool isDash = false; // ���
	bool isJump = false; // ����

	bool isGrounded = false; // ���ִ� ����
	bool isSlopeGrounded = false; // ��翡 ���ִ� ����

	bool isCantFlip = false; // ���⺯�� �Ұ� ����
	
	bool isShooting = false; // ��� ���� ����
	float shootTimer = 0.f;
	float shootInterval = 0.5f;

	bool isShootingMode = false; // ��� ������� ����
	float ShootingModeTimer = 0.f;
	float ShootingModeInterval = 1.f;

	bool isCharge = false; // ���� ��� ����
	bool chargeEffectMode = false;
	float chargeTimer = 0.f;
	float maxChargeTimer = 0.f;

	float h = 0; // ����Ű �Էº���
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

