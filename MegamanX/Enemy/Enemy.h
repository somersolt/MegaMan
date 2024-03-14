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

	bool isBottomColliding = false; // �ٴڸ� �浹 ó��
	bool isSlopeColliding = false; // ���� �浹 ó��

	bool isBottomLeftColliding = false; // ���� �ϴ� 
	bool isBottomCenterColliding = false; // �߾� �ϴ� 
	bool isBottomRightColliding = false; // ���� �ϴ�

	bool isBottomLeftSlopeColliding = false;
	bool isBottomRightSlopeColliding = false;
	bool isBottomSlopeColliding = false;



	bool isMiddleLeftColliding = false; // ���� �浹 ó��

	bool isMiddleRightColliding = false; // ���� �浹 ó��

	bool isJump = false; // ����

	bool isGrounded = false; // ���ִ� ����
	bool isSlopeGrounded = false; // ��翡 ���ִ� ����
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

