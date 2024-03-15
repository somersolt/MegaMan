#include "pch.h"
#include "BeeBoom.h"
#include "SceneGame.h"
#include "SpriteGoEffect.h"
#include "Player/Player.h"

BeeBoom::BeeBoom(const std::string& name, sf::Image& mapImage) : Enemy(name, mapImage)
{
}

void BeeBoom::Init()
{
	Enemy::Init();
	EnemyAnimation.SetTarget(&sprite);
	textureId = "graphics/bee/beeBoom1.png";
}

void BeeBoom::Reset(int g)
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
	speed = 100;
	gravity = 600;
	h = g;
	Hp = 1;

	// 애니메이션 세팅

	EnemyHitBox.setSize({ 10.f, 12.f });
	EnemyHitBox.setOrigin({ EnemyHitBox.getLocalBounds().width / 2, EnemyHitBox.getLocalBounds().height });
	EnemyHitBox.setFillColor(sf::Color::Transparent);
	EnemyHitBox.setPosition(position);
	EnemyAnimation.Play("animations/bee/Boom.csv");
}

void BeeBoom::Update(float dt)
{
	Enemy::Update(dt);

	skillTimer += dt;

	if (skillTimer > 3.f)
	{
		Hp = 0;
	}



	if (isBottomColliding)
	{
		speed = 0;
	}

}

void BeeBoom::LateUpdate(float dt)
{
	Enemy::LateUpdate(dt);

	if (GetGlobalBounds().intersects(player->GetPlayerBounds()) && !onSkill)
	{
		skillTimer += 2.f;
		onSkill = true;
	}
}
