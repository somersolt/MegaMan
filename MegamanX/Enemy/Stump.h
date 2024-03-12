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
	// TO-DO �׸��� �Լ�
	// -> �̰� �÷��̾ ����ִ� �ʿ� ����ȭ ó�� �ؾߵ�
	// getimage �޾Ƽ� �� �ؾ߰ٳ�
	//


};

