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
#include "Enemy/Boss.h"
#include "Enemy/Tree.h"
#include "Blizzard.h"

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
	
	waitingCollisionMapImage.loadFromFile("graphics/waitingroom_collision.png");
	bossCollisionMapImage.loadFromFile("graphics/boss_collision.png");


	mapHitBox = new SpriteGo("mapHitBox");
	mapHitBox->SetTexture("graphics/chill_penguin_stage_collision.png");
	AddGo(mapHitBox);
	mapHitBox->SetPosition({ 0, 0 });
	mapHitBox->SetActive(false);

	// 플레이어

	player = new Player("player", collisionMapImage);
	AddGo(player);/*
	player->SetPosition({ 0,0 });*/

	Stump* stump = new Stump("stump1");
	AddGo(stump);
	stump->SetOrigin(Origins::BC);
	stump->SetPosition({ 440, 900 });

	Stump* stump2 = new Stump("stump2");
	AddGo(stump2);
	stump2->SetOrigin(Origins::BC);
	stump2->SetPosition({ 950, 865 });

	EnemySummon();

	BossSummon();

	//// 체력바 UI
	playerLife = new SpriteGo("lifebar");
	playerLife->SetTexture("graphics/lifebar.png");
	AddGo(playerLife, Layers::Ui);

	bossLife = new SpriteGo("boslifebar");
	bossLife->SetTexture("graphics/bosslifebar.png");
	AddGo(bossLife, Layers::Ui);

	for (int i = 0; i < 16; ++i)
	{
		SpriteGo* playerLifeGauge = new SpriteGo("playerLifeGauge");
		playerLifeGauge->SetTexture("graphics/lifegauge.png");
		playerLifeGauge->SetPosition({ 0,0 });
		lifeGaugeList.push_back(playerLifeGauge);
		AddGo(playerLifeGauge, Layers::Ui);
	}
	for (int i = 0; i < 32; ++i)
	{
		SpriteGo* bossLifeGauge = new SpriteGo("playerLifeGauge");
		bossLifeGauge->SetTexture("graphics/lifegauge.png");
		bossLifeGauge->SetPosition({ 0,0 });
		bossGaugeList.push_back(bossLifeGauge);
		AddGo(bossLifeGauge, Layers::Ui);
	}

	pauseMenu = new SpriteGo("pauseMenu");
	pauseMenu->SetTexture("graphics/pausemenu.png");
	AddGo(pauseMenu, Layers::Ui);
	pauseMenu->SetActive(false);

	Scene::Init();

	worldView.setSize(viewSize);
	backgroundView.setSize(viewSize);
	uiView.setSize(viewSize);

	currentStatus = Status::Playing;
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
	std::cout << worldView.getCenter().x << "/" << worldView.getCenter().y << std::endl;
	if (player->GetCurrentStatus() == Player::Status::Die)
	{
		resetTimer += dt;
		if (resetTimer > 3)
		{
			if (isWatingRoom)
			{
				resetTimer = 0;
				player->Reset();
				player->SetPosition({ 7554, 163 });
				EnemyClear();
				BossClear();
				BossSummon();
				collisionMapImage = waitingCollisionMapImage;
				isBossRoom = false;
				isBossRoomView = false;
				BossAppear = false;
				cameraWorkCount = waitingRoomCameraWorkCount + 1;
				bossHp = 0;
				bossHPAnimationCount = 0;
				bossLife->SetActive(false);
				worldView.setCenter({ 7556 , 122 });
			}
			else {
				resetTimer = 0;
				player->Reset();
				EnemyClear();
				EnemySummon();
			}
		}

	}

	bossGaugeTimer += dt;
	playerLife->SetPosition(worldView.getCenter() - worldView.getSize() / 2.f + sf::Vector2f(10, 70));
	for (auto& lifegauge : lifeGaugeList) {
		lifegauge->SetPosition(sf::Vector2f(0, 0));
		lifegauge->SetActive(false);
	}
	int PlayerHP = player->GetPlayerLife();
	if (PlayerHP < 0)
	{
		PlayerHP = 0;
	}

	for (int i = 0; i < PlayerHP && i < lifeGaugeList.size(); ++i) {
		lifeGaugeList[i]->SetPosition(worldView.getCenter() - worldView.getSize() / 2.f + sf::Vector2f(10, 104 - static_cast<float>(i * 2)));
		lifeGaugeList[i]->SetActive(true);
	}

	if (BossAppear)
	{
		bossLife->SetActive(true);
		bossLife->SetPosition(worldView.getCenter() - worldView.getSize() / 2.f + sf::Vector2f(260, 40));
		for (auto& bosslifegauge : bossGaugeList) {
			bosslifegauge->SetPosition(sf::Vector2f(0, 0));
			bosslifegauge->SetActive(false);
		}
		if (appearAnimation && bossGaugeTimer > 0.05f)
		{
			++bossHPAnimationCount;
			bossHp = bossHPAnimationCount;
			bossGaugeTimer = 0;
			if (bossHPAnimationCount == 33)
			{
				appearAnimation = false;
				isBossRoomView = true;
				boss->SetBossStatus(Boss::BossStatus::Idle);
				player->SetWait(false);
				collisionMapImage = bossCollisionMapImage;
			}
			
		}
		if (!appearAnimation && isBossRoomView)
		{
			bossHp = boss->GetBossLife();
		}
		if (bossHp < 0)
		{
			bossHp = 0;
		}

		for (int i = 0; i < bossHp && i < bossGaugeList.size(); ++i) {
			bossGaugeList[i]->SetPosition(worldView.getCenter() - worldView.getSize() / 2.f + sf::Vector2f(260, 106 - static_cast<float>(i * 2)));
			bossGaugeList[i]->SetActive(true);
		}

	}
	pauseMenu->SetPosition(worldView.getCenter() - worldView.getSize() / 2.f);

	uiView.setCenter(worldView.getCenter());
	uiView.setSize(worldView.getSize());

	FindGoAll("enemy", enemyList, Scene::Layers::World);

	viewBounds.left = worldView.getCenter().x - worldView.getSize().x / 2.f;
	viewBounds.top = worldView.getCenter().y - worldView.getSize().y / 2.f;
	viewBounds.width = worldView.getCenter().x + worldView.getSize().x / 2.f;
	viewBounds.height = worldView.getCenter().y + worldView.getSize().y / 2.f;


	Test();

	if (InputMgr::GetKeyDown(sf::Keyboard::Escape))
	{
		if (currentStatus == Status::Playing)
		{
			SetStatus(Status::Pause);
			pauseMenu->SetActive(true);
		}
		else if (currentStatus == Status::Pause)
		{
			SetStatus(Status::Playing);
			pauseMenu->SetActive(false);
		}
	}

	Scene::Update(dt);
}

