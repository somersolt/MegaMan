#include "pch.h"
#include "Rabbit.h"
#include "SceneGame.h"
#include "SpriteGoEffect.h"



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
	MapImage = sceneGame->collisionMapImage;
	//게임 씬 동기화

	EnemyAnimation.ClearEvent();

	SetOrigin(Origins::BC);
	isGrounded = true;
	speed = 0;
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
		if (sceneGame->GetPlayerPostion().x - position.x > 0)
		{
			h = 1;
		}
		else
		{
			h = -1;
		}
		EnemyAnimation.Play("animations/rabbit/Jump.csv");
		skillTimer = 0;
		onSkill = true;
		isJump = true;
		isGrounded = false;
		isSlopeGrounded = false;
		speed = 200;
		position.y -= rollBackSideSlope + 3;
		rollBackSideSlope = 0;
		position.y -= 2;
		velocity.y = -400.f;
		break;
	case 3:
		if (sceneGame->GetPlayerPostion().x - position.x > 0)
		{
			h = 1;
		}
		else
		{
			h = -1;
		}
		skillTimer = 0;
		onSkill = true;
		EnemyAnimation.Play("animations/rabbit/Shot.csv");
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