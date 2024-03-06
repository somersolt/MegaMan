#pragma once
#include "SpriteGo.h"
#include "Animator.h"

class Player : public SpriteGo
{
protected:

	Animator playerAnimation; // sprite�ʹ� ����
	sf::RectangleShape playerCollision; //�浹�ڽ� (���� �Ⱦ�)


	float speed = 200.f;
	sf::Vector2f velocity;
	bool isGrounded = true; // �����߿� �� ���� �ȵǰ� �ϴ� ���
	bool isJumping = false; // �ϰ� ��� ����
	bool isCantFlip = false; // �Է¹��� ����
	bool isShooting = false; // ��� ������� ����
	bool isDash = false; // ���

	float shootTimer = 0.f;

	float h; // ����Ű �Էº���
 	const float gravity = 1500.f;
		/*
		�� ����ڿ� �ݵ�� �߷� �߰�
		*/

	void IdleAnimation();

public:

	Player(const std::string& name = "");
	~Player() override = default;


	void Init() override;
	void Reset() override;
	void Update(float dt) override;
};

