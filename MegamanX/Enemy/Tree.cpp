#include "pch.h"
#include "Tree.h"
#include "Stump.h"
#include "SceneGame.h"
#include "SpriteGoEffect.h"
#include "Player/Player.h"
#include "Player/Buster.h"

Tree::Tree(const std::string& name) : SpriteGo(name)
{
}

void Tree::Init()
{
	SpriteGo::Init();
	textureId = "graphics/stump2.png";
}

void Tree::Reset()
{
	SpriteGo::Reset();
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("player"));
	if (stumpName == "stump1")
	{
		stump = dynamic_cast<Stump*>(SCENE_MGR.GetCurrentScene()->FindGo("stump1"));
	}
	if (stumpName == "stump2")
	{
		stump = dynamic_cast<Stump*>(SCENE_MGR.GetCurrentScene()->FindGo("stump2"));
	}

	//게임 씬 동기화
	SetTexture(textureId);
	boundingBoxWorldPos = treebounds.getPosition();
	boundingBoxLocalPos = sceneGame->PlayerBoundsWorldToMapLocal(boundingBoxWorldPos);
	sortOrder = -1;
	if (treeNumber == 2)
	{
		sortOrder = -2;
	}
}

void Tree::Release()
{
	sceneGame->SetPixelToMap(startX, endX, startY, endY, {0,0,0,0});
	if (treeNumber == 1)
	{
		stump->SetFirstTree(false);
	}
	if (treeNumber == 2)
	{
		stump->SetSecondTree(false);
	}
	SetActive(false);
	sceneGame->RemoveGo(this);
}

void Tree::Update(float dt)
{
	treebounds = GetGlobalBounds();

	boundingBoxWorldPos = treebounds.getPosition();
	boundingBoxLocalPos = sceneGame->PlayerBoundsWorldToMapLocal(boundingBoxWorldPos);	
	startX = static_cast<int>(boundingBoxLocalPos.x);
	endX = static_cast<int>(boundingBoxLocalPos.x + 28);
	startY = static_cast<int>(boundingBoxLocalPos.y);
	endY = static_cast<int>(boundingBoxLocalPos.y + 16);
	
	GrowTimer += dt;

	if (isGrow && (GrowTimer > 0.01f))
	{
		GrowUp();
		GrowTimer = 0;
		GrowUpCount += 1;
	}
	if (GrowUpCount == 16)
	{
		isGrow = false;
		if (treeNumber == 1)
		{
			stump->SetFirstTree(true);
		}
		if (treeNumber == 2)
		{
			stump->SetSecondTree(true);
		}
		stump->SetTreeChange(true);
		GrowUpCount++;
	}
	if (Hp <= 0)
	{
		SpriteGoEffect* boom = new SpriteGoEffect("boom");
		sceneGame->AddGo(boom);
		boom->SetPosition(position);
		boom->Init();
		boom->Reset();
		Release();
	}

	if (fallDown && (GrowTimer > 0.01f) && GrowUpCount > 4)
	{
		FallDown();
		GrowTimer = 0;
		GrowUpCount -= 4;
	}
}

void Tree::OnDamage(int d)
{
	Hp -= d;
}

void Tree::GrowUp()
{
	position.y -= 1.f;
	SetPosition(position);
	sceneGame->SetPixelToMap(startX, endX, startY, endY, collisionColor);
	sceneGame->SetPixelToMap(startX, endX, startY, endY, sideCollisionColor);
}

void Tree::FallDown()
{
	sceneGame->SetPixelToMap(startX, endX, startY, endY, { 0,0,0,0 });
	position.y += 4.f;
	SetPosition(position);
	sceneGame->SetPixelToMap(startX, endX, startY + 4, endY + 4, collisionColor);
	sceneGame->SetPixelToMap(startX, endX, startY + 4, endY + 4, sideCollisionColor);
	treeNumber = 1;
	sortOrder = -1;
	stump->SetFirstTree(true);
	stump->SetSecondTree(false);

}
