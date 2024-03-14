#include "pch.h"
#include "Snow.h"
#include "SceneGame.h"
#include "SpriteGoEffect.h"
#include "Player/Player.h"

Snow::Snow(const std::string& name, sf::Image& mapImage) : Enemy(name, mapImage)
{
	EnemyHitBox.setSize({ 8.f, 8.f });
	EnemyHitBox.setPosition(position);
}

void Snow::Init()
{
	Enemy::Init();
	EnemyAnimation.SetTarget(&sprite);
	textureId = "graphics/snowthrower/snow.png";
}

void Snow::Reset()
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
	speed = 400;
	gravity = 300;
	h = -1;
	Hp = 3;

	// 애니메이션 세팅

	EnemyHitBox.setSize({ 8.f, 8.f });
	EnemyHitBox.setOrigin({ EnemyHitBox.getLocalBounds().width / 2, EnemyHitBox.getLocalBounds().height });
	EnemyHitBox.setFillColor(sf::Color::Transparent);
	EnemyHitBox.setPosition(position);
	EnemyAnimation.Play("animations/snowthrower/snow.csv");

	EnemyAnimation.ClearEvent();
	EnemyAnimation.AddEvent("animations/snowthrower/SnowRolling.csv", 4,
		[=]() {speed = 200; });
}

void Snow::Update(float dt)
{
	Enemy::Update(dt);

	if (isBottomColliding && !onSkill)
	{
		onSkill = true;
		EnemyAnimation.Play("animations/snowthrower/SnowRolling.csv");
	}

	if (isMiddleRightColliding || isMiddleLeftColliding)
	{
		Hp = 0;
	}
}

void Snow::LateUpdate(float dt)
{

	Enemy::LateUpdate(dt);

}



