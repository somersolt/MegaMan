#include "pch.h"
#include "SceneGame.h"
#include "Player/Player.h"
#include "Enemy/Stump.h"
#include "Enemy/Rabbit.h"
#include "Enemy/Lumberjack.h"
#include "Enemy/SnowThrower.h"
#include "Enemy/Wheeler.h"
#include "Enemy/Bat.h"
#include "Enemy/Bee.h"
#include "Enemy/Bee2.h"
#include "Enemy/Ostrich.h"

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
	background->SetTexture("graphics/background.png");
	AddGo(background, Scene::BackGround);
	background->SetOrigin({ background->GetGlobalBounds().left , background->GetGlobalBounds().height / 2 });
	background->SetPosition({ -100,750 });

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

	player = new Player("player", collisionMapImage);
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

	Rabbit* rabbit1 = new Rabbit("enemy", collisionMapImage);
	AddGo(rabbit1);
	rabbit1->SetOrigin(Origins::BC);
	rabbit1->SetPosition({ 300, 900 });


	Lumberjack* lumberjack = new Lumberjack("enemy", collisionMapImage, "stump1");
	AddGo(lumberjack);
	lumberjack->SetOrigin(Origins::BC);
	lumberjack->SetPosition({ 495, 900 });

	Lumberjack* lumberjack2 = new Lumberjack("enemy", collisionMapImage, "stump2");
	AddGo(lumberjack2);
	lumberjack2->SetOrigin(Origins::BC);
	lumberjack2->SetPosition({ 1005, 865 });

	SnowThrower* snowThrower1 = new SnowThrower("enemy", collisionMapImage);
	AddGo(snowThrower1);
	snowThrower1->SetOrigin(Origins::BC);
	snowThrower1->SetPosition({ 6611, 353 });

	SnowThrower* snowThrower2 = new SnowThrower("enemy", collisionMapImage);
	AddGo(snowThrower2);
	snowThrower2->SetOrigin(Origins::BC);
	snowThrower2->SetPosition({ 6759, 368 });

	SnowThrower* snowThrower3 = new SnowThrower("enemy", collisionMapImage);
	AddGo(snowThrower3);
	snowThrower3->SetOrigin(Origins::BC);
	snowThrower3->SetPosition({ 6873, 314 });

	SnowThrower* snowThrower4 = new SnowThrower("enemy", collisionMapImage);
	AddGo(snowThrower4);
	snowThrower4->SetOrigin(Origins::BC);
	snowThrower4->SetPosition({ 7091, 225 });

	SnowThrower* snowThrower5 = new SnowThrower("enemy", collisionMapImage);
	AddGo(snowThrower5);
	snowThrower5->SetOrigin(Origins::BC);
	snowThrower5->SetPosition({ 7315, 145 });

	Scene::Init();

	worldView.setSize(viewSize);
	backgroundView.setSize(viewSize);


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
	viewBounds.width = worldView.getCenter().x + worldView.getSize().x / 2.f;
	viewBounds.height = worldView.getCenter().y + worldView.getSize().y / 2.f;

	Scene::Update(dt);

	if (InputMgr::GetKeyDown(sf::Keyboard::F5))
	{
		Bat* bat = new Bat("enemy", collisionMapImage);
		bat->Init();
		bat->Reset({ 300, 900 });
		AddGo(bat);
		bat->SetOrigin(Origins::BC);
		bat->SetPosition({ 300, 900 });
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::F6))
	{
		Bee* bee = new Bee("enemy", collisionMapImage);
		bee->Init();
		bee->Reset();
		AddGo(bee);
		bee->SetOrigin(Origins::BC);
		bee->SetPosition({ 300, 850 });
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::F7))
	{
		Bee2* bee2 = new Bee2("enemy", collisionMapImage);
		bee2->Init();
		bee2->Reset();
		AddGo(bee2);
		bee2->SetOrigin(Origins::BC);
		bee2->SetPosition({ 300, 850 });
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::F8))
	{
		Ostrich* ostrich = new Ostrich("enemy", collisionMapImage);
		ostrich->Init();
		ostrich->Reset();
		AddGo(ostrich);
		ostrich->SetOrigin(Origins::BC);
		ostrich->SetPosition({ 300, 850 });
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::F9))
	{
		Wheeler* wheeler = new Wheeler("enemy", collisionMapImage);
		wheeler->Init();
		wheeler->Reset();
		AddGo(wheeler);
		wheeler->SetOrigin(Origins::BC);
		wheeler->SetPosition({ 2440, 680 });
	}
}

