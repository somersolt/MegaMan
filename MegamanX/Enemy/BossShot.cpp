#include "pch.h"
#include "BossShot.h"
#include "SceneGame.h"
#include "SpriteGoEffect.h"
#include "Player/Player.h"

BossShot::BossShot(const std::string& name, sf::Image& mapImage) : Enemy(name, mapImage)
{
}

void BossShot::Init()
{
	Enemy::Init();
	textureId = "graphics/boss/iceShot.png";
}

void BossShot::Reset(int g)
{
	Enemy::Reset();
	SetTexture(textureId);
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("player"));
	MapImage = sceneGame->collisionMapImage;
	//게임 씬 동기화

	EnemyAnimation.ClearEvent();

	SetOrigin(Origins::BC);
	isGrounded = true;
	speed = 150;
	gravity = 0;
	h = g;
	Hp = 999;

	// 애니메이션 세팅

	EnemyHitBox.setSize({ 14.f, 14.f });
	EnemyHitBox.setOrigin({ EnemyHitBox.getLocalBounds().width / 2, EnemyHitBox.getLocalBounds().height });
	EnemyHitBox.setFillColor(sf::Color::Transparent);
	EnemyHitBox.setPosition(position);
}

void BossShot::Update(float dt)
{
	Enemy::Update(dt);

	if (isMiddleRightColliding || isMiddleLeftColliding)
	{
		Hp = 0;
	}
}

void BossShot::LateUpdate(float dt)
{
	SpriteGo::LateUpdate(dt);
	EnemyAnimation.Update(dt);

	EnemyHitBox.setPosition(position);
	EnemyBounds = EnemyHitBox.getGlobalBounds();

	if (!isCantFlip)
		SetFlipX(h > 0 ? true : false);

	if (Hp <= 0)
	{
		skillTimer += dt;
		speed = 0;
		SetTexture("graphics/boss/iceShot2.png");
		if(skillTimer > 0.2)
		sceneGame->RemoveGo(this);
	}

	if (velocity.y > 800.f)
	{
		velocity.y = 800.f;
	}
	if (velocity.y > 0.f)
	{
		isJump = false;
	}

	velocity.x = h * speed * dt;

	position += sf::Vector2f(velocity.x, velocity.y * dt);

	boundingBoxWorldPos = EnemyBounds.getPosition();
	boundingBoxLocalPos = sceneGame->PlayerBoundsWorldToMapLocal(boundingBoxWorldPos);

	startX = std::max(0, static_cast<int>(boundingBoxLocalPos.x));
	endX = std::min(static_cast<int>(MapImage.getSize().x),
		static_cast<int>(boundingBoxLocalPos.x + EnemyHitBox.getSize().x));
	startY = std::max(0, static_cast<int>(boundingBoxLocalPos.y));
	endY = std::min(static_cast<int>(MapImage.getSize().y),
		static_cast<int>(boundingBoxLocalPos.y + EnemyHitBox.getSize().y));






	CheckRightCollision();
	CheckLeftCollision();
	CheckBottomCollision();
	CheckSlopeCollision();


	EnemyHitBox.setPosition(position);
	EnemyBounds = EnemyHitBox.getGlobalBounds();

	SetPosition(position); // 중력을 포함한 위치 적용
	SetOrigin(Origins::BC);
}
