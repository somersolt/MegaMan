#include "pch.h"
#include "Stump.h"
#include "SceneGame.h"
#include "Player/Player.h"
#include "Player/Buster.h"

Stump::Stump(const std::string& name) : SpriteGo(name)
{
}

void Stump::Init()
{
	SpriteGo::Init();
	textureId = "graphics/stump1.png";
}

void Stump::Reset()
{
	SpriteGo::Reset();
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("player"));
	//게임 씬 동기화
	SetTexture("graphics/stump1.png");
	stumpBounds = GetGlobalBounds();

	boundingBoxWorldPos = stumpBounds.getPosition();
	boundingBoxLocalPos = sceneGame->PlayerBoundsWorldToMapLocal(boundingBoxWorldPos);

}

void Stump::Update(float dt)
{

	startX = static_cast<int>(boundingBoxLocalPos.x);
	endX = static_cast<int>(boundingBoxLocalPos.x + 28);
	startY =static_cast<int>(boundingBoxLocalPos.y);
	endY = static_cast<int>(boundingBoxLocalPos.y + 16);

	sceneGame->SetPixelToMap(startX, endX, startY, endY, collisionColor);
	
}