void SceneGame::LateUpdate(float dt)
{
	Scene::LateUpdate(dt);

	preWorldViewCenter = worldView.getCenter();
	worldViewCenter = { player->GetPosition().x , player->GetPosition().y - 30 };


	viewXMax = map->GetGlobalBounds().left + map->GetGlobalBounds().width - viewSize.x / 2;
	viewXMin = map->GetGlobalBounds().left + viewSize.x / 2;
	viewYMax = map->GetGlobalBounds().top + map->GetGlobalBounds().height - viewSize.y / 2 - 15;
	viewYMin = map->GetGlobalBounds().top + viewSize.y / 2;


	if (player->GetPosition().x > 0 && player->GetPosition().x < 1780)
	{
		viewYMin = 771 + viewSize.y / 2;
	}
	if (player->GetPosition().x > 0 && player->GetPosition().x < 1620)
	{
		viewYMax = 1015 - viewSize.y / 2;
	}
	// 1번 구역 카메라 제한

	if (player->GetPosition().x > 1956 && player->GetPosition().x < 2235)
	{
		viewYMin = 1027 + viewSize.y / 2;
	}

	if (player->GetPosition().x > 2308 && player->GetPosition().x < 2820 && player->GetPosition().y > 450)
	{
		viewXMax = 2820 - viewSize.x / 2;
	}

	if (player->GetPosition().x > 2308 && player->GetPosition().x < 2820 && player-> GetPosition().y < 1020 && player->GetPosition().y > 450)
	{
		viewXMax = 2820 - viewSize.x / 2;
		viewXMin = 2303 + viewSize.x / 2;
	}
	if (player->GetPosition().x > 2308)
	{
		viewXMin = 2303 + viewSize.x / 2;
	}
	if (player->GetPosition().y < 503 && player->GetPosition().x > 2308 && player->GetPosition().x < 4000)
	{
		viewYMin = 259 + viewSize.y / 2;
	}
	if (player->GetPosition().y < 503 && player->GetPosition().x > 2600 && player->GetPosition().x < 3339)
	{
		viewYMax = 503 - viewSize.y / 2;
	}

	if (player->GetPosition().x > 4070 && player->GetPosition().x < 4450)
	{
		viewYMin = 515 + viewSize.y / 2;
	}

	// 2번 구역 카메라 제한

	if (player->GetPosition().x > 4710 && player->GetPosition().x < 5900)
	{
		viewYMin = 491 + viewSize.y / 2;
		viewYMax = 735 - viewSize.y / 2;
	}





	worldViewCenter.x = Utils::Clamp(worldViewCenter.x, viewXMin, viewXMax);
	worldViewCenter.y = Utils::Clamp(worldViewCenter.y, viewYMin, viewYMax);

	worldView.setCenter(worldViewCenter);
	std::cout << player->GetPosition().y << std::endl;

	worldViewMoment = worldViewCenter - preWorldViewCenter;
	backgroundViewMoment = worldViewMoment * 0.5f;
	backgroundViewCenter = backgroundView.getCenter() + backgroundViewMoment;
	if (isStart)
	{
		backgroundViewCenter.y = worldViewCenter.y + 30;
		isStart = false;
	}
	backgroundView.setCenter(backgroundViewCenter);
}

void SceneGame::FixedUpdate(float dt)
{
	FindGoAll("enemy", enemyList, Scene::Layers::World);
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