#pragma once
#include "SpriteGo.h"

class SceneGame;
class Player;
class EnemyShot;

class Enemy : public SpriteGo
{
protected:
	SceneGame* sceneGame = nullptr;
	Player* player = nullptr;
	Animator EnemyAnimation;
	sf::RectangleShape EnemyHitBox;
	sf::FloatRect EnemyBounds;
	sf::Vector2f boundingBoxWorldPos;
	sf::Vector2f boundingBoxLocalPos;
	sf::Color collisionColor = { 255 , 0 , 0 };
	sf::Color sideCollisionColor = { 0 , 255 , 0 };
	sf::Color slopeCollisionColor = { 0 , 0 , 255 };
	int h = 0;
	int Hp = 5;
	float speed = 200.f;
	sf::Vector2f velocity;
	sf::Image& MapImage;
	bool isCantFlip = false;

	bool isBottomColliding = false; // 바닥면 충돌 처리
	bool isSlopeColliding = false; // 경사로 충돌 처리

	bool isBottomLeftColliding = false; // 좌측 하단 
	bool isBottomCenterColliding = false; // 중앙 하단 
	bool isBottomRightColliding = false; // 우측 하단

	bool isBottomLeftSlopeColliding = false;
	bool isBottomRightSlopeColliding = false;
	bool isBottomSlopeColliding = false;



	bool isMiddleLeftColliding = false; // 좌측 충돌 처리

	bool isMiddleRightColliding = false; // 우측 충돌 처리

	bool isJump = false; // 점프

	bool isGrounded = false; // 서있는 상태
	bool isSlopeGrounded = false; // 경사에 서있는 상태
	float gravity = 1000.f;

	int startX;
	int endX;
	int startY;
	int endY;


	int rollBackBottomLeft;
	int rollBackBottomRight;
	int rollBackBottomCenter;

	int rollBackMiddleLeft;

	int rollBackMiddleRight;

	int rollBackSlope;
	int rollBackSideSlope;
	Enemy(const std::string& name, sf::Image& mapImage);
public:
	

	virtual void Init() override;
	virtual void Reset() override;

	virtual void OnDamage(int damage);

	virtual void LateUpdate(float dt) override;
	virtual void FixedUpdate(float dt) override;

	virtual void CheckRightCollision();
	virtual void CheckLeftCollision();
	virtual void CheckBottomCollision();
	virtual void CheckSlopeCollision();

};

