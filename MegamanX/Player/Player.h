#pragma once
#include "SpriteGo.h"
#include "Animator.h"

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

	Animator playerAnimation; // sprite�ʹ� ����
	sf::RectangleShape playerCollision; //�浹�ڽ� (���� �Ⱦ�)


	float speed = 200.f;
	sf::Vector2f velocity;
	Status preStatus = None;
	Status currentStatus = None;



	bool isCharge = false;
	float chargeTimer = 0.f;


	bool isDash = false; // ���

	bool isCantFlip = false; // �Է¹��� ����
	bool isShooting = false; // ��� ������� ����


	float shootTimer = 0.f;
	float shootInterval = 0.3f;

	bool isShootingMode = false; // ���
	float ShootingModeTimer = 0.f;
	float ShootingModeInterval = 1.f;


	float h; // ����Ű �Էº���
 	const float gravity = 1500.f;
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


};
