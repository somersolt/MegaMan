#include "pch.h"
#include "Rabbit.h"
#include "SceneGame.h"
#include "SpriteGoEffect.h"
#include "EnemyShot.h"
#include "Player/Player.h"



Rabbit::Rabbit(const std::string& name, sf::Image& mapImage) : Enemy(name, mapImage)
{
}

void Rabbit::Init()
{
	SpriteGo::Init();
	EnemyAnimation.SetTarget(&sprite);
	textureId = "graphics/rabbit1.png";
}

void Rabbit::Reset()
{
	SpriteGo::Reset();
	SetTexture(textureId);
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("player"));
	MapImage = sceneGame->collisionMapImage;
	//게임 씬 동기화

	EnemyAnimation.ClearEvent();

	SetOrigin(Origins::BC);
	isGrounded = true;
	speed = 0;
	Hp = 2;
	// 애니메이션 세팅

	EnemyHitBox.setSize({ 27.f, 31.f });
	EnemyHitBox.setOrigin({ EnemyHitBox.getLocalBounds().width / 2, EnemyHitBox.getLocalBounds().height });
	EnemyHitBox.setFillColor(sf::Color::Transparent);
	EnemyHitBox.setPosition(position);
	EnemyAnimation.AddEvent("animations/rabbit/Jump.csv", 3,
		[=]() {onSkill = false; });
	EnemyAnimation.AddEvent("animations/rabbit/Shot.csv", 6,
		[=]() {onSkill = false; });
}


void Rabbit::Update(float dt)
{
	SpriteGo::Update(dt);

	skillTimer += dt;

	int skill = -1;
	if (skillTimer > 3 && !onSkill)
	{
		skill = Utils::RandomRange(3, 4);
	}

	switch (skill)
	{
	case 0:
	case 1:
	case 2:
		Jump();
		break;
	case 3:
		Fire();
		break;
	default:
		break;
	}

	if (!onSkill && isGrounded)
	{
		SetTexture("graphics/rabbit1.png", true);
		speed = 0;

		SetOrigin(Origins::BC);
	}
}

void Rabbit::Jump()
{
	if (sceneGame->GetPlayerPostion().x - position.x > 0)
	{
		h = 1;
	}
	else
	{
		h = -1;
	}
	onSkill = true;
	EnemyAnimation.Play("animations/rabbit/Jump.csv");
	skillTimer = 0;
	isJump = true;
	isGrounded = false;
	isSlopeGrounded = false;
	speed = 200;
	position.y -= rollBackSideSlope + 3;
	rollBackSideSlope = 0;
	position.y -= 2;
	velocity.y = -300.f;
}

void Rabbit::Fire()
{
	if (player->GetPosition().x - position.x > 0)
	{
		h = 1;
	}
	else
	{
		h = -1;
	}
	onSkill = true;
	skillTimer = 0;
	EnemyAnimation.Play("animations/rabbit/Shot.csv");

	sf::Vector2f dir({ player->GetPosition().x - position.x,  player->GetPosition().y - position.y });
	Utils::Normalize(dir);

	EnemyShot* rabbitShot = new EnemyShot("rabbitShot");
	rabbitShot->Init("animations/rabbit/Bullet.csv");
	rabbitShot->Reset();
	rabbitShot->SetPosition(position);
	rabbitShot->EnemyFire(dir, 100.f, 1, h);
	sceneGame->AddGo(rabbitShot);
}
