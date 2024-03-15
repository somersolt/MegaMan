#include "pch.h"
#include "Wheeler.h"
#include "SceneGame.h"
#include "Player/Player.h"

Wheeler::Wheeler(const std::string& name, sf::Image& mapImage) : Enemy(name, mapImage)
{
}

void Wheeler::Init()
{
	SpriteGo::Init();
	EnemyAnimation.SetTarget(&sprite);
	textureId = "graphics/wheeler/wheeler1.png";
}

void Wheeler::Reset(int g)
{


	SpriteGo::Reset();
	SetTexture(textureId, true);
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("player"));
	MapImage = sceneGame->collisionMapImage;
	//���� �� ����ȭ

	EnemyAnimation.ClearEvent();

	EnemyAnimation.Play("animations/wheeler/Idle.csv");

	SetOrigin(Origins::BC);
	isGrounded = true;
	speed = 200;
	Hp = 2;
	h = g;
	// �ִϸ��̼� ����

	EnemyHitBox.setSize({ 37.f, 37.f });
	EnemyHitBox.setOrigin({ EnemyHitBox.getLocalBounds().width / 2, EnemyHitBox.getLocalBounds().height });
	EnemyHitBox.setFillColor(sf::Color::Transparent);
	EnemyHitBox.setPosition(position);
}

void Wheeler::Update(float dt)
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

	skillTimer += dt;

	if (isMiddleLeftColliding)
	{
		h = 1;
	}
	if (isMiddleRightColliding)
	{
		h = -1;
	}

	if (!onSkill && GetGlobalBounds().intersects(player->GetPlayerBounds()))
	{
		onSkill = true;

		EnemyAnimation.Play("animations/wheeler/Down.csv");
		Down();
	}
}

void Wheeler::Down()
{
	speed = 0;
	h = 0;
}
