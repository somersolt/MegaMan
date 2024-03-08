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

	// ÀÓ½Ã ¸Ê 
	backgroundtest = new SpriteGo("testMap");
	backgroundtest->SetTexture("graphics/test_stage.png");
	AddGo(backgroundtest);
	backgroundtest->SetPosition({ 1000, 0 });

	collisionMapImage.loadFromFile("graphics/test_stage_collision2.png");
	collisionMapTexture.loadFromImage(collisionMapImage);
	collisionMapSprite.setTexture(collisionMapTexture);
	collisionMapSprite.setPosition({ 1000, 0 });

	mapHitBox = new SpriteGo("mapHitBox");
	mapHitBox->SetTexture("graphics/test_stage_collision2.png");
	AddGo(mapHitBox);
	mapHitBox->SetPosition({ 1000, 0 });

	////ÁøÂ¥ ¸Ê
	//background = new SpriteGo("Map");
	//background->SetTexture("graphics/chill_penguin_stage.png");
	//AddGo(background);
	//background->SetPosition({ 1000, 0 });



	// ÇÃ·¹ÀÌ¾î

	player = new Player("player");
	AddGo(player);
	player->SetPosition({ 1000,0 });

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
	//std::cout << player->GetPosition().x << "/" << player->GetPosition().y << std::endl;
}

void SceneGame::LateUpdate(float dt)
{
	Scene::LateUpdate(dt);
	sf::Vector2f worldViewCenter = worldView.getCenter();
	worldViewCenter = player->GetPosition();
	worldView.setSize({ 1920 / 3,1080 / 3 });
	worldView.setCenter(worldViewCenter);
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