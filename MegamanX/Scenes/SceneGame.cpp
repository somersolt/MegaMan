#include "pch.h"
#include "SceneGame.h"
#include "Player/Player.h"


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
	background->SetScale({ 1, 2 });
	background->SetOrigin({ background->GetGlobalBounds().width / 2 , background->GetGlobalBounds().height / 2 - 70 });


	//// ÀÓ½Ã ¸Ê 
	//backgroundtest = new SpriteGo("testMap");
	//backgroundtest->SetTexture("graphics/test_stage.png");
	//AddGo(backgroundtest);
	//backgroundtest->SetPosition({ 1000, 0 });
	 
	// ÁøÂ¥ ¸Ê 
	map = new SpriteGo("Map");
	map->SetTexture("graphics/chill_penguin_stage.png");
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


	// ÇÃ·¹ÀÌ¾î

	player = new Player("player");
	AddGo(player);
	player->SetPosition({ 0,0 });

	Scene::Init();
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
	Scene::Update(dt);

	if (InputMgr::GetKeyDown(sf::Keyboard::F5))
	{
		mapHitBox->SetActive(false);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::F6))
	{
		mapHitBox->SetActive(true);
	}
}

void SceneGame::LateUpdate(float dt)
{
	Scene::LateUpdate(dt);
	sf::Vector2f backgroundViewCenter = player->GetPosition();
	backgroundView.setSize({ 1920 ,1080 });
	backgroundView.setCenter(backgroundViewCenter);

	sf::Vector2f worldViewCenter = worldView.getCenter();
	worldViewCenter = { player->GetPosition().x , player->GetPosition().y - 50 };
	worldView.setSize({ 1920 / 3 , 1080 / 3 });
	worldView.setCenter(worldViewCenter);


	background->SetPosition({ backgroundViewCenter });
}

void SceneGame::FixedUpdate(float dt)
{
	Scene::FixedUpdate(dt);
}


void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

sf::Vector2f SceneGame::PlayerBoundsWorldToLocal(sf::Vector2f playerBoundsPos)
{
	sf::Vector2f imageWorldPos = collisionMapSprite.getPosition();
	sf::Transform worldToLocal = collisionMapSprite.getInverseTransform();
	sf::Vector2f boundingBoxLocalPos = worldToLocal.transformPoint(playerBoundsPos);

	return boundingBoxLocalPos;
}