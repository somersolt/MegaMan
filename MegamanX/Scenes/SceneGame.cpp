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
	background->SetOrigin({ background->GetGlobalBounds().left , background->GetGlobalBounds().height / 2 });
	background->SetPosition({ 0,850 });

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

	worldView.setSize({ 1920 / 3, 1080 / 3 });
	backgroundView.setSize({ 1920 /3 ,1080 /3 });
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
		mapHitBox->SetActive(true);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::F6))
	{
		mapHitBox->SetActive(false);
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

sf::Vector2f SceneGame::PlayerBoundsWorldToLocal(sf::Vector2f playerBoundsPos)
{
	sf::Vector2f imageWorldPos = collisionMapSprite.getPosition();
	sf::Transform worldToLocal = collisionMapSprite.getInverseTransform();
	sf::Vector2f boundingBoxLocalPos = worldToLocal.transformPoint(playerBoundsPos);

	return boundingBoxLocalPos;
}