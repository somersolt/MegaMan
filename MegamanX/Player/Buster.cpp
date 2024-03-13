#include "pch.h"
#include "Buster.h"
#include "SceneGame.h"
#include "Enemy/Tree.h"
#include "Enemy/Enemy.h"


Buster::Buster(const std::string& name) : SpriteGo(name)
{
}

void Buster::Fire(const sf::Vector2f dir, float s, int d, Sides side)
{
	
	sprite.setTextureRect({ 61,0, 10,16 });
	SetOrigin({ GetLocalBounds().width - 20, GetLocalBounds().height / 2});

	direction = dir;
	speed = s;
	if (side == Sides::Left)
	{
		direction = -direction;
		SetFlipX(true);
	}
}

void Buster::ChargeFire(const sf::Vector2f dir, float s, int d, Sides side)
{
	direction = dir;
	speed = s;
	damage = d;
	if (side == Sides::Left)
	{
		direction = -direction;
		SetFlipX(true);
	}
	busterAnimation.Play("animations/Buster.csv");
}

void Buster::Release()
{
	SetActive(false);
	sceneGame->RemoveGo(this);
}


void Buster::Init()
{
	GameObject::Init();
	busterAnimation.SetTarget(&sprite);
	textureId = "graphics/Buster.png";
	std::function<void()> ToRelease = std::bind(&Buster::Release, this);
	busterAnimation.AddEvent("animations/BusterHit.csv", 3, ToRelease);
	busterAnimation.AddEvent("animations/ChargeBusterHit.csv", 3, ToRelease);
}

void Buster::Update(float dt)
{
	GameObject::Update(dt);
	busterAnimation.Update(dt);
	sf::Vector2f pos = position + direction * speed * dt;

	//TO-DO 카메라 벗어나면 삭제

	SetPosition(pos);

}

void Buster::FixedUpdate(float dt)
{
	SpriteGo::FixedUpdate(dt);

	auto& list = sceneGame->GetEnemyList();
	for (auto go : list)
	{
		if (!go->GetActive())
		{
			continue;
		}
		if (GetGlobalBounds().intersects(go->GetGlobalBounds()) && !hit)
		{
			hit = true;
			speed = 0;
			if (damage == 1)
			{
				busterAnimation.Play("animations/BusterHit.csv");
			}
			if (damage == 5)
			{
				if (direction.x > 0)
				{
					SetPosition({ position.x + 10 , position.y });
				}
				else
				{
					SetPosition({ position.x - 10 , position.y });
				}
				busterAnimation.Play("animations/ChargeBusterHit.csv");
			}

			Tree* tree = dynamic_cast<Tree*>(go);
			if (tree != nullptr)
			{
				tree->OnDamage(damage);
				break;
			}

			Enemy* enemy = dynamic_cast<Enemy*>(go);
			if (enemy != nullptr)
				enemy->OnDamage(damage);
			break;
		}
	}
}

void Buster::Reset()
{
	SpriteGo::Reset();
	//sprite.setScale({ 2.f , 2.f }); // 크기조절 만약에
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
}
