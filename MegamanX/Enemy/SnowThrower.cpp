#include "pch.h"
#include "SnowThrower.h"
#include "SceneGame.h"
#include "SpriteGoEffect.h"
#include "EnemyShot.h"
#include "Player/Player.h"
#include "Snow.h"

SnowThrower::SnowThrower(const std::string& name, sf::Image& mapImage) : Enemy(name, mapImage)
{
}

void SnowThrower::Init()
{
	SpriteGo::Init();
	EnemyAnimation.SetTarget(&sprite);
	textureId = "graphics/snowthrower/snowthrower.png";
}

void SnowThrower::Reset()
{

	SpriteGo::Reset();
	SetTexture(textureId, true);
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("player"));
	MapImage = sceneGame->collisionMapImage;
	//게임 씬 동기화

	EnemyAnimation.ClearEvent();

	EnemyAnimation.Play("animations/snowthrower/Idle.csv");

	SetOrigin(Origins::BC);
	isGrounded = true;
	speed = 0;
	Hp = 3;
	// 애니메이션 세팅

	EnemyHitBox.setSize({ 16.f, 30.f });
	EnemyHitBox.setOrigin({ EnemyHitBox.getLocalBounds().width / 2, EnemyHitBox.getLocalBounds().height });
	EnemyHitBox.setFillColor(sf::Color::Transparent);
	EnemyHitBox.setPosition(position);

	std::function<void()> Throw = std::bind(&SnowThrower::Throw, this);
	EnemyAnimation.AddEvent("animations/snowthrower/Throw.csv", 7,
		Throw);
}

void SnowThrower::Update(float dt)
{
	SpriteGo::Update(dt);

	skillTimer += dt;

	float toPlayerDistance = Utils::Distance(player->GetPosition(), position);

	if (skillTimer > 4.f && !onSkill && toPlayerDistance < 200)
	{
		onSkill = true;

		EnemyAnimation.Play("animations/snowthrower/Throw.csv");
	}

}

void SnowThrower::Throw()
{
	EnemyAnimation.Play("animations/snowthrower/Idle.csv");
	skillTimer = 0;
	onSkill = false;

	Snow* snow = new Snow("enemy", sceneGame->collisionMapImage);
	snow->Init();
	snow->Reset();
	snow->SetOrigin(Origins::BC);
	snow->SetPosition({ position.x , position.y - std::abs((player->GetPosition().y - position.y)) });
	sceneGame->AddGo(snow);

}
