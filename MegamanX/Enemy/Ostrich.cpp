#include "pch.h"
#include "Ostrich.h"
#include "SceneGame.h"
#include "SpriteGoEffect.h"
#include "EnemyShot.h"
#include "Player/Player.h"

Ostrich::Ostrich(const std::string& name, sf::Image& mapImage) : Enemy(name, mapImage)
{
}

void Ostrich::Init()
{
	SpriteGo::Init();
	EnemyAnimation.SetTarget(&sprite);
	textureId = "graphics/ostrich/ostrich2.png";
}

void Ostrich::Reset()
{
	SpriteGo::Reset();
	SetTexture(textureId);
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("player"));
	MapImage = sceneGame->collisionMapImage;
	//게임 씬 동기화

	EnemyAnimation.ClearEvent();

	EnemyAnimation.Play("animations/ostrich/Idle.csv");

	SetOrigin(Origins::BC);
	isGrounded = true;
	speed = 0;
	Hp = 6;
	// 애니메이션 세팅

	EnemyHitBox.setSize({ 27.f, 74.f });
	EnemyHitBox.setOrigin({ EnemyHitBox.getLocalBounds().width / 2, EnemyHitBox.getLocalBounds().height });
	EnemyHitBox.setFillColor(sf::Color::Transparent);
	EnemyHitBox.setPosition(position);

	

	std::function<void()> Fire = std::bind(&Ostrich::Fire, this);
	EnemyAnimation.AddEvent("animations/ostrich/Shot.csv", 5,
		Fire);

	std::function<void()> ToIdle = std::bind(&Ostrich::ToIdle, this);
	EnemyAnimation.AddEvent("animations/ostrich/Shot.csv", 17,
		ToIdle);
}

void Ostrich::Update(float dt)
{
	SpriteGo::Update(dt);

	skillTimer += dt;

	playerPos = player->GetPosition();

	float toPlayerDistance = std::abs(playerPos.x - position.x);
	if (!onSkill && skillTimer > 3 && toPlayerDistance < 100)
	{
		onSkill = true;
		EnemyAnimation.Play("animations/ostrich/Shot.csv");
	}
}

void Ostrich::LateUpdate(float dt)

{
	SpriteGo::LateUpdate(dt);
	EnemyAnimation.Update(dt);

	EnemyHitBox.setPosition(position);
	EnemyBounds = EnemyHitBox.getGlobalBounds();

	if (!isCantFlip)
		SetFlipX(h > 0 ? true : false);

	velocity.x = h * speed * dt * 0.5;
	velocity.x = Utils::Clamp(velocity.x, -100, 100);
	if (isGrounded)
	{
		velocity.y = 0;
	}

	if (!isGrounded && !isSlopeGrounded)
	{
		velocity.y += gravity * dt;
	}
	if (Hp <= 0)
	{
		SpriteGoEffect* boom = new SpriteGoEffect("boom");
		sceneGame->AddGo(boom);
		boom->SetOrigin({ EnemyHitBox.getGlobalBounds().width / 2, EnemyHitBox.getGlobalBounds().height });
		boom->SetPosition(position);
		boom->Init();
		boom->Reset();

		SpriteGoEffect* boom2 = new SpriteGoEffect("boom");
		sceneGame->AddGo(boom2);
		boom2->SetOrigin({ EnemyHitBox.getGlobalBounds().width / 2, EnemyHitBox.getGlobalBounds().height });
		boom2->SetPosition({ position.x,position.y + 40 });
		boom2->Init();
		boom2->Reset();

		SpriteGoEffect* boom3 = new SpriteGoEffect("boom");
		sceneGame->AddGo(boom3);
		boom3->SetOrigin({ EnemyHitBox.getGlobalBounds().width / 2, EnemyHitBox.getGlobalBounds().height });
		boom3->SetPosition({ position.x,position.y + 20 });
		boom3->Init();
		boom3->Reset();
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

	if (isMiddleRightColliding)
	{
		velocity.x = std::min(0.f, velocity.x);
	}
	if (isMiddleLeftColliding)
	{
		velocity.x = std::max(0.f, velocity.x);
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


	if (isMiddleRightColliding) // 우측 보정
	{
		position.x -= rollBackMiddleRight - 1;
		rollBackMiddleRight = 0;
	}
	if (isMiddleLeftColliding) // 좌측 보정
	{
		position.x += rollBackMiddleLeft - 1;
		rollBackMiddleLeft = 0;
	}

	if (isSlopeColliding && isGrounded && velocity.y >= 0.f) // 경사 보정
	{
		position.y -= rollBackSlope - 1;
		position.y = std::floor(position.y);
		rollBackSlope = 0;
	}
	if (isBottomColliding && isGrounded) // 하단 보정
	{
		rollBackBottomLeft = std::max(rollBackBottomLeft, rollBackBottomCenter);
		position.y -= std::max(rollBackBottomLeft, rollBackBottomRight) - 1;
		rollBackBottomLeft = 0;
		rollBackBottomRight = 0;
		rollBackBottomCenter = 0;
	}


	EnemyHitBox.setPosition(position);
	EnemyBounds = EnemyHitBox.getGlobalBounds();

	SetPosition(position); // 중력을 포함한 위치 적용
	SetOrigin(Origins::BC);
}

void Ostrich::Fire()
{
	if (player->GetPosition().x - position.x > 0)
	{
		h = 1;
	}
	else
	{
		h = -1;
	}
	skillTimer = 0;
	EnemyShot* ostrichShot = new EnemyShot("ostrichShot");
	ostrichShot->Init("animations/ostrich/Bullet.csv", "graphics/ostrich/ostrichShot.png");
	ostrichShot->Reset();
	ostrichShot->SetPosition({ position.x , position.y - 70 });
	sf::Vector2f dir({ player->GetPosition().x - position.x,  player->GetPosition().y - 20 - position.y + 70 });
	Utils::Normalize(dir);
	ostrichShot->EnemyFire(dir, 200.f, 1, h);
	sceneGame->AddGo(ostrichShot);
}

void Ostrich::ToIdle()
{
	EnemyAnimation.Play("animations/ostrich/Idle.csv");
	skillTimer = 0;
	onSkill = false;
}
