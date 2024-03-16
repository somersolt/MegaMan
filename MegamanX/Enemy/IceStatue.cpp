#include "pch.h"
#include "IceStatue.h"
#include "SceneGame.h"
#include "SpriteGoEffect.h"
#include "Player/Player.h"
#include "Boss.h"


IceStatue::IceStatue(const std::string& name, sf::Image& mapImage) : Enemy(name, mapImage)
{
}

void IceStatue::Init()
{
	Enemy::Init();
	textureId = "graphics/boss/iceStatue4.png";
}

void IceStatue::Reset(int g)
{
	Enemy::Reset();
	SetTexture(textureId);
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("player"));

	for (auto& go : sceneGame->enemyList) {
		Boss* findboss = dynamic_cast<Boss*>(go);
		if (findboss)
		{
			boss = findboss;
			break;
		}
	} // 보스 동기화

	MapImage = sceneGame->collisionMapImage;
	//게임 씬 동기화

	EnemyAnimation.ClearEvent();

	SetOrigin(Origins::BC);
	isGrounded = true;
	speed = 0;
	gravity = 0;
	h = g;
	Hp = 999;

	// 애니메이션 세팅
	EnemyHitBox.setSize({ 28.f, 32.f });
	EnemyHitBox.setOrigin({ EnemyHitBox.getLocalBounds().width / 2, EnemyHitBox.getLocalBounds().height });
	EnemyHitBox.setFillColor(sf::Color::Transparent);
	EnemyHitBox.setPosition(position);
	EnemyAnimation.Play("animations/boss/IceStatue.csv");

	EnemyAnimation.AddEvent("animations/boss/IceStatue.csv", 8,
		[=]() {gravity = 300; });
}

void IceStatue::Update(float dt)
{
	SpriteGo::Update(dt);
	isCantFlip = true;
	if (boss->GetWind() == true)
	{
		if (boss->GetWindSide() == Sides::Left)
		{
			h = -1;
		}
		if (boss->GetWindSide() == Sides::Right)
		{
			h = 1;
		}
		speed = 100;
	}
	if (isMiddleRightColliding || isMiddleLeftColliding)
	{
		Hp = 0;
	}
	if (!GetGlobalBounds().intersects(sceneGame->GetViewBounds()))
	{
		Hp = 0;
	}
	if (position.x < 7698 || position.x > 7926)
	{
		Hp = 0;
	}
	if (GetGlobalBounds().intersects(boss->GetGlobalBounds()) && boss->GetBossStatus() == Boss::BossStatus::Sliding)
	{
		Hp = 0;
	}
}

void IceStatue::LateUpdate(float dt)
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
		if (skillTimer > 0.05)
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

	if (isGrounded)
	{
		velocity.y = 0;
	}

	if (!isGrounded && !isSlopeGrounded)
	{
		velocity.y += gravity * dt;
	}

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
