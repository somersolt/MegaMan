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

	Bat* bat = new Bat("enemy", collisionMapImage);
	bat->Init();
	bat->Reset({ 1663,875 });
	AddGo(bat);
	bat->SetOrigin(Origins::BC);
	bat->SetPosition({ 1663,875 });
	Bat* bat2 = new Bat("enemy", collisionMapImage);
	bat2->Init();
	bat2->Reset({ 1785,940 });
	AddGo(bat2);
	bat2->SetOrigin(Origins::BC);
	bat2->SetPosition({ 1785,940 });
	Bat* bat3 = new Bat("enemy", collisionMapImage);
	bat3->Init();
	bat3->Reset({ 1816,1040 });
	AddGo(bat3);
	bat3->SetOrigin(Origins::BC);
	bat3->SetPosition({ 1816,1040 });
	Bat* bat4 = new Bat("enemy", collisionMapImage);
	bat4->Init();
	bat4->Reset({ 1870, 1100 });
	AddGo(bat4);
	bat4->SetOrigin(Origins::BC);
	bat4->SetPosition({ 1870 ,1100 });
	Bat* bat5 = new Bat("enemy", collisionMapImage);
	bat5->Init();
	bat5->Reset({ 1907, 1130 });
	AddGo(bat5);
	bat5->SetOrigin(Origins::BC);
	bat5->SetPosition({ 1907, 1130 });
	Bat* bat6 = new Bat("enemy", collisionMapImage);
	bat6->Init();
	bat6->Reset({ 1937, 1130 });
	AddGo(bat6);
	bat6->SetOrigin(Origins::BC);
	bat6->SetPosition({ 1937, 1130 });
	Bat* bat7 = new Bat("enemy", collisionMapImage);
	bat7->Init();
	bat7->Reset({ 2074, 1100 });
	AddGo(bat7);
	bat7->SetOrigin(Origins::BC);
	bat7->SetPosition({ 2074, 1100 });
	Bat* bat8 = new Bat("enemy", collisionMapImage);
	bat8->Init();
	bat8->Reset({ 2265 ,1100 });
	AddGo(bat8);
	bat8->SetOrigin(Origins::BC);
	bat8->SetPosition({ 2265 ,1100 });
	Bat* bat9 = new Bat("enemy", collisionMapImage);
	bat9->Init();
	bat9->Reset({ 2659, 1060 });
	AddGo(bat9);
	bat9->SetOrigin(Origins::BC);
	bat9->SetPosition({ 2659, 1060 });

	Ostrich* ostrich1 = new Ostrich("enemy", collisionMapImage);
	ostrich1->Init();
	ostrich1->Reset();
	AddGo(ostrich1);
	ostrich1->SetOrigin(Origins::BC);
	ostrich1->SetPosition({ 3328, 383 });
	Ostrich* ostrich2 = new Ostrich("enemy", collisionMapImage);
	ostrich2->Init();
	ostrich2->Reset();
	AddGo(ostrich2);
	ostrich2->SetOrigin(Origins::BC);
	ostrich2->SetPosition({ 3538 ,447 });
	Ostrich* ostrich3 = new Ostrich("enemy", collisionMapImage);
	ostrich3->Init();
	ostrich3->Reset();
	AddGo(ostrich3);
	ostrich3->SetOrigin(Origins::BC);
	ostrich3->SetPosition({ 3641, 480 });
	Ostrich* ostrich4 = new Ostrich("enemy", collisionMapImage);
	ostrich4->Init();
	ostrich4->Reset();
	AddGo(ostrich4);
	ostrich4->SetOrigin(Origins::BC);
	ostrich4->SetPosition({ 3962, 671 });

	CreateMonster(monster::rabbit, { 345,937 });
	CreateMonster(monster::rabbit, { 586,935});
	CreateMonster(monster::rabbit, { 774,960 });
	CreateMonster(monster::rabbit, { 892,892 });
	CreateMonster(monster::rabbit, { 1189,943 });
	CreateMonster(monster::bee, { 520,830 });
	CreateMonster(monster::bee, { 774,875 });
	CreateMonster(monster::bee, { 1018,820 });
	CreateMonster(monster::bee, { 1212,820 });
	CreateMonster(monster::bee2, { 3111,370 });
	CreateMonster(monster::bee2, { 3242,431 });
	CreateMonster(monster::bee2, { 3361,384 });
	CreateMonster(monster::bee2, { 3361,320 });
	CreateMonster(monster::bee2, { 3650,450 });
	CreateMonster(monster::bee2, { 3889,600 });
	

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
	if (InputMgr::GetKeyDown(sf::Keyboard::Q))
	{
		mapHitBox->SetActive(true);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::W))
	{
		mapHitBox->SetActive(false);
	}

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
		wheeler->Reset(-1);
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

	if (player->GetPosition().x > 2308 && player->GetPosition().x < 2820 && player->GetPosition().y < 1020 && player->GetPosition().y > 450)
	{
		viewXMax = 2820 - viewSize.x / 2;
		viewXMin = 2303 + viewSize.x / 2;
	}
	if (player->GetPosition().x > 2308 && player->GetPosition().y < 1020)
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

	if (player->GetPosition().x > 4710 && player->GetPosition().x < 5500)
	{
		viewYMin = 491 + viewSize.y / 2;
		viewYMax = 735 - viewSize.y / 2;
	}
	if (player->GetPosition().x > 5500)
	{
		viewYMax = 735 - viewSize.y / 2;
	}
	if (player->GetPosition().x > 6100)
	{
		viewYMax = 480 - viewSize.y / 2;
	}
	if (player->GetPosition().x > 6800)
	{
		viewYMax = 420 - viewSize.y / 2;
	}
	if (player->GetPosition().x > 7080)
	{
		viewYMax = 270 - viewSize.y / 2;
	}
	if (player->GetPosition().x > 7300)
	{
		viewYMax = 244 - viewSize.y / 2;
	}
	// 3번 구역 카메라 제한


	worldViewCenter.x = Utils::Clamp(worldViewCenter.x, viewXMin, viewXMax);
	worldViewCenter.y = Utils::Clamp(worldViewCenter.y, viewYMin, viewYMax);

	worldView.setCenter(worldViewCenter);
	std::cout << player->GetPosition().x << "/" << player->GetPosition().y << std::endl;

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

