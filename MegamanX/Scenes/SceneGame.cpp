#include "pch.h"
#include "SceneGame.h"
#include "Player/Player.h"
#include "Enemy/Stump.h"
#include "Enemy/Rabbit.h"

SceneGame::SceneGame(SceneIds id) : Scene(id)
{
}

void SceneGame::SetStatus(Status newStatus)
{
	currentStatus = newStatus;
	switch (currentStatus)
	{
	case SceneGame::Status::Playing:
		FRAMEWORK.SetTimeScale(1.f);
		break;
	case SceneGame::Status::Pause:
		FRAMEWORK.SetTimeScale(0.f);
		break;
	}
}

void SceneGame::Init()
{

	background = new SpriteGo("background");
	background->SetTexture("graphics/background_ice.png");
	AddGo(background, Scene::BackGround );
	background->SetOrigin({ background->GetGlobalBounds().left , background->GetGlobalBounds().height / 2 });
	background->SetPosition({ -100,850 });
	  
	map = new SpriteGo("Map");
	map->SetTexture("graphics/chill_penguin_stage.png");
	map->sortOrder = -3;
	AddGo(map);
	map->SetPosition({ 0, 0 });

	collisionMapImage.loadFromFile("graphics/chill_penguin_stage_collision.png");
	collisionMapTexture.loadFromImage(collisionMapImage);
	collisionMapSprite.setTexture(collisionMapTexture);
	collisionMapSprite.setPosition({ 0, 0 });

	mapHitBox = new SpriteGo("mapHitBox");
	mapHitBox->SetTexture("graphics/chill_penguin_stage_collision.png");
	AddGo(mapHitBox);
	mapHitBox->SetPosition({ 0, 0 });
	mapHitBox->SetActive(false);

	// 플레이어

	player = new Player("player" , collisionMapImage);
	AddGo(player);
	player->SetPosition({ 0,0 });

	Stump* stump = new Stump("stump1");
	AddGo(stump);
	stump->SetOrigin(Origins::BC);
	stump->SetPosition({ 440, 900 });

	Stump* stump2 = new Stump("stump2");
	AddGo(stump2);
	stump2->SetOrigin(Origins::BC);
	stump2->SetPosition({ 950, 865 });

	Rabbit* rabbit1 = new Rabbit("enemy" , collisionMapImage);
	AddGo(rabbit1);
	rabbit1->SetOrigin(Origins::BC);
	rabbit1->SetPosition({ 300, 900 });

	Scene::Init();

	worldView.setSize({ 1920 / 3, 1080 / 3 });
	backgroundView.setSize({ 1920 /3 ,1080 /3 });

	sf::RectangleShape pillar(sf::Vector2f(200, 600));
	pillar.setPosition(0, 200);
	pillar.setFillColor(sf::Color::Red);

}

sf::Vector2f SceneGame::GetPlayerPostion()
{
	return player->GetPosition();
}

void SceneGame::Release()
{
	Scene::Release();
}

void SceneGame::Enter()
{
	Scene::Enter();

	FRAMEWORK.GetWindow().setMouseCursorVisible(false);

}

void SceneGame::Exit()
{
	Scene::Exit();
	FRAMEWORK.GetWindow().setMouseCursorVisible(true);
}


void SceneGame::Update(float dt)
{
	FindGoAll("enemy", enemyList, Scene::Layers::World);
	viewBounds.left = worldView.getCenter().x - worldView.getSize().x / 2.f;
	viewBounds.top = worldView.getCenter().y - worldView.getSize().y / 2.f;
	viewBounds.width = worldView.getSize().x;
	viewBounds.height = worldView.getSize().y;

	Scene::Update(dt);

	if (InputMgr::GetKeyDown(sf::Keyboard::F5))
	{
		mapHitBox->SetActive(true);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::F6))
	{
		mapHitBox->SetActive(false);
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::R))
	{
		Rabbit* rabbit2 = new Rabbit("enemy", collisionMapImage);
		rabbit2->Init();
		rabbit2->Reset();
		AddGo(rabbit2);
		rabbit2->SetOrigin(Origins::BC);
		rabbit2->SetPosition({ 300, 900 });
	}


}

void SceneGame::LateUpdate(float dt)
{
	Scene::LateUpdate(dt);

	sf::Vector2f preWorldViewCenter = worldView.getCenter();
	sf::Vector2f worldViewCenter = player->GetPosition();
	worldViewCenter.y -= 50;
	worldView.setCenter(worldViewCenter);

	sf::Vector2f worldViewMoment = worldViewCenter - preWorldViewCenter;

	sf::Vector2f backgroundViewMoment = worldViewMoment * 0.5f;
	sf::Vector2f backgroundViewCenter = backgroundView.getCenter() + backgroundViewMoment;
	backgroundViewCenter.y = worldViewCenter.y;
	backgroundView.setCenter(backgroundViewCenter);


}

void SceneGame::FixedUpdate(float dt)
{
	Scene::FixedUpdate(dt);
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

void SceneGame::SetPixelToMap(int a, int b, int x, int y, sf::Color c)
{
	if (c == sf::Color({ 255,0,0 }))
	{
		for (int j = 0; j < 12; ++j)
		{
			for (int i = a; i < b + 1; i++)
			{
				collisionMapImage.setPixel(i, x + j, c);
			}
		}
	}
	if (c == sf::Color({ 0,255,0 }))
	{
		for (int j = 0; j < 5; ++j)
		{
			for (int i = x; i < y + 1; i++)
			{
				collisionMapImage.setPixel(a + j, i, c);
			}
		}
		for (int j = 0; j < 5; ++j)
		{
			for (int i = x; i < y + 1; i++)
			{
				collisionMapImage.setPixel(b - j, i, c);
			}
		}
	}

	if (c == sf::Color({ 0,0,0,0 }))
	{
		for (int j = x; j < y + 1; ++j)
		{
			for (int i = a; i < b + 1; i++)
			{
				collisionMapImage.setPixel(i, j, c);
			}
		}
	}

}
sf::Vector2f SceneGame::PlayerBoundsWorldToMapLocal(sf::Vector2f playerBoundsPos)
{
	//sf::Vector2f imageWorldPos = collisionMapSprite.getPosition();
	sf::Transform worldToLocal = collisionMapSprite.getInverseTransform();
	sf::Vector2f boundingBoxLocalPos = worldToLocal.transformPoint(playerBoundsPos);

	return boundingBoxLocalPos;
}