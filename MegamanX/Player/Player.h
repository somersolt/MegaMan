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

	Animator playerAnimation; // sprite�ʹ� ����
	sf::RectangleShape playerHitBox;  //�浹�ڽ� ǥ��
	sf::FloatRect playerBounds;
	sf::Color collisionColor = { 255 , 0 , 0 }; // �浹 ���� - ������!

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


	bool isDash = false; // ���
	bool isGrounded = false; // ���ִ� ����
	bool isColliding = false; // �浹 ó��

	bool isCantFlip = false; // �Է¹��� ����
	bool isShooting = false; // ��� ������� ����


	float shootTimer = 0.f;
	float shootInterval = 0.5f;

	bool isShootingMode = false; // ���
	float ShootingModeTimer = 0.f;
	float ShootingModeInterval = 1.f;


	float h; // ����Ű �Էº���
 	const float gravity = 1000.f;
		/*
		�� ����ڿ� �ݵ�� �߷� �߰�
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

	void LateUpdate(float dt) override;
	void FixedUpdate(float dt) override;



	void Draw(sf::RenderWindow& window) override;
};

