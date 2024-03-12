#pragma once
#include "SpriteGo.h"
class SceneGame;
class Player;

class Stump : public SpriteGo
{
protected:
	SceneGame* sceneGame = nullptr;
	Player* player = nullptr;
	sf::FloatRect stumpBounds;
	sf::Vector2f boundingBoxWorldPos;
	sf::Vector2f boundingBoxLocalPos;

	sf::Color collisionColor = { 255 , 0 , 0 }; 
	sf::Color sideCollisionColor = { 0 , 255 , 0 }; 
	int stumpCount = 3;

public:

	Stump(const std::string& name = "");
	~Stump() override = default;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;


	int startX;
	int endX;
	int startY;
	int endY;


	// 
	// TO-DO 그리는 함수
	// -> 이거 플레이어가 들고있는 맵에 동기화 처리 해야됨
	// getimage 받아서 셋 해야겟네
	//


};