void SceneGame::CreateMonster(monster monster, sf::Vector2f pos)
{
	switch (monster)
	{
	case SceneGame::monster::rabbit:
	{
		Rabbit* rabbit = new Rabbit("enemy", collisionMapImage);
		rabbit->Init();
		rabbit->Reset();
		AddGo(rabbit);
		rabbit->SetOrigin(Origins::BC);
		rabbit->SetPosition(pos);
	}
	break;
	case SceneGame::monster::bee:
	{
		Bee* bee = new Bee("enemy", collisionMapImage);
		bee->Init();
		bee->Reset();
		AddGo(bee);
		bee->SetOrigin(Origins::BC);
		bee->SetPosition(pos);
	}
	break;
	case SceneGame::monster::bee2:
	{
		Bee2* bee2 = new Bee2("enemy", collisionMapImage);
		bee2->Init();
		bee2->Reset();
		AddGo(bee2);
		bee2->SetOrigin(Origins::BC);
		bee2->SetPosition(pos);
	}

	break;
	}
}

void SceneGame::CreateWheeler(int g, sf::Vector2f pos)
{
	Wheeler* wheeler = new Wheeler("enemy", collisionMapImage);
	wheeler->Init();
	wheeler->Reset(g);
	AddGo(wheeler);
	wheeler->SetOrigin(Origins::BC);
	wheeler->SetPosition(pos);
}
