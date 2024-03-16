#include "pch.h"
#include "Lumberjack.h"
#include "SceneGame.h"
#include "SpriteGoEffect.h"
#include "EnemyShot.h"
#include "Player/Player.h"
#include "Tree.h"

Lumberjack::Lumberjack(const std::string& name, sf::Image& mapImage, std::string lumberjackStump) : Enemy(name, mapImage), lumberjackStump(lumberjackStump)
{
}

void Lumberjack::Init()
{
	SpriteGo::Init();
	EnemyAnimation.SetTarget(&sprite);
	textureId = "graphics/lumberjack.png";
}

void Lumberjack::Reset()
{
	SpriteGo::Reset();
	SetTexture(textureId,true);
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("player"));
	MapImage = sceneGame->collisionMapImage;
	//게임 씬 동기화

	EnemyAnimation.ClearEvent();

	EnemyAnimation.Play("animations/lumberjack/Idle.csv");

	SetOrigin(Origins::BC);
	isGrounded = true;
	speed = 0;
	Hp = 3;
	// 애니메이션 세팅

	EnemyHitBox.setSize({ 22.f, 30.f });
	EnemyHitBox.setOrigin({ EnemyHitBox.getLocalBounds().width / 2, EnemyHitBox.getLocalBounds().height });
	EnemyHitBox.setFillColor(sf::Color::Transparent);
	EnemyHitBox.setPosition(position);

	std::function<void()> ToIdle = std::bind(&Lumberjack::ToIdle, this);
	EnemyAnimation.AddEvent("animations/lumberjack/Swing.csv", 6,
		ToIdle);
}

void Lumberjack::Update(float dt)
{
	SpriteGo::Update(dt);

	skillTimer += dt;

	float toPlayerDistance = Utils::Distance(player->GetPosition(), position);

	if (skillTimer > 1.f && !onSkill && toPlayerDistance < 200)
	{
		onSkill = true;
		Swing();
	}

}



void Lumberjack::Swing()
{

	EnemyAnimation.Play("animations/lumberjack/Swing.csv");

	auto& list = sceneGame->GetEnemyList();
	for (auto go : list)
	{
		Tree* Findtree = dynamic_cast<Tree*>(go);
		if (Findtree != nullptr)
		{
			if (Findtree->GetTreeNumber() == 1 && Findtree->GetStumpName() == lumberjackStump)
			{
				tree = Findtree;
				tree->SetActive(false);
				tree->Release();
				EnemyShot* logShot = new EnemyShot("logShot");
				logShot->Init("animations/lumberjack/Bullet.csv", "graphics/LogShot.png");
				logShot->Reset();
				logShot->SetPosition({ position.x - 35 , position.y - 20 });
				logShot->EnemyFire({ -1,0 }, 200.f, 1, -1);
				sceneGame->AddGo(logShot);
				break;
			}
		}
	}
}

void Lumberjack::ToIdle()
{
	EnemyAnimation.Play("animations/lumberjack/Idle.csv");
	skillTimer = 0;
	onSkill = false;

}
