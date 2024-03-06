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

	// 임시 맵 
	backgroundtest = new SpriteGo("");
	backgroundtest->SetTexture("graphics/test_stage.png");
	AddGo(backgroundtest);

	backgroundtestLayer = new SpriteGo("");
	backgroundtestLayer->SetTexture("graphics/test_stage_collision.png");
	AddGo(backgroundtestLayer);

	// 플레이어

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

	sf::Vector2f worldViewCenter = worldView.getCenter();
	worldViewCenter = player->GetPosition();
	worldView.setSize({1920/3,1080/3});
	worldView.setCenter(worldViewCenter);
}

void SceneGame::LateUpdate(float dt)
{
	Scene::LateUpdate(dt);
}

void SceneGame::FixedUpdate(float dt)
{
	Scene::FixedUpdate(dt);
}


void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
