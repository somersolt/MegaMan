#include "pch.h"
#include "Bee2.h"
#include "SceneGame.h"
#include "Player/Player.h"
#include "SpriteGoEffect.h"

Bee2::Bee2(const std::string& name, sf::Image& mapImage) : Enemy(name, mapImage)
{
}

void Bee2::Init()
{
	SpriteGo::Init();
	EnemyAnimation.SetTarget(&sprite);
	textureId = "graphics/bee2/bee2_1.png";
}

void Bee2::Reset()
{

	SpriteGo::Reset();
	SetTexture(textureId, true);
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("player"));
	MapImage = sceneGame->collisionMapImage;
	//게임 씬 동기화

	EnemyAnimation.ClearEvent();

	EnemyAnimation.Play("animations/bee2/Idle.csv");

	SetOrigin(Origins::BC);
	isGrounded = true;
	gravity = 0;
	speed = 400;
	Hp = 2;
	// 애니메이션 세팅

	EnemyHitBox.setSize({ 29.f, 32.f });
	EnemyHitBox.setOrigin({ EnemyHitBox.getLocalBounds().width / 2, EnemyHitBox.getLocalBounds().height });
	EnemyHitBox.setFillColor(sf::Color::Transparent);
	EnemyHitBox.setPosition(position);
}

void Bee2::Update(float dt)
{
	SpriteGo::Update(dt);

	playerPos = player->GetPosition();
	float toPlayerDistance = Utils::Distance(playerPos, position);
	dir = playerPos - position;
	Utils::Normalize(dir);

	switch (status)
	{
	case Bee2::None:
		break;
	case Bee2::Far:
		speed = 400;
		break;
	case Bee2::Medium:
		speed = 70;
		break;
	case Bee2::Close:
		speed = 10;
		break;
	default:
		break;
	}

	if (toPlayerDistance < 100 && toPlayerDistance > 50)
	{
		onSkill = true;
		status = Status::Medium;
	}


	if (toPlayerDistance < 50)
	{
		status = Status::Close;
	}

	if (!GetGlobalBounds().intersects(sceneGame->GetViewBounds()) && onSkill)
	{
		Hp = 0;
	}

}

void Bee2::LateUpdate(float dt)
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
