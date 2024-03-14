#include "pch.h"
#include "Bat.h"
#include "SceneGame.h"
#include "Player/Player.h"
#include "SpriteGoEffect.h"

Bat::Bat(const std::string& name, sf::Image& mapImage) : Enemy(name, mapImage)
{
}

void Bat::Init()
{
	SpriteGo::Init();
	EnemyAnimation.SetTarget(&sprite);
	textureId = "graphics/bat/bat1.png";
}

void Bat::Reset(sf::Vector2f s)
{
	SpriteGo::Reset();
	SetTexture(textureId, true);
	spawn = s;
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("player"));
	MapImage = sceneGame->collisionMapImage;
	//게임 씬 동기화

	EnemyAnimation.ClearEvent();

	EnemyAnimation.Play("animations/bat/Idle.csv");

	SetOrigin(Origins::BC);
	isGrounded = true;
	gravity = 0;
	speed = 0;
	Hp = 2;
	// 애니메이션 세팅

	EnemyHitBox.setSize({ 14.f, 22.f });
	EnemyHitBox.setOrigin({ EnemyHitBox.getLocalBounds().width / 2, EnemyHitBox.getLocalBounds().height });
	EnemyHitBox.setFillColor(sf::Color::Transparent);
	EnemyHitBox.setPosition(position);
}

void Bat::Update(float dt)
{

	SpriteGo::Update(dt);

	playerPos = player->GetPosition();

	float toPlayerDistance = Utils::Distance(playerPos, position);

	if (!onSkill && toPlayerDistance < 100)
	{
		onSkill = true;
		EnemyAnimation.Play("animations/bat/Fly.csv");
		speed = 100;
		dir = playerPos - position;
		Utils::Normalize(dir);
	}


	if (onSkill && toPlayerDistance > 100)
	{
		onSkill = false;
		goHome = true;
		EnemyAnimation.Play("animations/bat/Fly.csv");
		speed = 50;
		dir = position - spawn;
		Utils::Normalize(dir);
	}


	if (GetPosition() == spawn && goHome)
	{
		goHome = false;
		EnemyAnimation.Play("animations/bat/Idle.csv");
		speed = 0;
	}
}

void Bat::LateUpdate(float dt)

{
	SpriteGo::LateUpdate(dt);
	EnemyAnimation.Update(dt);

	EnemyHitBox.setPosition(position);
	EnemyBounds = EnemyHitBox.getGlobalBounds();

	if (!isCantFlip)
		SetFlipX(h > 0 ? true : false);

	if (Hp <= 0)
	{
		SpriteGoEffect* boom = new SpriteGoEffect("boom");
		sceneGame->AddGo(boom);
		boom->SetOrigin({ EnemyHitBox.getGlobalBounds().width / 2, EnemyHitBox.getGlobalBounds().height });
		boom->SetPosition(position);
		boom->Init();
		boom->Reset();
		Release();
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

	position += dir * speed * dt;

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
