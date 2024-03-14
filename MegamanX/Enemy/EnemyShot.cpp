#include "pch.h"
#include "EnemyShot.h"
#include "SceneGame.h"
#include "Player/Player.h"

EnemyShot::EnemyShot(const std::string& name) : SpriteGo(name)
{
}

void EnemyShot::EnemyFire(const sf::Vector2f dir, float s, int d, int h)
{
	SetOrigin(Origins::MC);
	direction = dir;
	speed = s;
	damage = d;
	if (h == -1)
	{
		SetFlipX(true);
	}
	EnemyShotAnimation.Play(animationId);
}

void EnemyShot::Release()
{
	SetActive(false);
	sceneGame->RemoveGo(this);
}

void EnemyShot::Init(std::string id)
{
	GameObject::Init();
	animationId = id;
	EnemyShotAnimation.SetTarget(&sprite);
	textureId = "graphics/rabbitBullet.png";

}

void EnemyShot::Update(float dt)
{
	GameObject::Update(dt);
	EnemyShotAnimation.Update(dt);
	sf::Vector2f pos = position + direction * speed * dt;
	SetPosition(pos);

	if (!GetGlobalBounds().intersects(sceneGame->GetViewBounds()))
	{
		Release();
	}
}

void EnemyShot::FixedUpdate(float dt)
{
	SpriteGo::FixedUpdate(dt);
	if (GetGlobalBounds().intersects(player->GetGlobalBounds()) && player->GetDamageTimer() > 2.f && player->GetCurrentStatus() != Player::Status::Die)
	{
		player->OnDamage(damage);
		Release();
	}
}

void EnemyShot::Reset()
{
	SpriteGo::Reset();
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("player"));
}
