#pragma once
#include "SpriteGo.h"
#include "Animator.h"

class Player : public SpriteGo
{
protected:

	Animator playerAnimation; // sprite와는 별개
	sf::RectangleShape playerCollision; //충돌박스 (아직 안씀)


	float speed = 200.f;
	sf::Vector2f velocity;
	bool isGrounded = true; // 점프중에 또 점프 안되게 하는 기능
	bool isJumping = false; // 하강 모션 변수
	bool isCantFlip = false; // 입력방지 변수
	bool isShooting = false; // 사격 모션으로 변경
	bool isDash = false; // 대시

	float shootTimer = 0.f;

	float h; // 방향키 입력변수
 	const float gravity = 1500.f;
		/*
		맵 만든뒤에 반드시 중력 추가
		*/

	void IdleAnimation();

public:

	Player(const std::string& name = "");
	~Player() override = default;


	void Init() override;
	void Reset() override;
	void Update(float dt) override;
};

