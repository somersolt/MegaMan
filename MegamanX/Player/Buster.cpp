#include "pch.h"
#include "Buster.h"
#include "SceneGame.h"
//TO-DO #include enemy

Buster::Buster(const std::string& name) : SpriteGo(name)
{
}

void Buster::Fire(const sf::Vector2f dir, float s, int d, Sides side)
{
	SetTexture("graphics/Buster.png");
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
	if (side == Sides::Left)
	{
		direction = -direction;
		SetFlipX(true);
	}
		busterAnimation.Play("animations/Buster.csv");
}

void Buster::Init()
{
	GameObject::Init();
	busterAnimation.SetTarget(&sprite);
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
	//TO-DO 적과 충돌처리
	//scenegame에 접속해서? 아무튼 적을 찾아서 닿으면 데미지주고삭제
}

void Buster::Reset()
{
	SpriteGo::Reset();
	//sprite.setScale({ 2.f , 2.f }); // 크기조절 만약에
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
}
