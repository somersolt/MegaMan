#include "pch.h"
#include "Stump.h"
#include "SceneGame.h"
#include "Player/Player.h"
#include "Player/Buster.h"
#include "Tree.h"

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
	SetTexture(textureId);
	stumpBounds = GetGlobalBounds();
	boundingBoxWorldPos = stumpBounds.getPosition();
	boundingBoxLocalPos = sceneGame->PlayerBoundsWorldToMapLocal(boundingBoxWorldPos);

	startX = static_cast<int>(boundingBoxLocalPos.x);
	endX = static_cast<int>(boundingBoxLocalPos.x + 28);
	startY = static_cast<int>(boundingBoxLocalPos.y);
	endY = static_cast<int>(boundingBoxLocalPos.y + 16);

	sceneGame->SetPixelToMap(startX, endX, startY, endY, collisionColor);
	sceneGame->SetPixelToMap(startX, endX, startY, endY, sideCollisionColor);

}

void Stump::Update(float dt)
{
	stumpTimer += dt;

	if (!firstTree && !secondTree && treeChange && stumpTimer > 3.f)
	{
		Tree* tree = new Tree("enemy");
		tree->Init();
		tree->SetStump(name);
		tree->SetTreeNumber(1);
		tree->Reset();
		sceneGame->AddGo(tree);
		tree->SetPosition(GetPosition());
		treeChange = false;
	}
	if (firstTree && !secondTree && treeChange && stumpTimer > 3.f)
	{
		Tree* tree = new Tree("enemy");
		tree->Init();
		tree->SetStump(name);
		tree->SetTreeNumber(2);
		tree->Reset();
		sceneGame->AddGo(tree);
		tree->SetPosition({ GetPosition().x ,GetPosition().y - 16 });
		treeChange = false;
	}

	if (!firstTree && secondTree && treeChange)
	{
		auto& list = sceneGame->GetEnemyList();
		for (auto go : list)
		{
			if (!go->GetActive())
			{
				continue;
			}
			Tree* tree = dynamic_cast<Tree*>(go);
			if (tree != nullptr)
			{
				if(tree->GetStump() == name)
				{
					tree->SetFallDown(true);
					treeChange = false;
				}
			}
		}
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::E))
	{
		treeChange = true;
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::R))
	{
		sceneGame->SetPixelToMap(startX, endX, startY, endY, {0,0,0,0});
	}
	
}

void Stump::SetFirstTree(bool t)
{
	treeChange = true;
	firstTree = t;
	sceneGame->SetPixelToMap(startX, endX, startY, endY, collisionColor);
	sceneGame->SetPixelToMap(startX, endX, startY, endY, sideCollisionColor);
}

void Stump::SetSecondTree(bool t)
{
	stumpTimer = 0;
	treeChange = true;
	secondTree = t;
	sceneGame->SetPixelToMap(startX, endX, startY, endY, collisionColor);
	sceneGame->SetPixelToMap(startX, endX, startY, endY, sideCollisionColor);
}

