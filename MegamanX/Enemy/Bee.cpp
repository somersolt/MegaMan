#include "pch.h"
#include "Bee.h"
#include "SceneGame.h"
#include "Player/Player.h"
#include "SpriteGoEffect.h"
#include "BeeBoom.h"


Bee::Bee(const std::string& name, sf::Image& mapImage) : Enemy(name, mapImage)
{
}

void Bee::Init()
{
	SpriteGo::Init();
	EnemyAnimation.SetTarget(&sprite);
	textureId = "graphics/bee/bee1.png";
}

void Bee::Reset()
{
	SpriteGo::Reset();
	SetTexture(textureId, true);
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("player"));
	MapImage = sceneGame->collisionMapImage;
	//게임 씬 동기화

	EnemyAnimation.ClearEvent();

	EnemyAnimation.Play("animations/bee/Idle.csv");

	SetOrigin(Origins::BC);
	isGrounded = true;
	gravity = 0;
	speed = 100;
	Hp = 1;
	// 애니메이션 세팅

	EnemyHitBox.setSize({ 35.f, 36.f });
	EnemyHitBox.setOrigin({ EnemyHitBox.getLocalBounds().width / 2, EnemyHitBox.getLocalBounds().height });
	EnemyHitBox.setFillColor(sf::Color::Transparent);
	EnemyHitBox.setPosition(position);

	EnemyAnimation.AddEvent("animations/bee/Shot.csv", 9,
		[this]() {Drop(1);});
	EnemyAnimation.AddEvent("animations/bee/Shot.csv", 17,
		[this]() {Drop(0); });
	EnemyAnimation.AddEvent("animations/bee/Shot.csv", 22,
		[this]() {Drop(-1); });
	EnemyAnimation.AddEvent("animations/bee/Shot.csv", 28,
		[=]() {speed = 100;});
}

void Bee::Update(float dt)
{
	if (Utils::Distance(player->GetPosition(), position) < 200)
	{
		isFight = true;
	}
	if (!isFight)
	{
		return;
	}

	SpriteGo::Update(dt);

	playerPos = player->GetPosition();

	float toPlayerDistance = std::abs(playerPos.x - position.x);

	if (!onSkill && toPlayerDistance < 50)
	{
		onSkill = true;
		EnemyAnimation.Play("animations/bee/Shot.csv");
		speed = 0;

	}

	if (!GetGlobalBounds().intersects(sceneGame->GetViewBounds()) && onSkill)
	{
		Hp = 0;
	}

}

void Bee::LateUpdate(float dt)
{
	if (!isFight)
	{
		return;
	}
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

void Bee::Drop(int g)
{
	BeeBoom* boom = new BeeBoom("enemy", sceneGame->collisionMapImage);
	boom->Init();
	boom->Reset(g);
	boom->SetOrigin(Origins::BC);
	boom->SetPosition(position);
	sceneGame->AddGo(boom);
}