void SceneGame::LateUpdate(float dt)
{
	Scene::LateUpdate(dt);

	preWorldViewCenter = worldView.getCenter();
	if (!isWatingRoom)
	{
		worldViewCenter = { player->GetPosition().x , player->GetPosition().y - 30 };
	}
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

	if (player->GetPosition().x > 7427 && !isWatingRoom && cameraWorkCount < waitingRoomCameraWorkCount)
	{
		isWatingRoom = true;
		cameraWork = true;
		player->SetWait(true); //플레이어 정지
	}


	if (player->GetPosition().x > 7680 && !isBossRoom && cameraWorkCount < bossRoomCameraWorkCount)
	{
		isBossRoom = true; // 한번만
		cameraWork = true;
		player->SetWait(true); //플레이어 정지
	}

	worldViewCenter.x = Utils::Clamp(worldViewCenter.x, viewXMin, viewXMax);
	worldViewCenter.y = Utils::Clamp(worldViewCenter.y, viewYMin, viewYMax);

	if (!isWatingRoom && !isBossRoom)
	{
		worldView.setCenter(worldViewCenter);
	} // 평소 카메라워크

	worldViewMoment = worldViewCenter - preWorldViewCenter;
	backgroundViewMoment = worldViewMoment * 0.5f;
	backgroundViewCenter = backgroundView.getCenter() + backgroundViewMoment;
	if (isStart)
	{
		backgroundViewCenter.y = worldViewCenter.y + 30;
		isStart = false;
	} //처음 시작시 배경뷰 조절
	if (!isWatingRoom && !isBossRoom)
	{
		backgroundView.setCenter(backgroundViewCenter);
	} // 배경뷰 원근감 조절


	if (isWatingRoom && cameraWork && cameraWorkCount <= waitingRoomCameraWorkCount)
	{
		cameraWorkTimer += dt;
		if (cameraWorkTimer > 0.02)
		{
			worldView.move({ 1,0 });
			player->SetPosition({ player->GetPosition().x + 0.2f, player->GetPosition().y});
			cameraWorkTimer = 0;
			cameraWorkCount++;
			
			if (cameraWorkCount == waitingRoomCameraWorkCount)
			{
				cameraWorkCount++;
				cameraWork = false;
				player->SetWait(false); //플레이어 정지 해제
				collisionMapImage = waitingCollisionMapImage;
			}
		}
	}//대기실 카메라워크

	if (isBossRoom && cameraWork && cameraWorkCount < bossRoomCameraWorkCount)
	{
		cameraWorkTimer += dt;
		if (cameraWorkTimer > 0.02)
		{
			player->SetWait(true);
			worldView.move({ 2,0 });
			player->SetPosition({ player->GetPosition().x + 0.2f, player->GetPosition().y });
			cameraWorkTimer = 0;
			cameraWorkCount++;

			if (cameraWorkCount == bossRoomCameraWorkCount)
			{
				cameraWorkCount++;
				cameraWork = false;
				BossAppear = true;
				boss->SetActive(true); // 보스 등장
			}
		}
	}// 보스룸 카메라워크
	if (isBossRoomView)
	{
		worldView.setCenter({ 7668 + viewSize.x / 2, viewSize.y / 2 });
	}// 보스룸 카메라 고정
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
	case SceneGame::monster::snowThrower:
	{
		SnowThrower* snowThrower = new SnowThrower("enemy", collisionMapImage);
		snowThrower->Init();
		snowThrower->Reset();
		AddGo(snowThrower);
		snowThrower->SetOrigin(Origins::BC);
		snowThrower->SetPosition(pos);
	}
	break;
	case SceneGame::monster::bat:
	{
		Bat* bat = new Bat("enemy", collisionMapImage);
		bat->Init();
		bat->Reset(pos);
		AddGo(bat);
		bat->SetOrigin(Origins::BC);
		bat->SetPosition(pos);
	}
	break;
	case SceneGame::monster::ostrich:
	{
		Ostrich* ostrich = new Ostrich("enemy", collisionMapImage);
		ostrich->Init();
		ostrich->Reset();
		AddGo(ostrich);
		ostrich->SetOrigin(Origins::BC);
		ostrich->SetPosition(pos);
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

void SceneGame::CreateLumberjack(std::string name, sf::Vector2f pos)
{
	Lumberjack* lumberjack = new Lumberjack("enemy", collisionMapImage, name);
	lumberjack->Init();
	lumberjack->Reset();
	AddGo(lumberjack);
	lumberjack->SetOrigin(Origins::BC);
	lumberjack->SetPosition(pos);
}

void SceneGame::BlizzardEffect()
{
	Blizzard* blizzard = new Blizzard("blizzard");
	AddGo(blizzard);
	blizzard->Init();
	blizzard->Reset();
	blizzard->SetOrigin(Origins::TL);
	blizzard->SetPosition({ viewBounds.left,viewBounds.top });
}

void SceneGame::EnemyClear()
{
	for (auto it = enemyList.begin(); it != enemyList.end(); )
	{
		GameObject* go = *it;
		if (go == nullptr)
		{
			break;
		}
		if (dynamic_cast<Boss*>(go) != nullptr || dynamic_cast<Tree*>(go) != nullptr)
		{
			++it;
			continue;
		}
		it = enemyList.erase(it);
		RemoveGo(go);
	}
}

void SceneGame::EnemySummon()
{
	CreateLumberjack("stump1", { 495, 900 });
	CreateLumberjack("stump2", { 1005, 865 });

	CreateMonster(monster::rabbit, { 355,930 });
	CreateMonster(monster::rabbit, { 586,935 });
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
	CreateWheeler(-1, { 2456, 1168 });
	CreateWheeler(-1, { 2561, 1155 });
	CreateWheeler(-1, { 2659, 1123 });
	CreateWheeler(1, { 2620, 993 });
	CreateWheeler(1, { 2356, 915 });
	CreateWheeler(-1, { 2467, 842 });
	CreateWheeler(-1, { 2643, 787 });
	CreateWheeler(1, { 2355, 643 });
	CreateMonster(monster::snowThrower, { 6611, 353 });
	CreateMonster(monster::snowThrower, { 6759, 368 });
	CreateMonster(monster::snowThrower, { 6873, 314 });
	CreateMonster(monster::snowThrower, { 7091, 225 });
	CreateMonster(monster::snowThrower, { 7315, 145 });
	CreateMonster(monster::bat, { 1663,867 });
	CreateMonster(monster::bat, { 1788,950 });
	CreateMonster(monster::bat, { 1816,1015 });
	CreateMonster(monster::bat, { 1870 ,1090 });
	CreateMonster(monster::bat, { 1907, 1120 });
	CreateMonster(monster::bat, { 1937, 1120 });
	CreateMonster(monster::bat, { 2074, 1090 });
	CreateMonster(monster::bat, { 2265 ,1090 });
	CreateMonster(monster::bat, { 2659, 1050 });
	CreateMonster(monster::ostrich, { 3328, 383 });
	CreateMonster(monster::ostrich, { 3538 ,447 });
	CreateMonster(monster::ostrich, { 3641, 480 });
	CreateMonster(monster::ostrich, { 3962, 671 });

}

void SceneGame::BossClear()
{
	RemoveGo(boss);
}

void SceneGame::BossSummon()
{
	boss = new Boss("enemy", collisionMapImage);
	boss->Init();
	boss->Reset();
	AddGo(boss);
	boss->SetOrigin(Origins::BC);
	boss->SetPosition({ 7900, 20 });
	boss->SetActive(false);
}

void SceneGame::Test()
{
	if (InputMgr::GetKeyDown(sf::Keyboard::J))
	{
		player->OnDamage(10);
	}
	//if (InputMgr::GetKeyDown(sf::Keyboard::J))
	//{
	//	EnemyClear();
	//}
	//if (InputMgr::GetKeyDown(sf::Keyboard::K))
	//{
	//	EnemySummon();
	//}

	if (InputMgr::GetKeyDown(sf::Keyboard::Q))
	{
		mapHitBox->SetActive(true);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::W))
	{
		mapHitBox->SetActive(false);
	}

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